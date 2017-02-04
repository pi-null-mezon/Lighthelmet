#include "qsignalfileparser.h"

QSignalFileParser::QSignalFileParser(QObject *parent) : QObject(parent)
{

}

QByteArray QSignalFileParser::parseFile(const QString &_filename)
{
    QFileInfo _finfo(_filename);
    if(_finfo.exists() == false) {
        qWarning("Файл %s не существует!", _filename.toLocal8Bit().constData());
        return QByteArray();
    }

    QFile _file(_filename);
    if(_file.open(QFile::ReadOnly)) {
        qInfo("Файл %s был открыт для чтения", _filename.toLocal8Bit().constData());
        QByteArray _bytemessage;
        _bytemessage.reserve(1024);
        QByteArray _line = _file.readLine();
        uint8_t _value;
        bool _ok;
        while(_line.size() > 0) {
            _value = (uint8_t)QString(_line.simplified()).toInt(&_ok);
            if(!_ok) { // we have found symbol
                _bytemessage.push_back(_line.at(0));
            } else { // we have found symbols sequence that represents a number
                _bytemessage.push_back(_value);
            }
            _line = _file.readLine();
        }
        return _bytemessage;
    } else {
        qWarning("Не получается открыть файл %s", _filename.toLocal8Bit().constData());
        return QByteArray();
    }

}
