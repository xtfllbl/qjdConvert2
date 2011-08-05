
#include "qjdtraceheader.h"
#include "qjd.h"


uint QJDTraceHeader::headlen = 240;

QJDTraceHeader::QJDTraceHeader()
{
    initialize();
}
QJDTraceHeader::~QJDTraceHeader()
{
}

QJDTraceHeader::QJDTraceHeader( const QByteArray &ba, QDataStream::ByteOrder bo,QJDDataStream::DataFormat df)
{
    convertFromData( ba, bo,df );
}

bool
QJDTraceHeader::convertFromData( const QByteArray& ba, QDataStream::ByteOrder bo,QJDDataStream::DataFormat df )
{
    if ( ba.size() != len() )
    {
        qWarning(  "QJDTraceHeader: The size of bytearry is not equal to 240!"  );
        return FALSE;
    }

    QJDDataStream ds( ba);
    ds.setByteOrder( bo );
        ds.setFormat(df);

    ds >> *this;

    ds.unsetDevice();

    return true;
}
bool
QJDTraceHeader::convertToData( const QByteArray& ba, QDataStream::ByteOrder bo,QJDDataStream::DataFormat df )
{
    if ( ba.size() != len() )
    {
        qWarning(  "QJDTraceHeader: The size of bytearry is not equal to 240!"  );
        return FALSE;
    }

    QJDDataStream ds( ba );
    ds.setByteOrder( bo );
        ds.setFormat(df);

    ds << *this;

    ds.unsetDevice();

    return true;
}
void
QJDTraceHeader::initialize()
{
    tracl=0;

    tracr=0;    /* trace sequence number within reel 005-008*/

    fldr=0;    /* field record number 009-012*/

    tracf=0;    /* trace number within field record 013-016*/

    ep=0;    /* energy source point number 017-020*/

    cdp=0;    /* CDP ensemble number 021-024*/

    cdpt=0;    /* trace number within CDP ensemble 025-028*/

    trid=1;

    nvs=0;    /* number of vertically summed traces (see vscode
               in bhed structure) 031-032*/

    nhs=0;    /* number of horizontally summed traces (see vscode
               in bhed structure) 033-034 */

    duse=1;    /* data use:
                1 = production
                2 = test 035-036*/
    offset=0;    /* distance from source point to receiver
                  group (negative if opposite to direction
                  in which the line was shot) 037-040*/

    gelev=0;    /* receiver group elevation from sea level
                  (above sea level is positive) 041-044*/

    selev=0;    /* source elevation from sea level
                  (above sea level is positive) 045-048*/

    sdepth=0;    /* source depth (positive) 049-052*/

    gdel=0;    /* datum elevation at receiver group 053-056*/

    sdel=0;    /* datum elevation at source 057-060*/

    swdep=0;    /* water depth at source 061-064*/

    gwdep=0;    /* water depth at receiver group 065-068*/

    scalel=0;    /* scale factor for previous 7 entries
					  with value plus or minus 10 to the
					  power 0, 1, 2, 3, or 4 (if positive,
					  multiply, if negative divide) 069-070*/

    scalco=0;    /* scale factor for next 4 entries
					  with value plus or minus 10 to the
					  power 0, 1, 2, 3, or 4 (if positive,
					  multiply, if negative divide) 071-072*/
    sx=0;    /* X source coordinate 073-076*/

    sy=0;    /* Y source coordinate 077-080*/

    gx=0;    /* X group coordinate 081-084*/

    gy=0;    /* Y group coordinate 085-088*/

    counit=1;    /* coordinate units code:
                      for previous four entries
                      1 = length (meters or feet)
                      2 = seconds of arc (in this case, the
                      X values are longitude and the Y values
                      are latitude, a positive value designates
                      the number of seconds east of Greenwich
                      or north of the equator 089-090*/

    wevel=0;    /* weathering velocity 091-092*/

    swevel=0;    /* subweathering velocity 093-094*/

    sut=0;    /* uphole time at source 095-096*/

    gut=0;    /* uphole time at receiver group 097-098*/

    sstat=0;    /* source static correction 099-100*/

    gstat=0;    /* group static correction 101-102*/

    tstat=0;    /* total static applied 103-104*/

    laga=0;    /* lag time A, time in ms between end of 240-
                        byte trace identification header and time
                        break, positive if time break occurs after
                        end of header, time break is defined as
                        the initiation pulse which maybe recorded
                        on an auxiliary trace or as otherwise
                        specified by the recording system 105-106*/

    lagb=0;    /* lag time B, time in ms between the time break
                        and the initiation time of the energy source,
                        may be positive or negative 107-108 */

    delrt=0;    /* delay recording time, time in ms between
                        initiation time of energy source and time
                        when recording of data samples begins
                        (for deep water work if recording does not
                        start at zero time) 109-110*/

    muts=0;    /* mute time--start 111-112*/

    mute=0;    /* mute time--end 113-114*/

    ns=0;    /* number of samples in this trace 115-116*/

    dt=0;    /* sample interval; in micro-seconds 117-118 */

    gain=1;    /* gain type of field instruments code:
                         1 = fixed
						    2 = binary
							 3 = floating point
							 4 ---- N = optional use 119-120*/

    igc=0;    /* instrument gain constant 121-122*/

    igi=0;    /* instrument early or initial gain 123-124*/

    corr=1;    /* correlated:
                1 = no
                2 = yes 125-126*/

    sfs=0;    /* sweep frequency at start 127-128*/

    sfe=0;    /* sweep frequency at end 129-130*/

    slen=0;    /* sweep length in ms 131-132*/

    styp=1;    /* sweep type code:
                1 = linear
                2 = cos-squared
                3 = other 133-134*/

    stas=0;    /* sweep trace length at start in ms 135-136*/

    stae=0;    /* sweep trace length at end in ms 137-138*/

    tatyp=1;    /* taper type: 1=linear, 2=cos^2, 3=other 139-140*/

    afilf=0;    /* alias filter frequency if used 141-142*/

    afils=0;    /* alias filter slope 143-144*/

    nofilf=0;    /* notch filter frequency if used 145-146*/

    nofils=0;    /* notch filter slope 147-148*/

    lcf=0;    /* low cut frequency if used 149-150*/

    hcf=0;    /* high cut frequncy if used 151-152*/

    lcs=0;    /* low cut slope 153-154*/

    hcs=0;    /* high cut slope 155-156*/

    year=0;    /* year data recorded 157-158*/

    day=0;    /* day of year 159-160*/

    hour=0;    /* hour of day (24 hour clock) 161-162*/

    minute=0;    /* minute of hour 163-164*/

    sec=0;    /* second of minute 165-166*/

    timbas=1;    /* time basis code:
                    1 = local
                    2 = GMT
                    3 = other 167-168*/
    trwf=0;    /* trace weighting factor, defined as 1/2^N
                  volts for the least sigificant bit 169-170*/

    grnors=0;    /* geophone group number of roll switch
                  position one 171-172*/

    grnofr=0;    /* geophone group number of trace one within
                  original field record 173-174*/

    grnlof=0;    /* geophone group number of last trace within
                  original field record 175-176*/

    gaps=0;    /* gap size (total number of groups dropped) 177-178*/

    otrav=1;    /* overtravel taper code:
                   1 = down (or behind)
                   2 = up (or ahead) 179-180*/

       //after 180.added unassigned.
      lineNo=0;    //�ά�ݵ�ߺţά��0���189-192
      cmpNo=0;    //CMP:Ż��193-196
       cx=0.0;        //�Ԫ������181-184
       cy=0.0;        //�Ԫ������185-188

       spNo=0;    //shotpoint number 197-200
       scalarSpNo=0;    //scalar to be applied to the shotpoint number 201-202
       tvmu=0;    //Trace value measurement unit.203-204
       tc1=0;    //Transduction constant part1.205-208
       tc2=0;    //Transduction constant part2.209-210.tc1*(10^tc2)
       tu=0;        //Transduction units; 211-212
       dti=0;    //Device/Trace Identifier.213-214
       scalarTS=0;//Scalar to be applied to times specified. 215-216
       sType=0; //Source Type/Orientation 217-218
       sED1=0;        //Source energy direction with respect to the source orientation.219-224
       sED2=0;
       sMea1=0;    //Source Measurement part1 225-228
       sMea2=0;    //Source Measurement part2 229-230 souMea1*(10^souMea2);
       sMU=0;        //Source Measurement Unit 231-232
       reserved01=0;    //Reserved.233-236
       reserved02=0;    //237-240
}

QJDDataStream &
operator<< ( QJDDataStream & ds, const QJDTraceHeader &s)
{
    ds<<s.tracl<<s.tracr<<s.fldr<<s.tracf<<s.ep<<s.cdp<<s.cdpt
        <<s.trid<<s.nvs<<s.nhs<<s.duse<<s.offset<<s.gelev<<s.selev
        <<s.sdepth<<s.gdel<<s.sdel<<s.swdep<<s.gwdep<<s.scalel
        <<s.scalco<<s.sx<<s.sy<<s.gx<<s.gy<<s.counit<<s.wevel
        <<s.swevel<<s.sut<<s.gut<<s.sstat<<s.gstat<<s.tstat<<s.laga
        <<s.lagb<<s.delrt<<s.muts<<s.mute<<s.ns<<s.dt<<s.gain<<s.igc
        <<s.igi<<s.corr<<s.sfs<<s.sfe<<s.slen<<s.styp<<s.stas<<s.stae
        <<s.tatyp<<s.afilf<<s.afils<<s.nofilf<<s.nofils<<s.lcf<<s.hcf
        <<s.lcs<<s.hcs<<s.year<<s.day<<s.hour<<s.minute<<s.sec<<s.timbas
        <<s.trwf<<s.grnors<<s.grnofr<<s.grnlof<<s.gaps<<s.otrav
        <<s.lineNo<<s.cmpNo<<s.cx<<s.cy<<s.spNo<<s.scalarSpNo<<s.tvmu
        <<s.tc1<<s.tc2<<s.tu<<s.dti<<s.scalarTS<<s.sType<<s.sED1<<s.sED2<<s.sMea1
        <<s.sMea2<<s.sMU<<s.reserved01<<s.reserved02;
    return ds;
}

QJDDataStream &
operator>> ( QJDDataStream & ds, QJDTraceHeader &s)
{
    ds>>s.tracl>>s.tracr>>s.fldr>>s.tracf>>s.ep>>s.cdp>>s.cdpt
        >>s.trid>>s.nvs>>s.nhs>>s.duse>>s.offset>>s.gelev>>s.selev
        >>s.sdepth>>s.gdel>>s.sdel>>s.swdep>>s.gwdep>>s.scalel
        >>s.scalco>>s.sx>>s.sy>>s.gx>>s.gy>>s.counit>>s.wevel
        >>s.swevel>>s.sut>>s.gut>>s.sstat>>s.gstat>>s.tstat>>s.laga
        >>s.lagb>>s.delrt>>s.muts>>s.mute>>s.ns>>s.dt>>s.gain>>s.igc
        >>s.igi>>s.corr>>s.sfs>>s.sfe>>s.slen>>s.styp>>s.stas>>s.stae
        >>s.tatyp>>s.afilf>>s.afils>>s.nofilf>>s.nofils>>s.lcf>>s.hcf
        >>s.lcs>>s.hcs>>s.year>>s.day>>s.hour>>s.minute>>s.sec>>s.timbas
        >>s.trwf>>s.grnors>>s.grnofr>>s.grnlof>>s.gaps>>s.otrav
        >>s.lineNo>>s.cmpNo>>s.cx>>s.cy>>s.spNo>>s.scalarSpNo>>s.tvmu
        >>s.tc1>>s.tc2>>s.tu>>s.dti>>s.scalarTS>>s.sType>>s.sED1>>s.sED2>>s.sMea1
        >>s.sMea2>>s.sMU>>s.reserved01>>s.reserved02;

    return ds;
}
QDataStream &
operator<< ( QDataStream & ds, const QJDTraceHeader &s)
{
    ds<<s.tracl<<s.tracr<<s.fldr<<s.tracf<<s.ep<<s.cdp<<s.cdpt
        <<s.trid<<s.nvs<<s.nhs<<s.duse<<s.offset<<s.gelev<<s.selev
        <<s.sdepth<<s.gdel<<s.sdel<<s.swdep<<s.gwdep<<s.scalel
        <<s.scalco<<s.sx<<s.sy<<s.gx<<s.gy<<s.counit<<s.wevel
        <<s.swevel<<s.sut<<s.gut<<s.sstat<<s.gstat<<s.tstat<<s.laga
        <<s.lagb<<s.delrt<<s.muts<<s.mute<<s.ns<<s.dt<<s.gain<<s.igc
        <<s.igi<<s.corr<<s.sfs<<s.sfe<<s.slen<<s.styp<<s.stas<<s.stae
        <<s.tatyp<<s.afilf<<s.afils<<s.nofilf<<s.nofils<<s.lcf<<s.hcf
        <<s.lcs<<s.hcs<<s.year<<s.day<<s.hour<<s.minute<<s.sec<<s.timbas
        <<s.trwf<<s.grnors<<s.grnofr<<s.grnlof<<s.gaps<<s.otrav
        <<s.lineNo<<s.cmpNo<<s.cx<<s.cy<<s.spNo<<s.scalarSpNo<<s.tvmu
        <<s.tc1<<s.tc2<<s.tu<<s.dti<<s.scalarTS<<s.sType<<s.sED1<<s.sED2<<s.sMea1
        <<s.sMea2<<s.sMU<<s.reserved01<<s.reserved02;
    return ds;
}
QDataStream &
operator>> ( QDataStream & ds, QJDTraceHeader &s)
{
    ds>>s.tracl>>s.tracr>>s.fldr>>s.tracf>>s.ep>>s.cdp>>s.cdpt
        >>s.trid>>s.nvs>>s.nhs>>s.duse>>s.offset>>s.gelev>>s.selev
        >>s.sdepth>>s.gdel>>s.sdel>>s.swdep>>s.gwdep>>s.scalel
        >>s.scalco>>s.sx>>s.sy>>s.gx>>s.gy>>s.counit>>s.wevel
        >>s.swevel>>s.sut>>s.gut>>s.sstat>>s.gstat>>s.tstat>>s.laga
        >>s.lagb>>s.delrt>>s.muts>>s.mute>>s.ns>>s.dt>>s.gain>>s.igc
        >>s.igi>>s.corr>>s.sfs>>s.sfe>>s.slen>>s.styp>>s.stas>>s.stae
        >>s.tatyp>>s.afilf>>s.afils>>s.nofilf>>s.nofils>>s.lcf>>s.hcf
        >>s.lcs>>s.hcs>>s.year>>s.day>>s.hour>>s.minute>>s.sec>>s.timbas
        >>s.trwf>>s.grnors>>s.grnofr>>s.grnlof>>s.gaps>>s.otrav
        >>s.lineNo>>s.cmpNo>>s.cx>>s.cy>>s.spNo>>s.scalarSpNo>>s.tvmu
        >>s.tc1>>s.tc2>>s.tu>>s.dti>>s.scalarTS>>s.sType>>s.sED1>>s.sED2>>s.sMea1
        >>s.sMea2>>s.sMU>>s.reserved01>>s.reserved02;

    return ds;
}
QTextStream &
operator<< ( QTextStream & ts, const QJDTraceHeader &s)
{
    ts<<" tracl "<<s.tracl
        <<" tracr "<<s.tracr
        <<" fldr "<<s.fldr
        <<" tracf "<<s.tracf
        <<" ep "<<s.ep
        <<" cdp "<<s.cdp
        <<" cdpt "<<s.cdpt
        <<" trid "<<s.trid
        <<" nvs "<<s.nvs
        <<" nhs "<<s.nhs
        <<" duse "<<s.duse
        <<" offset "<<s.offset
        <<" gelev "<<s.gelev
        <<" selev "<<s.selev
        <<" sdepth "<<s.sdepth
        <<" gdel "<<s.gdel
        <<" sdel "<<s.sdel
        <<" swdep "<<s.swdep
        <<" gwdep "<<s.gwdep
        <<" scalel "<<s.scalel
        <<" scalco "<<s.scalco
        <<" sx "<<s.sx
        <<" sy "<<s.sy
        <<" gx "<<s.gx
        <<" gy "<<s.gy
        <<" counit "<<s.counit
        <<" wevel "<<s.wevel
        <<" swevel "<<s.swevel
        <<" sut "<<s.sut
        <<" gut "<<s.gut
        <<" sstat "<<s.sstat
        <<" gstat "<<s.gstat
        <<" tstat "<<s.tstat
        <<" laga "<<s.laga
        <<" lagb "<<s.lagb
        <<" delrt "<<s.delrt
        <<" muts "<<s.muts
        <<" mute "<<s.mute
        <<" ns "<<s.ns
        <<" dt "<<s.dt
        <<" gain "<<s.gain
        <<" igc "<<s.igc
        <<" igi "<<s.igi
        <<" corr "<<s.corr
        <<" sfs "<<s.sfs
        <<" sfe "<<s.sfe
        <<" slen "<<s.slen
        <<" styp "<<s.styp
        <<" stas "<<s.stas
        <<" stae "<<s.stae
        <<" tatyp "<<s.tatyp
        <<" afilf "<<s.afilf
        <<" afils "<<s.afils
        <<" nofilf "<<s.nofilf
        <<" nofils "<<s.nofils
        <<" lcf "<<s.lcf
        <<" hcf "<<s.hcf
        <<" lcs "<<s.lcs
        <<" hcs "<<s.hcs
        <<" year "<<s.year
        <<" day "<<s.day
        <<" hour "<<s.hour
        <<" minute "<<s.minute
        <<" sec "<<s.sec
        <<" timbas "<<s.timbas
        <<" trwf "<<s.trwf
        <<" grnors "<<s.grnors
        <<" grnofr "<<s.grnofr
        <<" grnlof "<<s.grnlof
        <<" gaps "<<s.gaps
        <<" otrav "<<s.otrav
        <<" lineNo "<<s.lineNo
        <<" cmpNo "<<s.cmpNo
        <<" cx "<<s.cx
        <<" cy "<<s.cy
        <<" spNo "<<s.spNo
        <<" scalarSpNo "<<s.scalarSpNo
        <<" tvmu "<<s.tvmu
        <<" tc1 "<<s.tc1
        <<" tc2 "<<s.tc2
        <<" tu "<<s.tu
        <<" dti "<<s.dti
        <<" scalarTS "<<s.scalarTS
        <<" sType "<<s.sType
        <<" sED1 "<<s.sED1
        <<" sED2 "<<s.sED2
        <<" sMea1 "<<s.sMea1
        <<" sMea2 "<<s.sMea2
        <<" sMU "<<s.sMU
        <<" reserved01 "<<s.reserved01
        <<" reserved02 "<<s.reserved02
        <<"\n";
    return ts;
}

void QJDTraceHeader::swap_header()
{
	QJD::qjdswap_int_4(&tracl);
	QJD::qjdswap_int_4(&tracr);
	QJD::qjdswap_int_4(&fldr);
	QJD::qjdswap_int_4(&tracf);
	QJD::qjdswap_int_4(&ep);
	QJD::qjdswap_int_4(&cdp);
	QJD::qjdswap_int_4(&cdpt);
        QJD::qjdswap_short_2(&trid);
        QJD::qjdswap_short_2(&nvs);
        QJD::qjdswap_short_2(&nhs);
        QJD::qjdswap_short_2(&duse);
	QJD::qjdswap_int_4(&offset);
	QJD::qjdswap_int_4(&gelev);
	QJD::qjdswap_int_4(&selev);
	QJD::qjdswap_int_4(&sdepth);
	QJD::qjdswap_int_4(&gdel);
	QJD::qjdswap_int_4(&sdel);
	QJD::qjdswap_int_4(&swdep);
	QJD::qjdswap_int_4(&gwdep);
        QJD::qjdswap_short_2(&scalel);
        QJD::qjdswap_short_2(&scalco);
	QJD::qjdswap_int_4(&sx);
	QJD::qjdswap_int_4(&sy);
	QJD::qjdswap_int_4(&gx);
	QJD::qjdswap_int_4(&gy);
        QJD::qjdswap_short_2(&counit);
        QJD::qjdswap_short_2(&wevel);
        QJD::qjdswap_short_2(&swevel);
        QJD::qjdswap_short_2(&sut);
        QJD::qjdswap_short_2(&gut);
        QJD::qjdswap_short_2(&sstat);
        QJD::qjdswap_short_2(&gstat);
        QJD::qjdswap_short_2(&tstat);
        QJD::qjdswap_short_2(&laga);
        QJD::qjdswap_short_2(&lagb);
        QJD::qjdswap_short_2(&delrt);
        QJD::qjdswap_short_2(&muts);
        QJD::qjdswap_short_2(&mute);
        QJD::qjdswap_u_short_2(&ns);
        QJD::qjdswap_u_short_2(&dt);
        QJD::qjdswap_short_2(&gain);
        QJD::qjdswap_short_2(&igc);
        QJD::qjdswap_short_2(&igi);
        QJD::qjdswap_short_2(&corr);
        QJD::qjdswap_short_2(&sfs);
        QJD::qjdswap_short_2(&sfe);
        QJD::qjdswap_short_2(&slen);
        QJD::qjdswap_short_2(&styp);
        QJD::qjdswap_short_2(&stas);
        QJD::qjdswap_short_2(&stae);
        QJD::qjdswap_short_2(&tatyp);
        QJD::qjdswap_short_2(&afilf);
        QJD::qjdswap_short_2(&afils);
        QJD::qjdswap_short_2(&nofilf);
        QJD::qjdswap_short_2(&nofils);
        QJD::qjdswap_short_2(&lcf);
        QJD::qjdswap_short_2(&hcf);
        QJD::qjdswap_short_2(&lcs);
        QJD::qjdswap_short_2(&hcs);
        QJD::qjdswap_short_2(&year);
        QJD::qjdswap_short_2(&day);
        QJD::qjdswap_short_2(&hour);
        QJD::qjdswap_short_2(&minute);
        QJD::qjdswap_short_2(&sec);
        QJD::qjdswap_short_2(&timbas);
        QJD::qjdswap_short_2(&trwf);
        QJD::qjdswap_short_2(&grnors);
        QJD::qjdswap_short_2(&grnofr);
        QJD::qjdswap_short_2(&grnlof);
        QJD::qjdswap_short_2(&gaps);
        QJD::qjdswap_short_2(&otrav);
	QJD::qjdswap_int_4(&lineNo);
	QJD::qjdswap_int_4(&cmpNo);
	QJD::qjdswap_float_4(&cx);
	QJD::qjdswap_float_4(&cy);
	QJD::qjdswap_int_4(&spNo);
        QJD::qjdswap_short_2(&scalarSpNo);
        QJD::qjdswap_short_2(&tvmu);
        QJD::qjdswap_int_4(&tc1);
        QJD::qjdswap_short_2(&tc2);
        QJD::qjdswap_short_2(&tu);
        QJD::qjdswap_short_2(&dti);
        QJD::qjdswap_short_2(&scalarTS);
        QJD::qjdswap_short_2(&sType);
        QJD::qjdswap_int_4(&sED1);
        QJD::qjdswap_short_2(&sED2);
        QJD::qjdswap_int_4(&sMea1);
        QJD::qjdswap_short_2(&sMea2);
        QJD::qjdswap_short_2(&sMU);
        QJD::qjdswap_int_4(&reserved01);
        QJD::qjdswap_int_4(&reserved02);
}
