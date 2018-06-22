#ifndef QCOLORGENERATOR_H
#define QCOLORGENERATOR_H

#include <QObject>
#include <QTimer>

class QColorGenerator : public QObject
{
    Q_OBJECT
public:
    explicit QColorGenerator(QObject *parent = nullptr);

signals:
    void colorGenerated(const QColor &_color);

public slots:
    void setParams(const std::vector<QByteArray> &_params);
    void start();
    void stop();

private slots:
    void __generateColor();

private:
    int timeintervalus;
    int channels;
    uint8_t r,g,b;
    QByteArray signal;
    QTimer timer;
};

#endif // QCOLORGENERATOR_H
