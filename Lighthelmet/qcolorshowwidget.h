#ifndef QCOLORSHOWWIDGET_H
#define QCOLORSHOWWIDGET_H

#include <QWidget>
#include <QTimer>

class QColorshowWidget : public QWidget
{
    Q_OBJECT
public:
    enum ShowMode {Void, Wait, Show};
    explicit QColorshowWidget(QWidget *parent = nullptr);

    void setMode(const ShowMode &value);

signals:
    void modeChanged(ShowMode _mode);

protected:
    void paintEvent(QPaintEvent *_event);

public slots:
    void updateColor(const QColor &_color);
    void enableShowMode();

private:
    void __rollCircles(QPainter &_painter, const QRect &_rect);
    void __fillRect(QPainter &_painter, const QRect &_rect);

    ShowMode mode;

    QTimer timer;
    QColor color;
};

#endif // QCOLORSHOWWIDGET_H
