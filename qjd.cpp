/*
 * qjd.cpp
 *
 *  Created on: 2008-11-24
 *      Author: zyq
 */

#include "qjd.h"
#include <QTextStream>
#include <QDebug>

//#define CDSIZE 650000000
//#define NFSIZE 2147483647
//#define MFSIZE 4294967295

//#define MINSIZE 10737418240
//#define NORMALSIZE 64424509440
//#define MAXSIZE 107374182400

QJD* QJD::qjd = NULL;
//Q_UINT64 Qst::CDSIZE = 650000000;
//Q_UINT64 Qst::NFSIZE = 2147483647;
//Q_UINT64 Qst::MFSIZE = 4294967295;
//float Qst::PRECISION = 1e-7;

QJD::QJD() {
}

QJD::~QJD() {
}

QJD*
        QJD::instance() {
    if (qjd == NULL) {
        qjd = new QJD();
    }

    return qjd;
}

uint QJD::qjdFileSize(quint64 total, int type) {
    qDebug() << "qjdFileSize: "<< total;

    switch (type) {
    case 1:
        return CDSIZE;
        break;
    case 2:
        return NFSIZE;
        break;
    case 3:
        return NFSIZE;
        //return MFSIZE;
        break;
    default:
        return NFSIZE;
    }
}

void QJD::qjdReadBytes(QFile *f, quint64 index, char* s, uint length) {
    if (f != NULL) {
        QDataStream ds;
        ds.setDevice(f);

        f->seek(index);
        ds.readRawData(s, length);
        ds.unsetDevice();
    } else {
        qWarning("qjdReadBytes: The file pointer is NULL!");
    }
}

void QJD::qjdWriteBytes(QFile *f, quint64 index, const char* s, uint length) {
    if (f != NULL) {
        QDataStream ds;
        ds.setDevice(f);

        f->seek(index);
        ds.writeRawData(s, length);

        ds.unsetDevice();
    } else {
        qWarning("qjdWriteBytes: The file pointer is NULL!");
    }
}

float QJD::qjdNearValue(float a, float b) {
    int i;
    float t1, t2;
    if (b == 0) {
        qWarning("QJD::qjdNearValue: b is 0!");
        return 0.0;
    }
    if (a != b) {
        t1 = a / b;
        t2 = (int) (t1 + PRECISION);
        t2 = (float) t2;
        if (t1 > 0) {
            if (a > 0) {
                if (t1 - t2 >= PRECISION) {
                    i = (int) (t1);
                    if ((a - i * b) <= 0.5 * b) {
                        a = i * b;
                    } else
                        a = (++i) * b;
                }
            } else {
                if (t1 - t2 >= PRECISION) {
                    i = (int) (t1);
                    if ((fabs(a) - i * fabs(b) <= fabs(0.5 * b))) {
                        a = i * b;
                    } else
                        a = (++i) * b;
                }
            }
        } else {
            if (a < 0) {
                if (fabs(t1) - fabs(t2) >= PRECISION) {
                    i = (int) fabs(t1);
                    if ((fabs(a) - i * b) <= 0.5 * b) {
                        a = -i * b;
                    } else
                        a = -(++i) * b;
                }
            } else {
                if (fabs(t1) - fabs(t2) >= PRECISION) {
                    i = (int) fabs(t1);
                    if (a - i * fabs(b) <= 0.5 * fabs(b)) {
                        a = -i * b;
                    } else
                        a = -(++i) * b;
                }
            }
        }
    }

    return a;
}

void QJD::qkinss(QVector<float> a, int p, int q) {
    int i, j;
    float ai;

    for (i = p + 1; i <= q; i++) {
        for (ai = a[i], j = i; j > p && a[j - 1] > ai; j--)
            a[j] = a[j - 1];
        a[j] = ai;
    }

}
void QJD::qkpart(QVector<float> a, int p, int q, int *j, int *k) {
    int pivot, left, right;
    float apivot, temp;
    static long int seed = 0L;

    /* choose random pivot element between p and q, inclusive */
    seed = (seed * FA + FC) % FM;
    pivot = p + (q - p) * (float) seed / (float) FM;
    if (pivot < p)
        pivot = p;
    if (pivot > q)
        pivot = q;
    apivot = a[pivot];

    /* initialize left and right pointers and loop until break */
    for (left = p, right = q;;) {
        /*
		 * increment left pointer until either
		 * (1) an element greater than the pivot element is found, or
		 * (2) the upper bound of the input subarray is reached
		 */
        while (a[left] <= apivot && left < q)
            left++;

        /*
		 * decrement right pointer until either
		 * (1) an element less than the pivot element is found, or
		 * (2) the lower bound of the input subarray is reached
		 */
        while (a[right] >= apivot && right > p)
            right--;

        /* if left pointer is still to the left of right pointer */
        if (left < right) {
            /* exchange left and right elements */
            temp = a[left];
            a[left++] = a[right];
            a[right--] = temp;
        }
        /* else, if pointers are equal or have crossed, break */
        else
            break;
    }
    /* if left pointer has not crossed pivot */
    if (left < pivot) {

        /* exchange elements at left and pivot */
        temp = a[left];
        a[left++] = a[pivot];
        a[pivot] = temp;
    }
    /* else, if right pointer has not crossed pivot */
    else if (pivot < right) {

        /* exchange elements at pivot and right */
        temp = a[right];
        a[right--] = a[pivot];
        a[pivot] = temp;
    }
    /* left and right pointers have now crossed; set output bounds */
    *j = right;
    *k = left;
}

void QJD::qksort(int n, QVector<float> a) {
    int pstack[NSTACK], qstack[NSTACK], j, k, p, q, top = 0;

    /* initialize subarray lower and upper bounds to entire array */
    pstack[top] = 0;
    qstack[top++] = n - 1;

    /* while subarrays remain to be sorted */
    while (top != 0) {

        /* get a subarray off the stack */
        p = pstack[--top];
        q = qstack[top];

        /* while subarray can be partitioned efficiently */
        while (q - p > NSMALL) {

            /* partition subarray into two subarrays */
            qkpart(a, p, q, &j, &k);

            /* save larger of the two subarrays on stack */
            if (j - p < q - k) {
                pstack[top] = k;
                qstack[top++] = q;
                q = j;
            } else {
                pstack[top] = p;
                qstack[top++] = j;
                p = k;
            }
        }
        /* use insertion sort to finish sorting small subarray */
        qkinss(a, p, q);
    }

}

QStringList QJD::openVolFile(QString name) {
    //qDebug(QString("Qst::openVolFile:name:%1").arg(name));
    QString fn = name;
    QStringList strList;
    //strList.clear();
    //qDebug(QString("Qst::openVolFile:count1:%1").arg(strList.count()));
    QFile file(fn);
    if (!file.open(QIODevice::ReadOnly)) {
        //qDebug(QString("Qst::openVolFile:"));
        file.close();
        return strList;
    }
    QTextStream stream(&file);
    QString fileStr;
    fileStr = stream.readLine();
    while (fileStr.simplified().isEmpty() && !stream.atEnd())
        //multi enter
        fileStr = stream.readLine();
    if (fileStr.simplified().isEmpty()) {
        file.close();
        return strList;
    }
    QString namex = fileStr.simplified();//���ļ·
    strList.append(namex);
    //qDebug(QString("Qst::openVolFile:count2:%1").arg(strList.count()));
    //qDebug(QString("QstLayerSet::openVolFile:sVolFileNamex:%1").arg(sVolFileNameX));
    fileStr = stream.readLine();
    while (fileStr.simplified().isEmpty() && !stream.atEnd())
        //multi enter
        fileStr = stream.readLine();
    if (fileStr.simplified().isEmpty()) {
        file.close();
        return strList;
    }
    QString namey = fileStr.simplified();
    strList.append(namey);
    //qDebug(QString("QstLayerSet::openVolFile:sVolFileNamey:%1").arg(sVolFileNameY));
    file.close();
    return strList;

}

QString QJD::qjdFileReName(const QString& oldf, const QString& olds,
                           const QString& news) {
    qDebug()<<"QJD::qjdFileReName: source "<< oldf;
    QString result;
    int len;
    int r1, r2;
    len = oldf.length();
    r1 = oldf.indexOf(olds);
    r2 = oldf.indexOf(".");
    if (r1 == -1) {
        if (r2 == -1) {
            result = oldf + "." + news;
        } else {
            result = oldf.left(r2) + "_" + news + oldf.right(len - r2);
        }
    } else {
        result = oldf;
        result.replace(olds, news);
    }

    qDebug()<<"QJD::qjdFileReName: result "<<result;

    return result;
}

QString QJD::qjdStackName(const QString& f, bool x) {
    qDebug()<<"QJD::qjdStackName: source "<<f;
    QString result;
    int len;
    int r;
    len = f.length();
    r = f.indexOf(".");
    if (r == -1) {
        if (x) {
            result = f + "-x";
        } else {
            result = f + "-y";
        }
    } else {
        if (x) {
            result = f.left(r) + "-x" + f.right(len - r);
        } else {
            result = f.left(r) + "-y" + f.right(len - r);
        }
    }

    qDebug()<<"QJD::qjdStackName: result "<< result;

    return result;
}

void QJD::qjdswap_short_2(qint16 *tni2)
        /**************************************************************************
swap_short_2            swap a short integer
***************************************************************************/
{
    *tni2=(((*tni2>>8)&0xff) | ((*tni2&0xff)<<8));
}

void QJD::qjdswap_u_short_2(quint16 *tni2)
        /**************************************************************************
swap_u_short_2          swap an unsigned short integer
***************************************************************************/
{
    *tni2=(((*tni2>>8)&0xff) | ((*tni2&0xff)<<8));
}

void QJD::qjdswap_int_4(qint32 *tni4)
        /**************************************************************************
swap_int_4              swap a 4 byte integer
***************************************************************************/
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}

void QJD::qjdswap_u_int_4(quint32 *tni4)
        /**************************************************************************
swap_u_int_4            swap an unsigned integer
***************************************************************************/
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}

void QJD::qjdswap_long_4(long *tni4)
        /**************************************************************************
swap_long_4             swap a long integer
***************************************************************************/
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}
void QJD::qjdswap_u_long_4(unsigned long *tni4)
        /**************************************************************************
swap_u_long_4           swap an unsigned long integer
***************************************************************************/
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}

void QJD::qjdswap_float_4(float *tnf4)
        /**************************************************************************
swap_float_4            swap a float
***************************************************************************/
{
    int *tni4=(int *)tnf4;
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
           ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}

void QJD::qjdswap_double_8(double *tndd8)
        /**************************************************************************
swap_double_8           swap a double
***************************************************************************/
{
    char *tnd8=(char *)tndd8;
    char tnc;

    tnc= *tnd8;
    *tnd8= *(tnd8+7);
    *(tnd8+7)=tnc;

    tnc= *(tnd8+1);
    *(tnd8+1)= *(tnd8+6);
    *(tnd8+6)=tnc;

    tnc= *(tnd8+2);
    *(tnd8+2)= *(tnd8+5);
    *(tnd8+5)=tnc;

    tnc= *(tnd8+3);
    *(tnd8+3)= *(tnd8+4);
    *(tnd8+4)=tnc;
}

/// 最后一个endian还包含了转换功能，可以无视，设置为1不转换
void QJD::qjdibm2ieee(int *from, int *to, int n, int endian)
        /***********************************************************************
ibm_to_float - convert between 32 bit IBM and IEEE floating numbers
************************************************************************
Input::
from            input vector
to              output vector, can be same as input vector
endian          byte order =0 little endian (DEC, PC's)
                            =1 other systems
*************************************************************************
Notes:
Up to 3 bits lost on IEEE -> IBM

Assumes sizeof(int) == 4

IBM -> IEEE may overflow or underflow, taken care of by
substituting large number or zero

Only integer shifting and masking are used.
*************************************************************************
Credits: CWP: Brian Sumner,  c.1985
*************************************************************************/
{

    register int fconv, fmant, i, t;

    for (i=0;i<n;++i) {

        fconv = from[i];

        /* if little endian, i.e. endian=0 do this */
        if (endian==0) fconv = (fconv<<24) | ((fconv>>24)&0xff) |
                               ((fconv&0xff00)<<8) | ((fconv&0xff0000)>>8);

        if (fconv) {
            fmant = 0x00ffffff & fconv;
            /* The next two lines were added by Toralf Foerster */
            /* to trap non-IBM format data i.e. conv=0 data  */
            if (fmant == 0)
                printf("mantissa is zero data may not be in IBM FLOAT Format !");
            t = (int) ((0x7f000000 & fconv) >> 22) - 130;
            while (!(fmant & 0x00800000)) { --t; fmant <<= 1; }
            if (t > 254) fconv = (0x80000000 & fconv) | 0x7f7fffff;
            else if (t <= 0) fconv = 0;
            else fconv = (0x80000000 & fconv) |(t << 23)|(0x007fffff & fmant);
        }
        to[i] = fconv;
    }
    return;

}

void QJD::qjdieee2ibm(int *from,int *to, int n, int endian)
        /**********************************************************************
 float_to_ibm - convert between 32 bit IBM and IEEE floating numbers
***********************************************************************
Input:
from       input vector
n          number of floats in vectors
endian     =0 for little endian machine, =1 for big endian machines

Output:
to         output vector, can be same as input vector

***********************************************************************
Notes:
Up to 3 bits lost on IEEE -> IBM

IBM -> IEEE may overflow or underflow, taken care of by
substituting large number or zero

Only integer shifting and masking are used.
***********************************************************************
Credits:     CWP: Brian Sumner
***********************************************************************/
{
    register int fconv, fmant, i, t;

    for (i=0;i<n;++i) {
        fconv = from[i];
        if (fconv) {
            fmant = (0x007fffff & fconv) | 0x00800000;
            t = (int) ((0x7f800000 & fconv) >> 23) - 126;
            while (t & 0x3) { ++t; fmant >>= 1; }
            fconv = (0x80000000 & fconv) | (((t>>2) + 64) << 24) | fmant;
        }
        if(endian==0)
            fconv = (fconv<<24) | ((fconv>>24)&0xff) |
                    ((fconv&0xff00)<<8) | ((fconv&0xff0000)>>8);

        to[i] = fconv;
    }
    return;

}
