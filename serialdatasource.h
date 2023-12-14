#ifndef SERIALDATASOURCE_H
#define SERIALDATASOURCE_H
#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QPointF>
#include <QList>
#include <QtQml>
#include <QAbstractSeries>
#include <QAbstractAxis>
#include <QQmlEngine>
#include "serialhandler.h" // Assuming this is where SerialHandler is defined

class SerialDataSource : public QObject {
    Q_OBJECT
    QML_ELEMENT

public:
    Q_INVOKABLE explicit SerialDataSource(SerialHandler *serialHandler = nullptr, QObject *parent = nullptr);
    Q_INVOKABLE void startReadingData();

    ~SerialDataSource();

signals:
    void newData(const QList<QPointF> &points);

private slots:
    void readSerialData();
    void handleDataReceived(const QByteArray &data);

private:
    qreal parseReceivedData(const QByteArray &data);

    SerialHandler *m_serialHandler;
};


#endif // SERIALDATASOURCE_H
