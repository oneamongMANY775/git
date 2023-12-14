#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QSerialPort>
#include <QQmlEngine>
#include <QStringList>

class SerialHandler : public QObject {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit SerialHandler(QObject *parent = nullptr);
    ~SerialHandler() override;

    bool openPort(const QString &portName, int baudRate);
    void closePort();
    bool writeData(const QByteArray &data);
    QByteArray readData();

signals:
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);

private:
    QSerialPort serialPort;
    QStringList availablePorts;
    bool isPortOpen() const;
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
};

#endif // SERIALHANDLER_H

