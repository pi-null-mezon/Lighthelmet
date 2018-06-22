#include "qcolorshowwidget.h"

#include <QPainter>
#include <QRect>

QColorshowWidget::QColorshowWidget(QWidget *parent) : QWidget(parent)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
}

void QColorshowWidget::paintEvent(QPaintEvent *_event)
{
    Q_UNUSED(_event);
    QPainter _painter(this);
    switch(mode) {
        case Wait:
            _painter.setRenderHint(QPainter::Antialiasing);
            __rollCircles(_painter,rect());
            break;
        case Show:
            __fillRect(_painter,rect());
            break;
        default:
            // DO NOTHING
            break;
    }
}

void QColorshowWidget::updateColor(const QColor &_color)
{
    color = _color;
    repaint(); // not update()! because it should be called immediately
}

void QColorshowWidget::enableShowMode()
{
    setMode(Show);
}

void QColorshowWidget::__rollCircles(QPainter &_painter, const QRect &_rect)
{
    uint numofcircles = 11;
    qreal _radius = std::sqrt(_rect.height()*_rect.height() + _rect.width()*_rect.width()) / 9.0f;
    qreal _circleradius = _radius / 5.0f;
    qreal _anglestep = 6.28 / numofcircles;
    static qreal _phaseshift = 0.0, _phase = 0.0;
    _painter.translate(_rect.center());
    _painter.setPen(Qt::NoPen);
    _painter.setBrush(QColor(255,255,255,100));
    for(uint i = 0; i < numofcircles; ++i) {
        QPointF _c(_radius*std::cos(_anglestep*i + _phaseshift), _radius*std::sin(_anglestep*i + _phaseshift));
        _painter.drawEllipse(_c,_circleradius,_circleradius);
    }
    _phase += 0.1;
    _phaseshift += 0.05*(1 - cos(_phase));
}

void QColorshowWidget::__fillRect(QPainter &_painter, const QRect &_rect)
{
    _painter.fillRect(_rect,color);
}

void QColorshowWidget::setMode(const ShowMode &value)
{
    if(mode != value) {
        mode = value;
        emit modeChanged(mode);
        switch(mode) {
            case Wait:
                timer.start(15);
                break;
            default:
                timer.stop();
                break;
        }
    }
}
