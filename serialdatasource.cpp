#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QPointF>
#include <QList>
#include <QtQml>
#include "serialhandler.h" // Assuming this is where SerialHandler is defined
#include <QtCharts> // Include the necessary Qt Charts headers
#include "serialdatasource.h" // Include your header file here


SerialDataSource::SerialDataSource(SerialHandler *serialHandler, QObject *parent)
    : QObject(parent), m_serialHandler(serialHandler) {
    // Constructor implementation
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    connect(m_serialHandler, &SerialHandler::dataReceived, this, &SerialDataSource::handleDataReceived);
}

void SerialDataSource::startReadingData() {
    // Start reading data periodically from the serial port
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SerialDataSource::readSerialData);
    timer->start(1000); // Adjust the interval as needed
}

void SerialDataSource::readSerialData() {
    m_serialHandler->writeData("Read data command"); // Send a command to read data from the serial port
}

void SerialDataSource::handleDataReceived(const QByteArray &data) {
    // Process received data and generate points for the series
    QList<QPointF> points;
    qreal x = QDateTime::currentMSecsSinceEpoch() / 1000.0; // Assuming x represents time
    qreal y = parseReceivedData(data); // Function to parse received data to get y value

    points.append(QPointF(x, y));

    emit newData(points);
}

qreal SerialDataSource::parseReceivedData(const QByteArray &data) {
    // Parse the received data to extract the y value
    // Replace this with your actual parsing logic
    // For example, converting received data to a number
    return data.toDouble();
}


SerialDataSource::~SerialDataSource() {
    if (m_serialHandler) {
        // Disconnect slots to prevent any further interactions
        disconnect(m_serialHandler, &SerialHandler::dataReceived, this, &SerialDataSource::handleDataReceived);

        // Stop any ongoing operations with the SerialHandler, if necessary
        // ...

        // Delete the SerialHandler instance
        delete m_serialHandler;
        m_serialHandler = nullptr; // Set to nullptr to avoid dangling pointers
    }
}
