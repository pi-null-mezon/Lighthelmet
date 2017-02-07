#include "qspprocessor.h"

#include <QDebug>

QSPProcessor::QSPProcessor(QObject *_parent) :
    QObject(_parent)
{
    connect(&serialport, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(__showError(QSerialPort::SerialPortError)));
    connect(&serialport, SIGNAL(readyRead()), this, SLOT(__readData()));
}

QStringList QSPProcessor::availablePorts()
{
    QStringList _portnamelist;
    QList<QSerialPortInfo> _portinfolist = QSerialPortInfo::availablePorts();
    if(_portinfolist.size() > 0) {
        qInfo("Список доступных для подключения портов:");
        for(int i = 0; i < _portinfolist.size(); ++i) {
            QString _portname = (_portinfolist[i]).portName().append(" ") + (_portinfolist[i]).description();
            _portnamelist << _portname;
            qInfo("%d) %s", i, _portname.toUtf8().constData());
        }
    } else {
        qInfo("Нет доступных для подключения портов. Проверьте надёжность подключения.");
    }
    return _portnamelist;
}

bool QSPProcessor::openPort(int _portid)
{
    QList<QSerialPortInfo> _portinfolist = QSerialPortInfo::availablePorts();
    if(_portid <= -1 || _portid >= _portinfolist.size()) {
        qWarning("Порт с указанным идентификатором не найден!");
        return false;
    }

    closePort();
    serialport.setPort(_portinfolist[_portid]);
    if(serialport.open(QIODevice::ReadWrite)) {
        bool _result;
        if(    serialport.setBaudRate(QSerialPort::Baud9600)
            && serialport.setFlowControl(QSerialPort::NoFlowControl)
            && serialport.setDataBits(QSerialPort::Data8)
            && serialport.setParity(QSerialPort::NoParity)
            && serialport.setStopBits(QSerialPort::OneStop)
            && serialport.setDataTerminalReady(true) ) {
            _result = true;
        } else {
            _result = false;
        }
        qDebug("\n-----------------\nПорт открыт с настройками:\n-----------------");
        qDebug() << serialport.portName();
        qDebug() << " - baudRate = " << serialport.baudRate() << " bps";
        qDebug() << " - dataBits = " << serialport.dataBits() << " bit";
        qDebug() << " - flowControl = " << serialport.flowControl();
        qDebug() << " - parity = " << serialport.parity();
        qDebug() << " - stopBits = " << serialport.stopBits();
        qDebug() << " - readBufferSize = " << serialport.readBufferSize() << " bytes";
        qDebug("-----------------\n");
        return _result;

    } else {
        qWarning("Не получается открыть указанный порт!");
        return false;
    }
}

qint64 QSPProcessor::writeToPort(const QByteArray &_data)
{
    return __writeData(_data);
}

void QSPProcessor::closePort()
{
    if(serialport.isOpen()) {
        serialport.close();
    }
}

void QSPProcessor::__showError(QSerialPort::SerialPortError _errorid)
{
    switch(_errorid) {
        case QSerialPort::NoError:
            // Do nothing
            break;
        case QSerialPort::DeviceNotFoundError:
            qWarning("QSerialPort: An error occurred while attempting to open an non-existing device");
            break;
        case QSerialPort::PermissionError:
            qWarning("QSerialPort: An error occurred while attempting to open an already opened device by another process or a user not having enough permission and credentials to open.");
            break;
        case QSerialPort::OpenError:
            qWarning("QSerialPort: An error occurred while attempting to open an already opened device in this object.");
            break;
        case QSerialPort::NotOpenError:
            qWarning("QSerialPort: This error occurs when an operation is executed that can only be successfully performed if the device is open.");
            break;
        case QSerialPort::ParityError:
            qWarning("QSerialPort: Parity error detected by the hardware while reading data. This value is obsolete. We strongly advise against using it in new code.");
            break;
        case QSerialPort::FramingError:
            qWarning("QSerialPort: Framing error detected by the hardware while reading data. This value is obsolete. We strongly advise against using it in new code.");
            break;
        case QSerialPort::BreakConditionError:
            qWarning("QSerialPort: Break condition detected by the hardware on the input line. This value is obsolete. We strongly advise against using it in new code.");
            break;
        case QSerialPort::WriteError:
            qWarning("QSerialPort: An I/O error occurred while writing the data.");
            break;
        case QSerialPort::ReadError:
            qWarning("QSerialPort: An I/O error occurred while reading the data.");
            break;
        case QSerialPort::ResourceError:
            qWarning("QSerialPort: An I/O error occurred when a resource becomes unavailable, e.g. when the device is unexpectedly removed from the system.");
            break;
        case QSerialPort::UnsupportedOperationError:
            qWarning("QSerialPort: The requested device operation is not supported or prohibited by the running operating system.");
            break;
        case QSerialPort::TimeoutError:
            qWarning("QSerialPort: A timeout error occurred.");
            break;
        case QSerialPort::UnknownError:
            qWarning("QSerialPort: An unidentified error occurred. ;)");
            break;
    }
}

void QSPProcessor::__readData()
{
    // Read incoming data
    QByteArray repeat = serialport.readAll();
    if(repeat.contains('\n')) {
        QString str(repeat);
        repeatbuffer.append(str.section('\n',0,0).toUtf8());
        qInfo("MCU repeats: %s", repeatbuffer.constData());
        repeatbuffer.clear();
        repeatbuffer.append(str.section('\n',1,1).toUtf8());
    } else {
        repeatbuffer.append(repeat);
    }
}

qint64 QSPProcessor::__writeData(const QByteArray &_data)
{
    if(serialport.isOpen()) {
        qDebug("Write to serial port");
        return serialport.write(_data);
    }
    qWarning("Serial port is closed!");
    return -2;
}
