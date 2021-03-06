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
    explicit QSPProcessor(QObject *_parent=0);

    static QStringList availablePorts();
    bool openPort(int _portid);    
    qint64 writeToPort(const QByteArray &_data);
    void closePort();

private slots:
    void __showError(QSerialPort::SerialPortError _errorid);
    void __readData();
    qint64 __writeData(const QByteArray &_data);

private:
    QSerialPort serialport;
    QByteArray repeatbuffer;
};

#endif // QSPPROCESSOR_H
