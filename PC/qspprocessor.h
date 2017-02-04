#ifndef QSPPROCESSOR_H
#define QSPPROCESSOR_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>

class QSPProcessor: public QObject
{
    Q_OBJECT
public:
    QSPProcessor(QObject *_parent=0);

    static QStringList availablePorts();

    bool openPort(int _portid);
    void closePort();

private slots:
    void __showError(QSerialPort::SerialPortError _errorid);
    void __readData();
    qint64 __writeData(const QByteArray &_data);

private:
    QSerialPort serialport;
};

#endif // QSPPROCESSOR_H
