#include "qjdsegybh.h"
#include "qjd.h"

QJDSegyBH::QJDSegyBH(void)
{
    init();
}

QJDSegyBH::~QJDSegyBH(void)
{
}

void QJDSegyBH::init()
{
    jobid =0;		//job id 3201-3204
    lino=0;		//line number 3205-3208
    reno=0;		//Reel number 3209-3212
    ntrpr=0;		//number of data traces per ensemble 3213-1214
    nart=0;		//number of auxiliary traces per ensemble 3215-3216
    hdt=0;		//sample interval in microsecond 3217-3218
    dto=0;		//sample interval in microsecond of original field recording. 3219-3220
    hns=0;		//number of slices per data trace; 3221-3222
    nso=0;		//number of slices per data trace for original field recording. 3223-3224
    format=0;		//data sample format code. 3225-3226
    fold=0;		//ensemble fold=0; 3227-3228
    tsort=0;		//trace sort code. 3229-3230
    vscode=0;		//vertical sum code.	3231-3232
    hsfs=0;		//sweep frequency at start (HZ) 3233-3234
    hsfe=0;		//sweep frequency at end(HZ) 3235-3236
    hslen=0;		//sweep length (ms). 3237-3238
    hstyp=0;		//sweep type code. 3239-3240
    schn=0;		//trace number of sweep channel 3241-3242
    hstas=0;		//sweep trace taper length in millionsecond at start if tapered.3243-3244
    hstae=0;		//sweep trace taper length in millionsecond at end.3245-3246
    htatyp=0;		//taper type 3247-3248
    hcorr=0;		//correlated data traces 3249-3250
    bgrcv=0;		//binary gain recovered. 3251-3252
    rcvm=0;		//Amplitude recovery method.3253-3254
    mfeet=0;		//Measurement System.3255-3256
    polyt=0;		//implus signal polarity 3257-3258
    vpol=0;		//Vibratory polarity code 3259-3260



}

bool QJDSegyBH::readData(const QString& fn,QJDDataStream::ByteOrder bo,QJDDataStream::DataFormat df)
{
    QFile tf(fn);
    if (!tf.open(QIODevice::ReadOnly))
        return false;
    QJDDataStream ds(&tf);
    ds.setByteOrder(bo);        // 设置Little Endian或BigEndian
    ds.setFormat(df);           // 设置IBM 或 IEEE
    tf.seek(3200);
    ds>>*this;                  //如果格式是IBM，则全部转换为IEEE。如果不是IBM，则直接输入
    //ds.unsetDevice();           //已经被淘汰，使用setDevice(0)来代替
    ds.setDevice(0);
    tf.close();

    return true;
}
bool QJDSegyBH::writeData(const QString& fn,QJDDataStream::ByteOrder bo,QJDDataStream::DataFormat df)
{
    QFile tf(fn);
    if (!tf.open(QIODevice::ReadWrite))
        return false;
    QJDDataStream ds(&tf);
    ds.setByteOrder(bo);
    ds.setFormat(df);
    tf.seek(3200);
    ds<<*this;
    //ds.unsetDevice();
    ds.setDevice(0);
    tf.close();
    return true;
}

//****************************Friend Global Functions***********************//
QJDDataStream & operator<< ( QJDDataStream & ds, const QJDSegyBH &s)
{
    ds<<s.jobid<<s.lino<<s.reno<<s.ntrpr<<s.nart<<s.hdt<<s.dto<<s.hns
            <<s.nso<<s.format<<s.fold<<s.tsort<<s.vscode<<s.hsfs<<s.hsfe
            <<s.hslen<<s.hstyp<<s.schn<<s.hstas<<s.hstae<<s.htatyp<<s.hcorr
            <<s.bgrcv<<s.rcvm<<s.mfeet<<s.polyt<<s.vpol;
    for(int i=0; i<170; i++)
        ds<<s.hunass[i];

    return ds;
}

QJDDataStream & operator>> ( QJDDataStream & ds, QJDSegyBH &s)
{
    ds>>s.jobid>>s.lino>>s.reno>>s.ntrpr>>s.nart>>s.hdt>>s.dto>>s.hns
            >>s.nso>>s.format>>s.fold>>s.tsort>>s.vscode>>s.hsfs>>s.hsfe
            >>s.hslen>>s.hstyp>>s.schn>>s.hstas>>s.hstae>>s.htatyp>>s.hcorr
            >>s.bgrcv>>s.rcvm>>s.mfeet>>s.polyt>>s.vpol;
    for(int i=0; i<170; i++)
        ds>>s.hunass[i];
    return ds;
}

//void QJDSegyBH::swap_header()
//{
//        QJD::qjdswap_int_4(&jobid);
//        QJD::qjdswap_int_4(&lino);
//        QJD::qjdswap_int_4(&reno);
//        QJD::qjdswap_short_2(&ntrpr);
//        QJD::qjdswap_short_2(&nart);
//        QJD::qjdswap_u_short_2(&hdt);
//        QJD::qjdswap_u_short_2(&dto);
//        QJD::qjdswap_u_short_2(&hns);
//        QJD::qjdswap_u_short_2(&nso);
//        QJD::qjdswap_short_2(&format);
//        QJD::qjdswap_short_2(&fold);
//        QJD::qjdswap_short_2(&tsort);
//        QJD::qjdswap_short_2(&vscode);
//        QJD::qjdswap_short_2(&hsfs);
//        QJD::qjdswap_short_2(&hsfe);
//        QJD::qjdswap_short_2(&hslen);
//        QJD::qjdswap_short_2(&hstyp);
//        QJD::qjdswap_short_2(&schn);
//        QJD::qjdswap_short_2(&hstas);
//        QJD::qjdswap_short_2(&hstae);
//        QJD::qjdswap_short_2(&htatyp);
//        QJD::qjdswap_short_2(&hcorr);
//        QJD::qjdswap_short_2(&bgrcv);
//        QJD::qjdswap_short_2(&rcvm);
//        QJD::qjdswap_short_2(&mfeet);
//        QJD::qjdswap_short_2(&polyt);
//        QJD::qjdswap_short_2(&vpol);

//}
/*QTextStream  & operator<< ( QTextStream & ts, const QJDSegyBH &s)
{
        ts<<" jobid"<<s.jobID
        <<" ln "<<s.ln
        <<" rn "<<s.rn
        <<" nodtpe "<<s.nodtpe
        <<" noatpe "<<s.noatpe
        <<" si "<<s.si
        <<" siofr "<<s.siofr
        <<" nospdt "<<s.nospdt
        <<" nosofr "<<s.nosofr
        <<" dataFormat "<<s.dataFormat
        <<" EF "<<s.EF
        <<" tsc "<<s.tsc
        <<" vsc "<<s.vsc
        <<" swfas "<<s.swfas
        <<" swfae "<<s.swfae
        <<" swl "<<s.swl
        <<" swtc "<<s.swtc
        <<" tnosc "<<s.tnosc
        <<" swttls "<<s.swttls
        <<" swttle "<<s.swttle
        <<" taty "<<s.taty
        <<" cdt "<<s.cdt
        <<" bgr "<<s.bgr
        <<" arm "<<s.arm
        <<" ms "<<s.ms
        <<" isp "<<s.isp
        <<" vpc "<<s.vpc

        return ts;
}*/
