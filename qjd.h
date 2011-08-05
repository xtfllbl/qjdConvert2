#ifndef QJD_H
#define QJD_H

#include <math.h>

#include <Qt>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QVector>
#define NSTACK 50
#define NSMALL 7
#define FM 7875         /* constants used to generate random pivots */
#define FA 211
#define FC 1663

class QJD
{
public:
    static const quint64 CDSIZE = 650000000;
    static const quint64 NFSIZE = 2147483647;
    //static const quint64 MFSIZE = 4294967295;
    static const float PRECISION = 1e-7;
    //static const quint64 CDSIZE;
    //static const quint64 NFSIZE;
    //static const quint64 MFSIZE;
    //static const float PRECISION;

public:
    static uint qjdFileSize( quint64 total, int type );
    static void qjdReadBytes( QFile *, quint64 index, char*s, uint length);
    static void qjdWriteBytes( QFile *, quint64 index, const char*s, uint length);
    static float qjdNearValue( float a, float b );
    
    static void qksort(int n,QVector<float> a);
    static void qkpart (QVector<float> a, int p, int q, int *j, int *k);
    static void qkinss(QVector<float> a, int p, int q);

    static QString qjdFileReName( const QString& oldf, const QString& olds, const QString& news );
    static QString qjdStackName( const QString& f, bool x = TRUE );

    static QStringList openVolFile(QString name);//get two direct file(x or y)from index file


    static	void qjdibm2ieee(int *from, int *to, int n, int endian);
    static	void qjdieee2ibm(int *from, int *to, int n, int endian);
    static	void qjdswap_float_4(float *f);
    static	void qjdswap_double_8(double *d);
    static	void qjdswap_int_4(qint32 *i);
    static	void qjdswap_u_int_4(quint32 *i);
    static	void qjdswap_short_2(qint16 *s);
    static	void qjdswap_u_short_2(quint16 *us);
    static	void qjdswap_long_4(long *l);
    static	void qjdswap_u_long_4(unsigned long *l);
public:
    static QJD* instance();
    
protected:
    QJD();
    ~QJD();

private:
    static QJD* qjd;
};

#endif //QJD_H

