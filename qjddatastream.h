
#ifndef QJDDATAJDREAM_H
#define QJDDATAJDREAM_H

#include <QDataStream>
#include <QString>

/*
   ClassName:QJDDataStream

   Description:convert format between IBM & IEEE 4 or 8 bytes float. 
*/
class QJDDataStream : public QDataStream
{
public:
    QJDDataStream();
    QJDDataStream( QByteArray a );
    QJDDataStream(QIODevice *);
    ~QJDDataStream();

    enum DataFormat { IEEE, IBM };

    void setFormat(DataFormat);

    QJDDataStream& operator<<(float f);

    QJDDataStream& operator>>(float &f);


    //operators from QDataStream
    QJDDataStream & operator>> ( qint8 & i );
    QJDDataStream & operator>> ( quint8 & i );
    QJDDataStream & operator>> ( qint16 & i );
    QJDDataStream & operator>> ( quint16 & i );
    QJDDataStream & operator>> ( qint32 & i );
    QJDDataStream & operator>> ( quint32 & i );
    QJDDataStream & operator>> ( qint64 & i );
    QJDDataStream & operator>> ( quint64 & i );
    QJDDataStream & operator>> ( char *& s );

    QJDDataStream & operator<< ( qint8 i );
    QJDDataStream & operator<< ( quint8 i );
    QJDDataStream & operator<< ( qint16 i );
    QJDDataStream & operator<< ( quint16 i );
    QJDDataStream & operator<< ( qint32 i );
    QJDDataStream & operator<< ( quint32 i );
    QJDDataStream & operator<< ( qint64 i );
    QJDDataStream & operator<< ( quint64 i );
    QJDDataStream & operator<< ( const char * s );

    QJDDataStream & readBytes ( char *& s, uint & l );
    QJDDataStream & readRawData ( char * s, uint len );
    QJDDataStream & writeBytes ( const char * s, uint len );
    QJDDataStream & writeRawData ( const char * s, uint len );
private:
    DataFormat df;
};

#endif //QJDDATASTREAM_H
