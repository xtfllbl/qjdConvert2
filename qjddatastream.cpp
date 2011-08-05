
#include "qjddatastream.h"
#include <QDebug>
QJDDataStream::QJDDataStream()
        : QDataStream(), df(QJDDataStream::IEEE)
{setVersion(QDataStream::Qt_4_5);}  //避免一些问题

QJDDataStream::QJDDataStream( QByteArray a )
        : QDataStream( a), df(QJDDataStream::IEEE)
{}

QJDDataStream::QJDDataStream(QIODevice* io)
        : QDataStream(io),df(QJDDataStream::IEEE)
{}

QJDDataStream::~QJDDataStream()
{}

void QJDDataStream::setFormat(DataFormat f)
{
    df=f;
}

QJDDataStream &QJDDataStream::operator <<(float f)
{
    if (QJDDataStream::IBM==df)
    {
        float ft=f;
        void *v=&ft;
        int *i=(int*)v;

        //conversion. IEEE to IBM
        if (*i)
        {
            int fmant=(0x007fffff & *i) | 0x00800000;
            //ex:exponent.(ex/2^23)-127+1.(IEEE throw away the most left 1.so plus 1)
            int ex= (int) ((0x7f800000 & *i) >> 23) - 126;
            //change the last two bits of ex into zero.(make ex%4==0)
            while (ex & 0x3)
            {
                //exponent plus 1,mantissa divide 2
                ++ex;
                fmant>>=1;
            }
            *i=(0x80000000 & *i) | (((ex>>2) + 64) << 24) | fmant;
        }
        //if byte order is big endian.change it's order
        if(this->byteOrder()==QDataStream::BigEndian)
            //if(this->byteOrder()==QDataStream::LittleEndian)
            *i = (*i<<24) | ((*i>>24) & 0xff) |
                 ((*i & 0xff00)<<8) | ((*i & 0xff0000)>>8);

        this->writeRawData((char*)v,sizeof(float));

    }
    else
    {
        *dynamic_cast<QDataStream*>(this)<<f;
    }

    return *this;
}

QJDDataStream &QJDDataStream::operator >>(float &f)
{

    if (QJDDataStream::IBM==df)
    {
        int len=sizeof(float);

        QByteArray ba;
        ba.resize(len);
        this->readRawData(ba.data(),len);
        void* v=ba.data();

        int *i=(int*)v;

        //conversion. IBM to IEEE
        //if it is big endian,change it's byte order.
        if (this->byteOrder()==QDataStream::BigEndian)
            //if (this->byteOrder()==QDataStream::LittleEndian)
        {
            *i = (*i<<24) | ((*i>>24)&0xff) |
                 ((*i & 0xff00)<<8) | ((*i & 0xff0000)>>8);
           /* *i=(*i<<24) | ((*i>>24)&0xff) |
            ((*i & 0x0000ff00) <<8) | ((*i & 0x00ff0000) >>8) ;*/
        }

        if (*i)	//*i!=0
        {
            //fmant:mantissa
            int fmant=0x00ffffff & *i;
            //if fmant ==0
            if (!fmant)
            {
                qWarning("it is not a IBM float format data");
                return *this;
            }

            //ex:exponent.(ex>>24-64)*4+127-1=ex>>22-130
            int ex=(int)((0x7f000000 & *i) >> 22) -130;

            //move bits.change the first bit of fmant into 1.
            while (!(fmant & 0x00800000))
            {
                --ex;
                fmant<<=1;
            }
            //overflow(the max of IEEE float format exponent is 127.127+127=254)
            if (ex>254)
            {
                *i=(0x80000000 & *i) | 0x7f7fffff;
            }
            //overflow(the min of IEEE float format exponent is -127.)
            else if (ex<=0)
            {
                *i=0;
            }
            else
            {
                // throw away the first 1 of fmant and combine the 3 parts
                *i = (0x80000000 & *i) |(ex << 23)|(0x007fffff & fmant);
            }
        }

        float *t=(float*)v;
        f=*t;

    }
    else
    {
        *dynamic_cast<QDataStream*>(this)>>f;
    }
    return *this;
}

QJDDataStream & QJDDataStream::operator>> ( qint8 & i )
{
    *dynamic_cast<QDataStream*>(this)>>i;
    return *this;
}
QJDDataStream & QJDDataStream::operator>> (quint8 & i )
{
    *dynamic_cast<QDataStream*>(this)>>i;
    return *this;
}
QJDDataStream & QJDDataStream::operator>> ( qint16 & i )
{
    *dynamic_cast<QDataStream*>(this)>>i;
    return *this;
}
QJDDataStream & QJDDataStream::operator>> (quint16 & i )
{
    *dynamic_cast<QDataStream*>(this)>>i;
    return *this;
}
QJDDataStream & QJDDataStream::operator>> ( qint32 & i )
{
    *dynamic_cast<QDataStream*>(this)>>i;
    return *this;
}
QJDDataStream & QJDDataStream::operator>> ( quint32 & i )
{
    *dynamic_cast<QDataStream*>(this)>>i;
    return *this;
}
QJDDataStream & QJDDataStream::operator>> ( qint64 & i )
{
    *dynamic_cast<QDataStream*>(this)>>i;
    return *this;
}
QJDDataStream & QJDDataStream::operator>> ( quint64 & i )
{
    *dynamic_cast<QDataStream*>(this)>>i;
    return *this;
}

QJDDataStream & QJDDataStream::operator>> ( char *& s )
{
    *dynamic_cast<QDataStream*>(this)>>s;
    return *this;
}

QJDDataStream & QJDDataStream::operator<< ( qint8  i )
{
    *dynamic_cast<QDataStream*>(this)<<i;
    return *this;
}
QJDDataStream & QJDDataStream::operator<< ( quint8 i )
{
    *dynamic_cast<QDataStream*>(this)<<i;
    return *this;
}
QJDDataStream & QJDDataStream::operator<< ( qint16 i )
{
    *dynamic_cast<QDataStream*>(this)<<i;
    return *this;
}
QJDDataStream & QJDDataStream::operator<< ( quint16 i )
{
    *dynamic_cast<QDataStream*>(this)<<i;
    return *this;
}
QJDDataStream & QJDDataStream::operator<< ( qint32 i )
{
    *dynamic_cast<QDataStream*>(this)<<i;
    return *this;
}
QJDDataStream & QJDDataStream::operator<< ( quint32 i )
{
    *dynamic_cast<QDataStream*>(this)<<i;
    return *this;
}
QJDDataStream & QJDDataStream::operator<< ( qint64 i )
{
    *dynamic_cast<QDataStream*>(this)<<i;
    return *this;
}
QJDDataStream & QJDDataStream::operator<< ( quint64 i )
{
    *dynamic_cast<QDataStream*>(this)<<i;
    return *this;
}

QJDDataStream & QJDDataStream::operator<< ( const char *s )
{
    *dynamic_cast<QDataStream*>(this)<<s;
    return *this;
}

QJDDataStream & QJDDataStream::readBytes ( char *& s, uint & l )
{
    QDataStream::readBytes(s,l);
    return *this;
}
QJDDataStream & QJDDataStream::readRawData ( char * s, uint len )
{
    QDataStream::readRawData(s,len);

    return *this;
}
QJDDataStream & QJDDataStream::writeBytes ( const char * s, uint len )
{
    QDataStream::writeBytes(s,len);
    return *this;
}
QJDDataStream & QJDDataStream::writeRawData( const char * s, uint len )
{
    QDataStream::writeRawData(s,len);
    return *this;
}
