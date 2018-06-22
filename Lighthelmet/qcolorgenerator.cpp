#include "qcolorgenerator.h"

#include <QColor>

QColorGenerator::QColorGenerator(QObject *parent) : QObject(parent)
{
    timer.setTimerType(Qt::PreciseTimer);
    connect(&timer,SIGNAL(timeout()),this,SLOT(__generateColor()));
}

void QColorGenerator::setParams(const std::vector<QByteArray> &_params)
{
    if(_params.size() == 6) {
        timeintervalus = QString(_params[0]).section('t',1).toInt();
        channels = QString(_params[1]).section('c',1).toInt();
        r = (uint8_t)(QString(_params[2]).section('r',1).toInt());
        g = (uint8_t)(QString(_params[3]).section('g',1).toInt());
        b =  (uint8_t)(QString(_params[4]).section('b',1).toInt());
        signal = QByteArray(_params[5].data()+1,_params[5].size()-1);
        /*qDebug("QColorGenerator::setParams::Info - parsed values:\n"
              "timeintervalus == %d\n"
              "channels == %d\n"
              "r == %u\n"
              "g == %u\n"
              "b == %u\n"
              "s == %s",
              timeintervalus,
              channels,
              (uint)r,(uint)g,(uint)b,signal.constData());
        for(int i = 0; i < signal.size(); ++i) {
            qDebug("%d) %d",i,static_cast<int>((uint16_t)signal[i] & 0x00FF));
        }*/
    } else {
        qWarning("QColorGenerator::setParams::Warning - unsupported input format!");
    }
}

void QColorGenerator::start()
{
    timer.start(timeintervalus/1000.0);
}

void QColorGenerator::stop()
{
    timer.stop();
}

void QColorGenerator::__generateColor()
{
    if(signal.size() > 0) {
        if(channels == 1) {
            static int _pos = 0;
            QColor _color;
            int _val = (int16_t)signal[_pos] & 0x00FF;
            _color.setRed(r > 0 ? _val : 0);
            _color.setGreen(g > 0 ? _val : 0);
            _color.setBlue(b > 0 ? _val : 0);
            _pos = (_pos + 1) % signal.size();
            emit colorGenerated(_color);
        } else if(channels == 3) {
            static int _pos = 0;
            const int _shift = signal.size()/3;
            QColor _color;
            _color.setRed((int16_t)signal[_pos] & 0x00FF);
            _color.setGreen((int16_t)signal[_pos+_shift] & 0x00FF);
            _color.setBlue((int16_t)signal[_pos+2*_shift] & 0x00FF);
            _pos = (_pos + 1) % _shift;
            emit colorGenerated(_color);
        }
    }
}
