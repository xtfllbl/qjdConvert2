#ifndef QJDSEGYTH_H
#define QJDSEGYTH_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QVector>


class QJDSegyTH
{
public:
    QJDSegyTH();
    ~QJDSegyTH();
    QString getTextLine(int ln);
    bool readText(const QString& fn,QString& buf);
    bool writeText(const QString& fn,const QString& content);

private:
    QString Ebcdic2Ascii(const QString &e);
    QByteArray Ascii2Ebcdic(const QString &a);
    QVector<QString> vStr;
    QString errStr;
};

#endif //QJDSEGYTH_H
