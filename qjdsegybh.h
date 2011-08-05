#ifndef QJDSEGYBH_H
#define QJDSEGYBH_H

#include <QString>
#include <QTextStream>
#include <QFile>

#include "qjddatastream.h"

class QJDSegyBH
{
public:
    QJDSegyBH(void);
    ~QJDSegyBH(void);

    void init();
    void swap_header();

    bool readData(const QString& fn,QJDDataStream::ByteOrder bo=QJDDataStream::BigEndian,QJDDataStream::DataFormat df=QJDDataStream::IBM);
    bool writeData(const QString& fn,QJDDataStream::ByteOrder bo=QJDDataStream::BigEndian,QJDDataStream::DataFormat df=QJDDataStream::IBM);

    //QJDDataStream::ByteOrder byteOrder;
    //QJDDataStream::DataFormat dataFormat;

    qint32 jobid;      /* job identification number */

    qint32 lino;       /* line number (only one line per reel) */

    qint32 reno;       /* reel number */

    qint16 ntrpr;    /* number of data traces per record */

    qint16 nart;     /* number of auxiliary traces per record */

    quint16 hdt; /* sample interval in micro secs for this reel */

    quint16 dto; /* same for original field recording */

    quint16 hns; /* number of samples per trace for this reel */

    quint16 nso; /* same for original field recording */

    qint16 format;   /* data sample format code:
                                    1 = floating point (4 bytes)
                                2 = fixed point (4 bytes)
                                3 = fixed point (2 bytes)
                                4 = fixed point w/gain code (4 bytes) */

    qint16 fold;     /* CDP fold expected per CDP ensemble */

    qint16 tsort;    /* trace sorting code:
                                1 = as recorded (no sorting)
                                2 = CDP ensemble
                                3 = single fold continuous profile
                                4 = horizontally stacked */

    qint16 vscode;   /* vertical sum code:
                                1 = no sum
                                2 = two sum ...
                                N = N sum (N = 32,767) */

    qint16 hsfs;     /* sweep frequency at start */

    qint16 hsfe;     /* sweep frequency at end */

    qint16 hslen;    /* sweep length (ms) */

    qint16 hstyp;    /* sweep type code:
                                1 = linear
                                2 = parabolic
                                    3 = exponential
                                4 = other */

    qint16 schn;     /* trace number of sweep channel */

    qint16 hstas;    /* sweep trace taper length at start if
                           tapered (the taper starts at zero time
                           and is effective for this length) */

    qint16 hstae;    /* sweep trace taper length at end (the ending
                           taper starts at sweep length minus the taper
                           length at end) */

    qint16 htatyp;   /* sweep trace taper type code:
                                1 = linear
                                2 = cos-squared
                                3 = other */

    qint16 hcorr;    /* correlated data traces code:
                                1 = no
                                2 = yes */

    qint16 bgrcv;    /* binary gain recovered code:
                                1 = yes
                                2 = no */

    qint16 rcvm;     /* amplitude recovery method code:
                                1 = none
                                2 = spherical divergence
                                3 = AGC
                                4 = other */

    qint16 mfeet;    /* measurement system code:
                                1 = meters
                                2 = feet */

    qint16 polyt;    /* impulse signal polarity code:
                                1 = increase in pressure or upward
                                    geophone case movement gives
                                    negative number on tape
                                2 = increase in pressure or upward
                                    geophone case movement gives
                                    positive number on tape */

    qint16 vpol;     /* vibratory polarity code:
                                code    seismic signal lags pilot by
                                1       337.5 to  22.5 degrees
                                2        22.5 to  67.5 degrees
                                3        67.5 to 112.5 degrees
                                4       112.5 to 157.5 degrees
                                5       157.5 to 202.5 degrees
                                6       202.5 to 247.5 degrees
                                7       247.5 to 292.5 degrees
                                8       293.5 to 337.5 degrees */

    qint16 hunass[170];      /* unassigned 2*170=340 bit*/


    friend QJDDataStream & operator<< ( QJDDataStream & ds, const QJDSegyBH &s);
    friend QJDDataStream & operator>> ( QJDDataStream & ds, QJDSegyBH &s);
    //	friend QTextStream  & operator<< ( QTextStream & ts, const QJDSegyBH &s);


};




#endif //QJDSEGYBH_H
