#ifndef QSIGNALFILEPARSER_H
#define QSIGNALFILEPARSER_H

#include <QObject>
#include <QByteArray>
#include <QFileInfo>
#include <QFile>

#ifdef Q_OS_LINUX
typedef u_int8_t uint8_t;
#endif

class QSignalFileParser : public QObject
{
    Q_OBJECT
public:
    explicit QSignalFileParser(QObject *parent = 0);

    static std::vector<QByteArray> parseSignalFromFile(const QString &_filename);
};

#endif // QSIGNALFILEPARSER_H
