#include "qsignalfileparser.h"

QSignalFileParser::QSignalFileParser(QObject *parent) : QObject(parent)
{

}

std::vector<QByteArray> QSignalFileParser::parseSignalFromFile(const QString &_filename)
{
    std::vector<QByteArray> _vdatamsgs;

    QFileInfo _finfo(_filename);
    if(_finfo.exists() == false) {
        qWarning("Файл %s не существует!", _filename.toUtf8().constData());
        return _vdatamsgs;
    }

    QFile _file(_filename);
    if(_file.open(QFile::ReadOnly)) {
        qInfo("Файл %s был открыт для чтения", _filename.toUtf8().constData());
        QByteArray _bytemessage;
        _bytemessage.reserve(1024);
        QByteArray _line = _file.readLine();
        int _value;
        bool _ok;
        char symbol;
        while(_line.size() > 0) {
            _value = QString(_line.simplified()).toInt(&_ok);
            if(!_ok) { // we have found symbol
                symbol = _line.at(0);
                if(_bytemessage.size() > 0) {
                    _vdatamsgs.push_back(_bytemessage);
                    _bytemessage.clear();
                }
                _bytemessage.push_back(_line.at(0));
            } else { // we have found symbols sequence that represents a number
                if(symbol == 's') {
                    _bytemessage.push_back((uint8_t)_value);
                } else { // if symbol == 't'
                    _bytemessage.append(_line);
                }
            }            
            _line = _file.readLine();
        }
        if(_bytemessage.size() > 0) {
            _vdatamsgs.push_back(_bytemessage);
        }
        return _vdatamsgs;
    } else {        
        qWarning("Не получается открыть файл %s", _filename.toUtf8().constData());
        return _vdatamsgs;
    }   
}
