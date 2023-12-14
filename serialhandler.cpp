    #include "SerialHandler.h"

#include <QDebug>

SerialHandler::SerialHandler(QObject *parent)
    : QObject(parent)
{
    connect(&serialPort, &QSerialPort::readyRead, this, &SerialHandler::handleReadyRead);
    connect(&serialPort, &QSerialPort::errorOccurred, this, &SerialHandler::handleError);

}

SerialHandler::~SerialHandler()
{
    closePort();
}

bool SerialHandler::openPort(const QString &portName, int baudRate)
{
    if (isPortOpen()) {
        emit errorOccurred("Port already open");
        return false;
    }

    if (!availablePorts.contains(portName)) {
        emit errorOccurred("Invalid port name");
        return false;
    }

    serialPort.setPortName(portName);
    serialPort.setBaudRate(baudRate);

    if (!serialPort.open(QIODevice::ReadWrite)) {
        emit errorOccurred(serialPort.errorString());
        return false;
    }

    return true;
}

void SerialHandler::closePort()
{
    if (!isPortOpen()) {
        return;
    }

    serialPort.close();
}

bool SerialHandler::writeData(const QByteArray &data)
{
    if (!isPortOpen()) {
        emit errorOccurred("Port not open");
        return false;
    }

    if (data.endsWith('\n')) {
        return serialPort.write(data) == data.size();
    } else {
        return serialPort.write(data + '\n') == data.size() + 1;
    }
}

QByteArray SerialHandler::readData()
{
    if (!isPortOpen()) {
        emit errorOccurred("Port not open");
        return "";
    }

    static QByteArray bufferedData; // Static variable to store leftover data between calls

    QByteArray data;
    while (serialPort.canReadLine()) {
        QByteArray line = serialPort.readLine();
        data += line;

        if (line.endsWith('\n')) {
            if (bufferedData.isEmpty()) {
                // If there's no buffered data, emit the line as a complete unit
                emit dataReceived(data.trimmed());
                data.clear();
            } else {
                // Combine buffered data with the new line and emit as a complete unit
                QByteArray completeData = bufferedData + data;
                emit dataReceived(completeData.trimmed());
                data.clear();
                bufferedData.clear();
            }
        } else {
            // If the line does not end with '\n', buffer it for future concatenation
            bufferedData += line;
        }
    }

    return data; // Returning any incomplete data (not terminated with '\n')
}


bool SerialHandler::isPortOpen() const
{
    return serialPort.isOpen();
}

void SerialHandler::handleReadyRead()
{
    QByteArray data = readData();
    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
}

void SerialHandler::handleError(QSerialPort::SerialPortError error)
{
    emit errorOccurred(serialPort.errorString());
}

