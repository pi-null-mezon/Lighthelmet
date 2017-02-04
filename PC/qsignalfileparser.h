#ifndef QSIGNALFILEPARSER_H
#define QSIGNALFILEPARSER_H

#include <QObject>
#include <QByteArray>
#include <QFileInfo>
#include <QFile>


class QSignalFileParser : public QObject
{
    Q_OBJECT
public:
    explicit QSignalFileParser(QObject *parent = 0);

    static QByteArray parseFile(const QString &_filename);
};

#endif // QSIGNALFILEPARSER_H
