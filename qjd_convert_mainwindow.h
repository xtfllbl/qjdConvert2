#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QFile>
#include <QDebug>
#include <QButtonGroup>
#include "qjd.h"
#include "qjddatastream.h"
#include "qjdtraceheader.h"
#include "qjdsegybh.h"
#include "qjdsegyth.h"
#include <QInputDialog>
#include <QStringListModel>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool isSegy;
    bool isSu;
    int headNum;
    QString openFileName;
    QString convertToFileName;
    QFile fOpen;
    QFile fConvert;

    float temp;
    QButtonGroup groupFormat;
    QButtonGroup groupByteOrder;
    QButtonGroup groupOriginalFormat;
    QButtonGroup groupOriginalByteOrder;
    QButtonGroup groupFunction;
    QButtonGroup groupChooseFunction;
    QJDTraceHeader thTemp;
    QJDSegyBH bhTemp;
    int count;
    qint64 traceLength;

    QJDDataStream read;
    QJDDataStream write;
    QJDTraceHeader th;
    QByteArray bty;
    QJDSegyBH bh;
//    QJD qjd;  //受保护，直接调用就是


    bool flagBHis0;     //判断文件中是否信息不完整
    bool flagTHis0;

    bool flagStandard;      //功能启用和关闭
    bool flagNonStandard;
    bool flagResample;
    bool flagAcResample;
    bool flagTraceScope;
    bool flagSkip;
    bool flagResampleChange;    /// 用来处理标准resample和精确resample同时启用引发的冲突问题

    bool flagUse;           //decide数据类型系列中使用
    bool flagStandardUse;   //decide(int int int )中使用

    bool flagSetInfo;
    bool flagSU;    //转换成SU
    bool flagStandardDefault;
    bool flagNonStandardDefault;

    /// 标准道头
    QVector<QString> s180;
    int SCI;                    //Standard Current Index

    /// 非标准道头
    QVector<QString> dataType;
    int intx;                    //四种类型
    qint16 qint16x;
    float floatx;
    quint16 quint16x;

    int NSCI;                   //Non-Standard Current Index
    int seekPos;             //用户指定偏移距离
    qint64 seekSave;      //存储当前的偏移距离

    /// 道头通用
    QVector<QString> dataRangeTH;
    QString rangeTempTH;
    QStringList rangeFinalTH;
    QStringListModel *mdTH;
    QVector<QString> leftTH;
    QVector<QString> rightTH;
    QVector<int> leftNumTH;
    QVector<int> rightNumTH;

    /// 重采样
    int skipNum;
    uint oriDt;
    uint oriNs;
    uint nowDt;
    uint nowNs;

    int maxTime;
    QVector<QString> dataRangeTC;
    QString rangeTempTC;
    QStringList rangeFinalTC;
    QStringListModel *mdTC;
    QVector<QString> leftTC;
    QVector<QString> rightTC;
    QVector<int> leftNumTC;
    QVector<int> rightNumTC;
    QVector<int> leftNumTCReal;
    QVector<int> rightNumTCReal;
    QVector<int> rightNumTCMinus;

    QVector<int> loopNum;        //重采样时每段的循环次数都要改变
    int allPointNum;                  //重采样时改变th.ns
    qint64 newTraceNum;

    /// 取道
    QVector<QString> dataRange;
    QString rangeTemp;
    QStringList rangeFinal;
    QStringListModel *md;
    QVector<QString> left;
    QVector<QString> right;
    QVector<int> leftNum;
    QVector<int> rightNum;

    /// 隔道取道
    int skipTrace;
    QVector<int> skipLoop;

    /// 全局
    bool flagConvertComplete;

    /// -------------------函数----------------------///
    void resizeEvent();
    void setData();
    bool decide(int aa,int bb,int xx);
    bool decideInt(int aa,int bb);
    bool decideQint16(int aa,int bb);
    bool decideQuint16(int aa,int bb);
    bool decideFloat(int aa,int bb);
    void lastClear();
    void flagClear();
    void allInOne();
    void setInfo();
    void convert2SU();
    void convert2SEGY();
    void convertStandard();
    void convertNonStandard();
    void convertTraceScope();
    void convertResample();
    void convertSkipTrace();
    int reference(int index);
    void checkInfo();

    /// 数据读取和写入
    void read3200();
    void read400();
    void readData();


private slots:
    void on_checkAcResample_clicked(bool checked);
    void on_toolRemoveTime_clicked();
    void on_toolAddTime_clicked();
    void on_btnAD_clicked(bool checked);
    void on_combo240_currentIndexChanged(int index);
    void on_toolTHRemove_clicked();
    void on_toolTHAdd_clicked();
    void on_lineConvertToFileName_textChanged(QString );
    void on_checkTraceScope_clicked(bool checked);
    void on_checkResample_clicked(bool checked);
    void on_checkStandard_clicked(bool checked);
    void on_checkSkip_clicked(bool checked);
    void on_radioNonStandard_clicked();
    void on_radioStandard_clicked();
    void on_toolRemove_clicked();
    void on_toolAdd_clicked();
    void on_btnConvertTo_clicked();
    void on_btnConvert_clicked();
    void on_actionExit_triggered();
    void on_actionOpen_triggered();

    void depend();
};

#endif // MAINWINDOW_H
