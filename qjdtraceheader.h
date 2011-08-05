#ifndef QJDTRACEHEADER_H
#define QJDTRACEHEADER_H

#include <QTextStream>

#include "qjddatastream.h"

/*
   ClassName:QJDTraceHeader

   Description: 

*/
class QJDTraceHeader 
{
public:
    static uint headlen;
    QJDTraceHeader();
    QJDTraceHeader( const QByteArray &ba, QDataStream::ByteOrder bo = QDataStream::BigEndian,QJDDataStream::DataFormat df = QJDDataStream::IEEE );
    ~QJDTraceHeader();

    //   bool convertFromStHead( const QstStTraceHead &st );
    bool convertFromData( const QByteArray& ba, QDataStream::ByteOrder bo = QDataStream::BigEndian,QJDDataStream::DataFormat df = QJDDataStream::IEEE );
    bool convertToData( const QByteArray& ba, QDataStream::ByteOrder bo = QDataStream::BigEndian,QJDDataStream::DataFormat df = QJDDataStream::IEEE );

    void swap_header();

    int len() const { return headlen; }
private:
    void initialize();

public:
    //su trace head data struct:
    int tracl;    /* trace sequence number within line 001-004*/

    int tracr;    /* trace sequence number within reel 005-008*/

    int fldr;    /* field record number 009-012*/

    int tracf;    /* trace number within field record 013-016*/

    int ep;    /* energy source point number 017-020*/

    int cdp;    /* CDP ensemble number 021-024*/

    int cdpt;    /* trace number within CDP ensemble 025-028*/

    qint16 trid;    /* trace identification code:
            1 = seismic data
            2 = dead
            3 = dummy
            4 = time break
            5 = uphole
            6 = sweep
            7 = timing
            8 = water break
            9---, N = optional use (N = 32,767)

            Following are CWP id flags:

             9 = autocorrelation

            10 = Fourier transformed - no packing
                 xr[0],xi[0], ..., xr[N-1],xi[N-1]

            11 = Fourier transformed - unpacked Nyquist
                 xr[0],xi[0],...,xr[N/2],xi[N/2]

            12 = Fourier transformed - packed Nyquist
                  even N:
                 xr[0],xr[N/2],xr[1],xi[1], ...,
                xr[N/2 -1],xi[N/2 -1]
                (note the exceptional second entry)
                 odd N:
                 xr[0],xr[(N-1)/2],xr[1],xi[1], ...,
                xr[(N-1)/2 -1],xi[(N-1)/2 -1],xi[(N-1)/2]
                (note the exceptional second & last entries)

            13 = Complex signal in the time domain
                 xr[0],xi[0], ..., xr[N-1],xi[N-1]

            14 = Fourier transformed - amplitude/phase
                 a[0],p[0], ..., a[N-1],p[N-1]

            15 = Complex time signal - amplitude/phase
                 a[0],p[0], ..., a[N-1],p[N-1]

            16 = Real part of complex trace from 0 to Nyquist

            17 = Imag part of complex trace from 0 to Nyquist

            18 = Amplitude of complex trace from 0 to Nyquist

            19 = Phase of complex trace from 0 to Nyquist

            21 = Wavenumber time domain (k-t)

            22 = Wavenumber frequency (k-omega)

            23 = Envelope of the complex time trace

            24 = Phase of the complex time trace

            25 = Frequency of the complex time trace

            30 = Depth-Range (z-x) traces

            43 = Seismic Data, Vertical Component 

            44 = Seismic Data, Horizontal Component 1 

            45 = Seismic Data, Horizontal Component 2 

            46 = Seismic Data, Radial Component

            47 = Seismic Data, Transverse Component  

            101 = Seismic data packed to bytes (by supack1)
            
            102 = Seismic data packed to 2 bytes (by supack2)
            029-030*/

    qint16 nvs;    /* number of vertically summed traces (see vscode
               in bhed structure) 031-032*/

    qint16 nhs;    /* number of horizontally summed traces (see vscode
               in bhed structure) 033-034 */

    qint16 duse;    /* data use:
                1 = production
                2 = test 035-036*/

    int offset;    /* distance from source point to receiver
               group (negative if opposite to direction
               in which the line was shot) 037-040*/

    int gelev;    /* receiver group elevation from sea level
               (above sea level is positive) 041-044*/

    int selev;    /* source elevation from sea level
               (above sea level is positive) 045-048*/

    int sdepth;    /* source depth (positive) 049-052*/

    int gdel;    /* datum elevation at receiver group 053-056*/

    int sdel;    /* datum elevation at source 057-060*/

    int swdep;    /* water depth at source 061-064*/

    int gwdep;    /* water depth at receiver group 065-068*/

    qint16 scalel;    /* scale factor for previous 7 entries
               with value plus or minus 10 to the
               power 0, 1, 2, 3, or 4 (if positive,
               multiply, if negative divide) 069-070*/

    qint16 scalco;    /* scale factor for next 4 entries
               with value plus or minus 10 to the
               power 0, 1, 2, 3, or 4 (if positive,
               multiply, if negative divide) 071-072*/

    int  sx;    /* X source coordinate 073-076*/

    int  sy;    /* Y source coordinate 077-080*/

    int  gx;    /* X group coordinate 081-084*/

    int  gy;    /* Y group coordinate 085-088*/

    qint16 counit;    /* coordinate units code:
                for previous four entries
                1 = length (meters or feet)
                2 = seconds of arc (in this case, the
                X values are longitude and the Y values
                are latitude, a positive value designates
                the number of seconds east of Greenwich
                or north of the equator 089-090*/

    qint16 wevel;    /* weathering velocity 091-092*/

    qint16 swevel;    /* subweathering velocity 093-094*/

    qint16 sut;    /* uphole time at source 095-096*/

    qint16 gut;    /* uphole time at receiver group 097-098*/

    qint16 sstat;    /* source static correction 099-100*/

    qint16 gstat;    /* group static correction 101-102*/

    qint16 tstat;    /* total static applied 103-104*/

    qint16 laga;    /* lag time A, time in ms between end of 240-
               byte trace identification header and time
               break, positive if time break occurs after
               end of header, time break is defined as
               the initiation pulse which maybe recorded
               on an auxiliary trace or as otherwise
               specified by the recording system 105-106*/

    qint16 lagb;    /* lag time B, time in ms between the time break
               and the initiation time of the energy source,
               may be positive or negative 107-108 */

    qint16 delrt;    /* delay recording time, time in ms between
               initiation time of energy source and time
               when recording of data samples begins
               (for deep water work if recording does not
               start at zero time) 109-110*/

    qint16 muts;    /* mute time--start 111-112*/

    qint16 mute;    /* mute time--end 113-114*/

    quint16 ns;    /* number of samples in this trace 115-116*/

    quint16 dt;    /* sample interval; in micro-seconds 117-118 */

    qint16 gain;    /* gain type of field instruments code:
                1 = fixed
                2 = binary
                3 = floating point
                4 ---- N = optional use 119-120*/

    qint16 igc;    /* instrument gain constant 121-122*/

    qint16 igi;    /* instrument early or initial gain 123-124*/

    qint16 corr;    /* correlated:
                1 = no
                2 = yes 125-126*/

    qint16 sfs;    /* sweep frequency at start 127-128*/

    qint16 sfe;    /* sweep frequency at end 129-130*/

    qint16 slen;    /* sweep length in ms 131-132*/

    qint16 styp;    /* sweep type code:
                1 = linear
                2 = cos-squared
                3 = other 133-134*/

    qint16 stas;    /* sweep trace length at start in ms 135-136*/

    qint16 stae;    /* sweep trace length at end in ms 137-138*/

    qint16 tatyp;    /* taper type: 1=linear, 2=cos^2, 3=other 139-140*/

    qint16 afilf;    /* alias filter frequency if used 141-142*/

    qint16 afils;    /* alias filter slope 143-144*/

    qint16 nofilf;    /* notch filter frequency if used 145-146*/

    qint16 nofils;    /* notch filter slope 147-148*/

    qint16 lcf;    /* low cut frequency if used 149-150*/

    qint16 hcf;    /* high cut frequncy if used 151-152*/

    qint16 lcs;    /* low cut slope 153-154*/

    qint16 hcs;    /* high cut slope 155-156*/

    qint16 year;    /* year data recorded 157-158*/

    qint16 day;    /* day of year 159-160*/

    qint16 hour;    /* hour of day (24 hour clock) 161-162*/

    qint16 minute;    /* minute of hour 163-164*/

    qint16 sec;    /* second of minute 165-166*/

    qint16 timbas;    /* time basis code:
                1 = local
                2 = GMT
                3 = other 167-168*/

    qint16 trwf;    /* trace weighting factor, defined as 1/2^N
               volts for the least sigificant bit 169-170*/

    qint16 grnors;    /* geophone group number of roll switch
               position one 171-172*/

    qint16 grnofr;    /* geophone group number of trace one within
               original field record 173-174*/

    qint16 grnlof;    /* geophone group number of last trace within
               original field record 175-176*/

    qint16 gaps;    /* gap size (total number of groups dropped) 177-178*/

    qint16 otrav;    /* overtravel taper code:
                1 = down (or behind)
                2 = up (or ahead) 179-180*/

    //after 180.added  unassigned.
    int            lineNo;    //�ά�ݵ�ߺţά�ￄ17�ￄ189-192
    int            cmpNo;    //CMP:Ż��193-196
    float        cx;        //�Ԫ�����ￄ181-184
    float        cy;        //�Ԫ�����ￄ185-188
    int            spNo;    //shotpoint number 197-200
    qint16        scalarSpNo;    //scalar to be applied to the shotpoint number 201-202
    qint16        tvmu;    //Trace value measurement unit.203-204
    int            tc1;    //Transduction constant part1.205-208
    qint16        tc2;    //Transduction constant part2.209-210.tc1*(10^tc2)
    qint16        tu;        //Transduction units; 211-212
    qint16        dti;    //Device/Trace Identifier.213-214
    qint16        scalarTS;//Scalar to be applied to times specified. 215-216
    qint16        sType; //Source Type/Orientation 217-218
    int            sED1;    //Source energy direction with respect to the source orientation.219-222
    qint16        sED2;    //223-224
    int            sMea1;    //Source Measurement part1 225-228
    qint16        sMea2;    //Source Measurement part2 229-230 souMea1*(10^souMea2);
    qint16        sMU;        //Source Measurement Unit 231-232
    int            reserved01;        //Reserved.233-236
    int            reserved02;        //Reserved.237-240
    
private:
    //    friend class QstStTraceHead;
    //    friend class QstTraceInfo;
    friend QDataStream & operator<< ( QDataStream & ds, const QJDTraceHeader &s);
    friend QDataStream & operator>> ( QDataStream & ds, QJDTraceHeader &s);
    friend QTextStream & operator<< ( QTextStream & ts, const QJDTraceHeader &s);
    friend QTextStream & operator>> ( QTextStream & ts, QJDTraceHeader &s);

};

QDataStream & operator<< ( QDataStream & ds, const QJDTraceHeader &s);
QDataStream & operator>> ( QDataStream & ds, QJDTraceHeader &s);
#endif	//QJDTraceHeader_H
