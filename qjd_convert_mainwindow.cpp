#include "qjd_convert_mainwindow.h"
#include "ui_qjd_convert_mainwindow.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <QMessageBox>
#include <QMap>
#include <QDesktopWidget>
#include <math.h>
/// TODO: 需要一个全新的dataIN ,dataOUT,最好是一个底层模块接口，然后通过批处理来完成
// 完全重写，需要对数据传输有重新认识
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->setMovable(false);

    int width=QApplication::desktop()->width();         //获取屏幕的分辨率
    int height=QApplication::desktop()->height();
    this->move((width/2)-250,(height/2)-300);

    md=new QStringListModel(this);
    mdTH=new QStringListModel(this);
    mdTC=new QStringListModel(this);

    rangeFinal.clear();

    ui->btnConvert->setEnabled(false);
    ui->groupBox_3->setEnabled(false);
    ui->groupOutput->setEnabled(false);
    ui->groupOption->setEnabled(false);

    groupOriginalFormat.addButton(ui->radioOriginalIEEE);   //Convert时选择的group
    groupOriginalFormat.addButton(ui->radioOriginalIBM);
    groupOriginalByteOrder.addButton(ui->radioOriginalLittleEndian);
    groupOriginalByteOrder.addButton(ui->radioOriginalBigEndian);

    groupFormat.addButton(ui->radioIEEE);       //Format时选择的group
    groupFormat.addButton(ui->radioIBM);
    groupByteOrder.addButton(ui->radioLittleEndian);
    groupByteOrder.addButton(ui->radioBigEndian);
    groupFunction.addButton(ui->radioConvert);
    groupFunction.addButton(ui->radioFormat);

    groupChooseFunction.addButton(ui->radioStandard);
    groupChooseFunction.addButton(ui->radioNonStandard);    

    flagConvertComplete=false;
    flagClear();        //初始化flag

    ui->groupTraceHeader->hide();
    ui->groupTraceHeader->hide();
    ui->groupResample->hide();
    ui->groupTraceScope->hide();
    ui->groupChoose->hide();
    ui->groupOption->hide();

    ui->spinSkip->setVisible(false);
    ui->label_9->hide();
    ui->combo240->hide();
    ui->label_23->hide();
    ui->lineStPos->hide();
    ui->label_12->hide();
    ui->label_24->hide();
    ui->comboDataType->hide();
    ui->label_10->hide();
    ui->labelReference->hide();
    ui->labelBlank->hide();
    ui->labelBlank2->hide();
    ui->labelBlank3->hide();
    ui->label_14->hide();
    ui->listTimeRange->hide();
    ui->label_11->hide();
    ui->labelMaxTime->hide();
    ui->toolAddTime->hide();
    ui->toolRemoveTime->hide();

    connect(ui->radioIEEE,SIGNAL(clicked()),this,SLOT(depend()));
    connect(ui->radioIBM,SIGNAL(clicked()),this,SLOT(depend()));
    connect(ui->radioLittleEndian,SIGNAL(clicked()),this,SLOT(depend()));
    connect(ui->radioBigEndian,SIGNAL(clicked()),this,SLOT(depend()));

    layout()->setSizeConstraint(QLayout::SetFixedSize);     //设一下layout，窗口伸缩轻松自如
    ui->groupTraceHeader->layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->groupTraceScope->layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->groupResample->layout()->setSizeConstraint(QLayout::SetFixedSize);

    isSegy=false;
    isSu=false;
    headNum=0;      //su是0，segy是3600

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    if(fOpen.isOpen())
        fOpen.close();
    openFileName=QFileDialog::getOpenFileName(this,tr("Open Files"),"/home/xtf/Source",
                                              tr("SEGY or SU files(*segy *Segy *SEGY *sgy *Sgy *SGY *su *SU *Su)"));
    if(openFileName=="")
        return;
    ui->labelOpenFileName->setText(openFileName);
    fOpen.setFileName(openFileName);
    if(!fOpen.open(QIODevice::ReadOnly))
        QMessageBox::warning(this,tr("Open Error"),tr("The file can`t be opened as excepted."));
    count=0;

    /// 从这里就开始有可能需要变化
    read.setDevice(&fOpen);
    read.setByteOrder(QJDDataStream::BigEndian);        //设置默认为big，如果为little则再次进行计算
    setData();

    ui->groupBox_3->setEnabled(true);
    ui->groupOutput->setEnabled(true);
    ui->groupOption->setEnabled(true);
}

void MainWindow::on_btnConvertTo_clicked()
{
    if(ui->radioConvert->isChecked()==false && ui->radioFormat->isChecked()==false)
    {
        QMessageBox::warning(this,tr("Attention"),tr("SEGY or SU, this is a question."));
    }

    if(groupFunction.checkedButton()==ui->radioConvert)
    {
        convertToFileName=QFileDialog::getSaveFileName(this,tr("Convert To SU files"),"/home/student/Source/Su",
                                                       tr("SU files(*su *Su *SU)"));
    }
    if(groupFunction.checkedButton()==ui->radioFormat)
    {
        convertToFileName=QFileDialog::getSaveFileName(this,tr("Format To New SEGY files"),"/home/student/Source/SEGY",
                                                       tr("SEGY files(*segy *Segy *SEGY *sgy *Sgy *SGY)"));
    }
    ui->lineConvertToFileName->setText(convertToFileName);
}

void MainWindow::depend()
{
    if(ui->lineConvertToFileName->text()!="")
        ui->btnConvert->setEnabled(true);
}
void MainWindow::on_lineConvertToFileName_textChanged(QString )
{
    if(ui->lineConvertToFileName->text()!="")
        ui->btnConvert->setEnabled(true);
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}
void MainWindow::setData()
{
    ui->radioFormat->setChecked(1); //默认转换为segy
    //判断选则的文件类型
    isSegy=false;
    isSu=false;
    QString suffix=openFileName.right(openFileName.count()-openFileName.indexOf(".")-1);

    if(suffix.contains("su",Qt::CaseInsensitive))
        isSu=true;
    if(suffix.contains("segy",Qt::CaseInsensitive))
        isSegy=true;
    if(suffix.contains("sgy",Qt::CaseInsensitive))
        isSegy=true;

    //开始预读
    //SU,无3600
    if(isSu==true)
    {
        headNum=0;
        maxTime=0;
        qDebug()<<"isSU";
        fOpen.seek(headNum);
        /// read>>thTemp;
        read.readRawData((char *)&thTemp,240);  /// 这个肯定是little的,需要转换
        thTemp.swap_header();  /// 如果是big则需要转换

        count=fOpen.size()/(240+thTemp.ns*sizeof(float));  //取得道数
        traceLength=240+thTemp.ns*sizeof(float);    //道长
        maxTime=thTemp.ns*thTemp.dt/1000;
        qDebug()<<thTemp.ns<<thTemp.dt<<count<<"1";

        if(thTemp.ns>15000 || thTemp.dt>15000)
        {
            fOpen.seek(0);
            read.setByteOrder(QJDDataStream::LittleEndian); //BigEndian不对，换一种格式预读
            /// read>>thTemp;
            read.readRawData((char *)&thTemp,240);  /// 这个肯定是little的,需要转换
            //            thTemp.swap_header();  /// 如果不是big则不需要转换

            count=fOpen.size()/(240+thTemp.ns*sizeof(float));
            traceLength=240+thTemp.ns*sizeof(float);
            maxTime=thTemp.ns*thTemp.dt/1000;
            ui->radioOriginalLittleEndian->setChecked(1);
        }
        else
        {
            ui->radioOriginalBigEndian->setChecked(1);
        }
        qDebug()<<thTemp.ns<<thTemp.dt<<count<<"2";

        QMessageBox a;
        a.setText(tr("SU file do not exist <DataFormat> information, the programe will set the IBM for default!"));
        a.exec();
        read.setFormat(QJDDataStream::IBM);     //默认的su格式都是ibm的，自己转换的例外
        ui->radioOriginalIBM->setChecked(1);
        ui->radioIBM->setChecked(1);

        if(groupOriginalByteOrder.checkedButton()==ui->radioOriginalLittleEndian)
        {
            read.setByteOrder(QJDDataStream::LittleEndian);
            ui->radioLittleEndian->setChecked(1);
        }
        if(groupOriginalByteOrder.checkedButton()==ui->radioOriginalBigEndian)
        {
            read.setByteOrder(QJDDataStream::BigEndian);
            ui->radioBigEndian->setChecked(1);
        }
    }

    //SEGY
    if (isSegy==true)
    {
        headNum=3600;
        ui->btnAD->setDisabled(false);
        ui->radioConvert->setDisabled(false);
        count=0;
        maxTime=0;
        fOpen.seek(3200);
        read>>bhTemp;   //这个无所谓改不改
        fOpen.seek(headNum);
        /// read>>thTemp;
        read.readRawData((char *)&thTemp,240); //! 以little读进来
        thTemp.swap_header();  /// 默认是big，所以需要转换
        count=(fOpen.size()-headNum)/(240+thTemp.ns*sizeof(float));  //取得道数
        maxTime=bhTemp.hns*bhTemp.hdt/1000;
        traceLength=240+thTemp.ns*sizeof(float);

        qDebug()<<thTemp.ns<<thTemp.dt;
        //    qDebug()<<maxTime<<count<<traceLength;
        if(bhTemp.hns==0 || bhTemp.hdt==0)
        {
            bhTemp.hns=thTemp.ns;
            bhTemp.hdt=thTemp.dt;
            flagBHis0=true;
        }
        if(thTemp.ns==0 || thTemp.dt==0)
        {
            flagTHis0=true;
        }
        if(bhTemp.hns>30000 || bhTemp.format>10)  //判断非正常的尺度，尺度有时可能需要调整
        {
            qDebug()<<"excute abnormal process";
            read.setByteOrder(QJDDataStream::LittleEndian);
            fOpen.seek(3200);
            read>>bhTemp;   //400
            /// read>>thTemp;    //240
            read.readRawData((char *)&thTemp,240); //! 以little读进来
            count=(fOpen.size()-headNum)/(240+thTemp.ns*sizeof(float));  //取得道数
            maxTime=thTemp.ns*thTemp.dt/1000;
            traceLength=240+thTemp.ns*sizeof(float);
            ui->radioOriginalLittleEndian->setChecked(1);
            if(bhTemp.format==0)
            {
                QMessageBox a;
                a.setText(tr("This file do not exist correct <DataFormat> information, the programe will set the IEEE for default!"));
                ui->radioOriginalIEEE->setChecked(1);
                a.exec();
            }
            qDebug()<<bhTemp.format<<bhTemp.hns<<bhTemp.hdt;
            if(bhTemp.format==1)
                ui->radioOriginalIBM->setChecked(1);
            if(bhTemp.format==5)
                ui->radioOriginalIEEE->setChecked(1);
        }
        else
        {
            ui->radioOriginalBigEndian->setChecked(1);
            if(bhTemp.format==1)
                ui->radioOriginalIBM->setChecked(1);
            if(bhTemp.format==5)
                ui->radioOriginalIEEE->setChecked(1);
        }
        qDebug()<<thTemp.ns<<thTemp.dt;

        //使输出保持一致
        if(groupOriginalFormat.checkedButton()==ui->radioOriginalIEEE)
        {
            read.setFormat(QJDDataStream::IEEE);
            ui->radioIEEE->setChecked(1);
        }
        if(groupOriginalFormat.checkedButton()==ui->radioOriginalIBM)
        {
            read.setFormat(QJDDataStream::IBM);
            ui->radioIBM->setChecked(1);
        }
        if(groupOriginalByteOrder.checkedButton()==ui->radioOriginalLittleEndian)
        {
            read.setByteOrder(QJDDataStream::LittleEndian);
            ui->radioLittleEndian->setChecked(1);
        }
        if(groupOriginalByteOrder.checkedButton()==ui->radioOriginalBigEndian)
        {
            read.setByteOrder(QJDDataStream::BigEndian);
            ui->radioBigEndian->setChecked(1);
        }
    }
}

void MainWindow::resizeEvent()
{}

bool MainWindow::decide(int aa,int bb,int xx)
{
    //qDebug()<<aa<<bb<<xx<<th.dt;
    switch(xx)
    {
    case 0:
        if(th.tracl>=aa && th.tracl<=bb)
            return true;
        else return false;
    case 1:
        if(th.tracr>=aa && th.tracr<=bb)
            return true;
        else return false;
    case 2:
        if(th.fldr>=aa && th.fldr<=bb)
            return true;
        else return false;
    case 3:
        if(th.tracf>=aa && th.tracf<=bb)
            return true;
        else return false;
    case 4:
        if(th.ep>=aa && th.ep<=bb)
            return true;
        else return false;
    case 5:
        if(th.cdp>=aa && th.cdp<=bb)
            return true;
        else return false;
    case 6:
        if(th.cdpt>=aa && th.cdpt<=bb)
            return true;
        else return false;
    case 7:
        if(th.trid>=aa && th.trid<=bb)
            return true;
        else return false;
    case 8:
        if(th.nvs>=aa && th.nvs<=bb)
            return true;
        else return false;
    case 9:
        if(th.nhs>=aa && th.nhs<=bb)
            return true;
        else return false;
    case 10:
        if(th.duse>=aa && th.duse<=bb)
            return true;
        else return false;
    case 11:
        if(th.offset>=aa && th.offset<=bb)
            return true;
        else return false;
    case 12:
        if(th.gelev>=aa && th.gelev<=bb)
            return true;
        else return false;
    case 13:
        if(th.selev>=aa && th.selev<=bb)
            return true;
        else return false;
    case 14:
        if(th.sdepth>=aa && th.sdepth<=bb)
            return true;
        else return false;
    case 15:
        if(th.gdel>=aa && th.gdel<=bb)
            return true;
        else return false;
    case 16:
        if(th.sdel>=aa && th.sdel<=bb)
            return true;
        else return false;
    case 17:
        if(th.swdep>=aa && th.swdep<=bb)
            return true;
        else return false;
    case 18:
        if(th.gwdep>=aa && th.gwdep<=bb)
            return true;
        else return false;
    case 19:
        if(th.scalel>=aa && th.scalel<=bb)
            return true;
        else return false;
    case 20:
        if(th.scalco>=aa && th.scalco<=bb)
            return true;
        else return false;
    case 21:
        if(th.sx>=aa && th.sx<=bb)
            return true;
        else return false;
    case 22:
        if(th.sy>=aa && th.sy<=bb)
            return true;
        else return false;
    case 23:
        if(th.gx>=aa && th.gx<=bb)
            return true;
        else return false;
    case 24:
        if(th.gy>=aa && th.gy<=bb)
            return true;
        else return false;
    case 25:
        if(th.counit>=aa && th.counit<=bb)
            return true;
        else return false;
    case 26:
        if(th.wevel>=aa && th.wevel<=bb)
            return true;
        else return false;
    case 27:
        if(th.swevel>=aa && th.swevel<=bb)
            return true;
        else return false;
    case 28:
        if(th.sut>=aa && th.sut<=bb)
            return true;
        else return false;
    case 29:
        if(th.gut>=aa && th.gut<=bb)
            return true;
        else return false;
    case 30:
        if(th.sstat>=aa && th.sstat<=bb)
            return true;
        else return false;
    case 31:
        if(th.gstat>=aa && th.gstat<=bb)
            return true;
        else return false;
    case 32:
        if(th.tstat>=aa && th.tstat<=bb)
            return true;
        else return false;
    case 33:
        if(th.laga>=aa && th.laga<=bb)
            return true;
        else return false;
    case 34:
        if(th.lagb>=aa && th.lagb<=bb)
            return true;
        else return false;
    case 35:
        if(th.delrt>=aa && th.delrt<=bb)
            return true;
        else return false;
    case 36:
        if(th.muts>=aa && th.muts<=bb)
            return true;
        else return false;
    case 37:
        if(th.mute>=aa && th.mute<=bb)
            return true;
        else return false;
    case 38:
        if(th.ns>=aa && th.ns<=bb)
            return true;
        else return false;
    case 39:
        if(th.dt>=aa && th.dt<=bb)
            return true;
        else return false;
    case 40:
        if(th.gain>=aa && th.gain<=bb)
            return true;
        else return false;
    case 41:
        if(th.igc>=aa && th.igc<=bb)
            return true;
        else return false;
    case 42:
        if(th.igi>=aa && th.igi<=bb)
            return true;
        else return false;
    case 43:
        if(th.corr>=aa && th.corr<=bb)
            return true;
        else return false;
    case 44:
        if(th.sfs>=aa && th.sfs<=bb)
            return true;
        else return false;
    case 45:
        if(th.sfe>=aa && th.sfe<=bb)
            return true;
        else return false;
    case 46:
        if(th.slen>=aa && th.slen<=bb)
            return true;
        else return false;
    case 47:
        if(th.styp>=aa && th.styp<=bb)
            return true;
        else return false;
    case 48:
        if(th.stas>=aa && th.stas<=bb)
            return true;
        else return false;
    case 49:
        if(th.stae>=aa && th.stae<=bb)
            return true;
        else return false;
    case 50:
        if(th.tatyp>=aa && th.tatyp<=bb)
            return true;
        else return false;
    case 51:
        if(th.afilf>=aa && th.afilf<=bb)
            return true;
        else return false;
    case 52:
        if(th.afils>=aa && th.afils<=bb)
            return true;
        else return false;
    case 53:
        if(th.nofilf>=aa && th.nofilf<=bb)
            return true;
        else return false;
    case 54:
        if(th.nofils>=aa && th.nofils<=bb)
            return true;
        else return false;
    case 55:
        if(th.lcf>=aa && th.lcf<=bb)
            return true;
        else return false;
    case 56:
        if(th.hcf>=aa && th.hcf<=bb)
            return true;
        else return false;
    case 57:
        if(th.lcs>=aa && th.lcs<=bb)
            return true;
        else return false;
    case 58:
        if(th.hcs>=aa && th.hcs<=bb)
            return true;
        else return false;
    case 59:
        if(th.year>=aa && th.year<=bb)
            return true;
        else return false;
    case 60:
        if(th.day>=aa && th.day<=bb)
            return true;
        else return false;
    case 61:
        if(th.hour>=aa && th.hour<=bb)
            return true;
        else return false;
    case 62:
        if(th.minute>=aa && th.minute<=bb)
            return true;
        else return false;
    case 63:
        if(th.sec>=aa && th.sec<=bb)
            return true;
        else return false;
    case 64:
        if(th.timbas>=aa && th.timbas<=bb)
            return true;
        else return false;
    case 65:
        if(th.trwf>=aa && th.trwf<=bb)
            return true;
        else return false;
    case 66:
        if(th.grnors>=aa && th.grnors<=bb)
            return true;
        else return false;
    case 67:
        if(th.grnofr>=aa && th.grnofr<=bb)
            return true;
        else return false;
    case 68:
        if(th.grnlof>=aa && th.grnlof<=bb)
            return true;
        else return false;
    case 69:
        if(th.gaps>=aa && th.gaps<=bb)
            return true;
        else return false;
    case 70:
        if(th.otrav>=aa && th.otrav<=bb)
            return true;
        else return false;
    default:
        return false;
    }
}

bool MainWindow::decideInt(int aa,int bb)
{
    if(intx>=aa && intx <=bb)
    {
        return true;
    }
    else
        return false;
}
bool MainWindow::decideQint16(int aa,int bb)
{
    if(qint16x>=aa && qint16x <=bb)
        return true;
    else
        return false;
}
bool MainWindow::decideQuint16(int aa,int bb)
{
    if(quint16x>=aa && quint16x <=bb)
        return true;
    else
        return false;
}
bool MainWindow::decideFloat(int aa,int bb)
{
    if(floatx>=aa && floatx <=bb)
        return true;
    else
        return false;
}

void MainWindow::on_toolAdd_clicked()
{
    rangeTemp = QInputDialog::getText(this, tr("Input"),
                                      tr("Please input the range of the trace: "), QLineEdit::Normal);
    if(rangeTemp=="")       //避免输入空字符串
    {
        return;
    }
    rangeFinal<<rangeTemp;
    rangeFinal.removeDuplicates();       //去除相同字符窜
    /// ListView作显示用
    md->setStringList(rangeFinal);       //!!! 注意，/h声明过后要在这里先new
    ui->listTraceRange->setModel(md);
    ui->listTraceRange->setEditTriggers(QAbstractItemView::AnyKeyPressed
                                        | QAbstractItemView::DoubleClicked);
}

void MainWindow::on_toolRemove_clicked()
{
    left.clear();
    right.clear();
    dataRange.clear();
    rangeFinal.removeAt(ui->listTraceRange->currentIndex().row());
    md->removeRows(ui->listTraceRange->currentIndex().row(), 1);
}

void MainWindow::on_toolTHAdd_clicked()
{
    rangeTempTH = QInputDialog::getText(this, tr("Input"),
                                        tr("Please input the range of the Trace Header Value: "), QLineEdit::Normal);
    if(rangeTempTH=="")       //避免输入空字符串
    {
        return;
    }
    rangeFinalTH<<rangeTempTH;
    rangeFinalTH.removeDuplicates();       //去除相同字符窜
    /// ListView作显示用
    mdTH->setStringList(rangeFinalTH);       //!!! 注意，/h声明过后要在这里先new
    ui->listTraceHeadRange->setModel(mdTH);
    ui->listTraceHeadRange->setEditTriggers(QAbstractItemView::AnyKeyPressed
                                            | QAbstractItemView::DoubleClicked);
}

void MainWindow::on_toolTHRemove_clicked()
{
    leftTH.clear();
    rightTH.clear();
    dataRangeTH.clear();
    rangeFinalTH.removeAt(ui->listTraceHeadRange->currentIndex().row());
    mdTH->removeRows(ui->listTraceHeadRange->currentIndex().row(), 1);
}

void MainWindow::on_toolAddTime_clicked()
{
    rangeTempTC = QInputDialog::getText(this, tr("Input"),
                                        tr("Please input the range of the Time Scale: "), QLineEdit::Normal);
    if(rangeTempTC=="")       //避免输入空字符串
    {
        return;
    }
    rangeFinalTC<<rangeTempTC;
    rangeFinalTC.removeDuplicates();       //去除相同字符窜
    /// ListView作显示用
    mdTC->setStringList(rangeFinalTC);       //!!! 注意，/h声明过后要在这里先new
    ui->listTimeRange->setModel(mdTC);
    ui->listTimeRange->setEditTriggers(QAbstractItemView::AnyKeyPressed
                                       | QAbstractItemView::DoubleClicked);
}

void MainWindow::on_toolRemoveTime_clicked()
{
    leftTC.clear();
    rightTC.clear();
    dataRangeTC.clear();
    rangeFinalTC.removeAt(ui->listTimeRange->currentIndex().row());
    mdTC->removeRows(ui->listTimeRange->currentIndex().row(), 1);
}


void MainWindow::on_radioStandard_clicked()
{
    ui->label_9->show();
    ui->combo240->show();
    ui->label_10->show();
    ui->labelReference->show();

    ui->label_23->hide();
    ui->lineStPos->hide();
    ui->label_12->hide();
    ui->label_24->hide();
    ui->comboDataType->hide();
    ui->labelBlank->hide();
    ui->labelBlank2->hide();
    ui->labelBlank3->hide();

    if(ui->checkStandard->isChecked() && ui->radioStandard->isChecked())          // 标准
    {
        flagStandard=true;
        flagNonStandard=false;
    }
}

void MainWindow::on_radioNonStandard_clicked()
{
    ui->label_9->hide();
    ui->combo240->hide();
    ui->label_10->hide();
    ui->labelReference->hide();

    ui->label_23->show();
    ui->lineStPos->show();
    ui->label_12->show();
    ui->label_24->show();
    ui->comboDataType->show();
    ui->labelBlank->show();
    ui->labelBlank2->show();
    ui->labelBlank3->show();


    if(ui->checkStandard->isChecked() && ui->radioNonStandard->isChecked())       //不标准
    {
        flagNonStandard=true;
        flagStandard=false;
    }
}

void MainWindow::on_checkSkip_clicked(bool checked)
{
    ui->spinSkip->setVisible(checked);
    if(checked)       //取道
    {
        flagSkip=true;
        ui->spinSkip->setMaximum(count);
    }

    if(checked==false)
    {
        flagSkip=false;
    }
}

void MainWindow::on_checkStandard_clicked(bool checked)
{
    if(checked==false)
    {
        s180.clear();
        ui->combo240->clear();
        ui->comboDataType->clear();
        dataType.clear();
        flagStandard=false;
        flagNonStandard=false;
    }
    if(checked==true)
    {
        s180.clear();
        ui->combo240->clear();
        s180<<"tracl"<<"tracr"<<"fldr"<<"tracf"<<"ep"<<"cdp"<<"cdpt"
                <<"trid"<<"nvs"<<"nhs"<<"duse"<<"offset"<<"gelev"<<"selev"
                <<"sdepth"<<"gdel"<<"sdel"<<"swdep"<<"gwdep"<<"scalel"
                <<"scalco"<<"sx"<<"sy"<<"gx"<<"gy"<<"counit"<<"wevel"
                <<"swevel"<<"sut"<<"gut"<<"sstat"<<"gstat"<<"tstat"<<"laga"
                <<"lagb"<<"delrt"<<"muts"<<"mute"<<"ns"<<"dt"<<"gain"<<"igc"
                <<"igi"<<"corr"<<"sfs"<<"sfe"<<"slen"<<"styp"<<"stas"<<"stae"
                <<"tatyp"<<"afilf"<<"afils"<<"nofilf"<<"nofils"<<"lcf"<<"hcf"
                <<"lcs"<<"hcs"<<"year"<<"day"<<"hour"<<"minute"<<"sec"<<"timbas"
                <<"trwf"<<"grnors"<<"grnofr"<<"grnlof"<<"gaps"<<"otrav";
        for(int i=0;i<s180.size();i++)
        {
            ui->combo240->insertItem(i,s180[i]);
        }

        ui->comboDataType->clear();
        dataType.clear();
        dataType<<tr("int")<<tr("short int")<<tr("float")<<tr("unsigned short int");
        for(int i=0;i<dataType.size();i++)
        {
            ui->comboDataType->insertItem(i,dataType[i]);
        }

        if(ui->radioStandard->isChecked())
        {
            flagStandard=true;
            flagNonStandard=false;
        }
        if(ui->radioNonStandard->isChecked())
        {
            flagNonStandard=true;
            flagStandard=false;
        }
    }
}

void MainWindow::on_checkResample_clicked(bool checked)
{
    if(checked && ui->checkResample->isChecked())       //采样间隔
    {
        ui->comboInterval->clear();
        QString temp=QString::number(thTemp.dt);
        ui->labelCurrentInterval->setText(temp);
        ui->labelMaxTime->setText(QString::number(maxTime));

        for(int i=2;i<=4;i++)
        {
            int b=thTemp.dt*i;
            QString a=QString::number(b);
            ui->comboInterval->addItem(a);
        }

        QString aa=QString::number(thTemp.dt);
        ui->comboInterval->addItem(aa);
        ui->comboInterval->setCurrentIndex(3);

        flagResample=true;
    }
    if(checked==false)
    {
        ui->comboInterval->clear();
        flagResample=false;
    }
}

void MainWindow::on_checkTraceScope_clicked(bool checked)
{
    if(checked)
    {
        flagTraceScope=true;
        ui->labelMaxTrace->setText(QString::number(count));
    }
    if(checked==false)
    {
        flagTraceScope=false;
    }
}

void MainWindow::on_checkAcResample_clicked(bool checked)
{
    if(checked)
    {
        flagAcResample=true;
    }
    if(checked==false)
    {
        flagAcResample=false;
    }
}

void MainWindow::on_btnConvert_clicked()        //最终点击转换按钮
{
    setInfo();      //设置相关信息
    checkInfo();    //检查相关输入
    if(flagSetInfo==false)      //设置信息不成功，则在此就退出
        return;

    /// 单单转换成SU
    if(ui->radioConvert->isChecked() && flagStandard==false && flagNonStandard==false
       && flagResample==false && flagTraceScope==false && flagSkip==false )
    {
        convert2SU();
    }

    /// 单单转换成SEGY，没有附加条件
    if(ui->radioFormat->isChecked() && flagStandard==false && flagNonStandard==false
       && flagResample==false && flagTraceScope==false && flagSkip==false )
    {
        convert2SEGY();
    }

    /// 开始非标准转换
    if(flagStandard==true)
    {
        qDebug()<<"st";
        convertStandard();      //标准道头情况下的所有情况
    }
    if(flagNonStandard==true)
    {
        qDebug()<<"nst";
        convertNonStandard();       //不标准道头情况下的所有情况
    }
    if(flagTraceScope==true && ui->checkStandard->isChecked()==false)
    {
        qDebug()<<"ts";
        convertTraceScope();        //排除道头的取道情况
    }
    if(flagResample==true && ui->checkStandard->isChecked()==false && flagTraceScope==false)
    {
        qDebug()<<"resample";
        convertResample();      //排除道头和取道的重采样
    }
    if(flagSkip==true && ui->checkStandard->isChecked()==false && flagTraceScope==false && flagResample==false)
    {
        qDebug()<<"skip";
        convertSkipTrace();     //排除所有情况的Trace Resample
    }
    qDebug()<<"convert to : "<<convertToFileName;

    lastClear();
    flagClear();
}

void MainWindow::setInfo()  //Start 点击后，第一步，拾取信息
{
    if(ui->radioConvert->isChecked())
    {
        flagSU=true;
    }

    convertToFileName=ui->lineConvertToFileName->text();
    /// 打开被转换的文件
    if(!fConvert.isOpen())
    {
        fConvert.setFileName(convertToFileName);
        fConvert.open(QIODevice::WriteOnly);
    }
    write.setDevice(&fConvert);
    if(groupFormat.checkedButton()==0 || groupByteOrder.checkedButton()==0)     //判断选择了标准或非标准否
    {
        QMessageBox::warning(this,tr("Attention"),tr("The format and the byte order must be choosed!"));
        flagSetInfo=false;
        return;
    }

    if(groupFormat.checkedButton()==ui->radioIEEE)      //设置目标文件的格式,可以一次性统一设好
    {
        write.setFormat(QJDDataStream::IEEE);
    }
    if(groupFormat.checkedButton()==ui->radioIBM)
    {
        write.setFormat(QJDDataStream::IBM);
    }
    if(groupByteOrder.checkedButton()==ui->radioLittleEndian)
    {
        write.setByteOrder(QJDDataStream::LittleEndian);
    }
    if(groupByteOrder.checkedButton()==ui->radioBigEndian)
    {
        write.setByteOrder(QJDDataStream::BigEndian);
    }

    QMessageBox msg;
    /// ----------------------------------------------取得信息------------------------------------------------///
    /// 道头通用范围
    if(ui->checkStandard->isChecked())
    {
        leftTH.clear();
        rightTH.clear();
        leftNumTH.clear();
        rightNumTH.clear();
        dataRangeTH.clear();
        rangeFinalTH.clear();
        rangeFinalTH=mdTH->stringList();    //将数据清空

        QString tempTH;
        int tempLeftTH;
        int tempRightTH;
        QString too="~";        // 以～符号做为范围之间的符号
        QString too2="-";

        for(int i=0;i<rangeFinalTH.count();i++)       //将QStringList中的信息导出到QString的容器中
        {
            dataRangeTH<<rangeFinalTH.at(i);
        }
        for(int i=0;i<dataRangeTH.count();i++)
        {
            tempTH=dataRangeTH[i];
            if(tempTH.contains(too))
            {
                leftTH<<tempTH.left(tempTH.indexOf(too));      //如果用户没有输入”～“，则自动判断为left=right=temp
                rightTH<<tempTH.right(tempTH.count()-tempTH.indexOf(too)-1);
            }
            if(tempTH.contains(too2))
            {
                leftTH<<tempTH.left(tempTH.indexOf(too2));      //如果用户没有输入”～“，则自动判断为left=right=temp
                rightTH<<tempTH.right(tempTH.count()-tempTH.indexOf(too2)-1);
            }
            if(!tempTH.contains(too) && !tempTH.contains(too2))
            {
                leftTH<<tempTH;
                rightTH<<tempTH;
            }
            if(leftTH[i].toInt()<0 || rightTH[i].toInt()<0 || rightTH[i].toInt()<leftTH[i].toInt())     //让用户自查输入不规范的地方
            {
                dataRangeTH.clear();
                leftTH.clear();
                rightTH.clear();

                QMessageBox m;
                m.setText(tr("Please Check the Input, make sure after the fashion of Example "));
                m.exec();
                flagSetInfo=false;
                return;
            }
        }
        leftNumTH.resize(leftTH.size());
        rightNumTH.resize(rightTH.size());
        for(int i=0;i<leftTH.size();i++)          //QString ---> int
        {
            leftNumTH[i]=leftTH[i].toInt();
            rightNumTH[i]=rightTH[i].toInt();
        }

        for(int i=0;i<leftNumTH.size();i++)      //对输入的范围进行排序，以求从前到后读取数据
        {                                                /// 注意：不要用QString 来排序
            for(int j=i+1;j<leftNumTH.size();j++)
            {
                if(leftNumTH[i]>leftNumTH[j])
                {
                    tempLeftTH=leftNumTH[i];
                    leftNumTH[i]=leftNumTH[j];
                    leftNumTH[j]=tempLeftTH;

                    tempRightTH=rightNumTH[i];
                    rightNumTH[i]=rightNumTH[j];
                    rightNumTH[j]=tempRightTH;
                }
            }
        }
        //qDebug()<<leftNumTH<<rightNumTH;
        for(int i=0;i<leftNumTH.size()-1;i++)
        {
            if(leftNumTH[i+1]<=rightNumTH[i])
            {
                msg.setText("The number input have repetition. Please check out");
                msg.exec();
                flagSetInfo=false;
                return;
            }
        }
    }

    /// 标准道头
    if(flagStandard==true)
    {
        SCI=ui->combo240->currentIndex();       //选择的道头
    }

    /// 非标准道头
    if(flagNonStandard==true)
    {
        NSCI=ui->comboDataType->currentIndex();     //数据类型
        seekPos=ui->lineStPos->text().toInt();
        seekPos=seekPos-1;      //保证从1开始，1～240

        if(seekPos<0 || seekPos>239)
        {
            msg.setText(tr("The position you type must be in range 1~240"));
            msg.exec();
            flagSetInfo=false;
            return;
        }
    }

    /// 重采样
    if(flagResample==true)
    {
        nowDt=ui->comboInterval->currentText().toInt();       //用户选择的转换间隔
        skipNum=ui->comboInterval->currentIndex()+1;        //将索引值加1，直接做为读取数据时跳过的数据量

        if(skipNum==3)      //索引3是原始采样间隔
            skipNum=0;      //保持原样

        if(ui->comboInterval->currentIndex()!=3 && flagAcResample==true)        //处理特殊情况
            flagResampleChange=true;
        else
            flagResampleChange=false;
        /// ///////////////////////////////////
        leftTC.clear();
        rightTC.clear();
        leftNumTC.clear();
        rightNumTC.clear();
        leftNumTCReal.clear();
        rightNumTCReal.clear();
        rightNumTCMinus.clear();
        dataRangeTC.clear();
        rangeFinalTC.clear();
        allPointNum=0;
        rangeFinalTC=mdTC->stringList();    //将数据清空

        QString tempTC;
        int tempLeftTC;
        int tempRightTC;
        QString too="~";        // 以～符号做为范围之间的符号
        QString too2="-";

        for(int i=0;i<rangeFinalTC.count();i++)       //将QStringList中的信息导出到QString的容器中
        {
            dataRangeTC<<rangeFinalTC.at(i);
        }
        for(int i=0;i<dataRangeTC.count();i++)
        {
            tempTC=dataRangeTC[i];
            if(tempTC.contains(too))
            {
                leftTC<<tempTC.left(tempTC.indexOf(too));      //如果用户没有输入”～“，则自动判断为left=right=temp
                rightTC<<tempTC.right(tempTC.count()-tempTC.indexOf(too)-1);
            }
            if(tempTC.contains(too2))
            {
                leftTC<<tempTC.left(tempTC.indexOf(too2));      //如果用户没有输入”-“，则自动判断为left=right=temp
                rightTC<<tempTC.right(tempTC.count()-tempTC.indexOf(too2)-1);
            }
            if(!tempTC.contains(too) && !tempTC.contains(too2))
            {
                leftTC<<tempTC;
                rightTC<<tempTC;
            }
            if(leftTC[i].toInt()<0 || rightTC[i].toInt()<0 || rightTC[i].toInt()<leftTC[i].toInt())     //让用户自查输入不规范的地方
            {
                dataRangeTC.clear();
                leftTC.clear();
                rightTC.clear();

                QMessageBox m;
                m.setText(tr("Please Check the Input, make sure after the fashion of Example "));
                m.exec();
                flagSetInfo=false;
                return;
            }
        }
        leftNumTC.resize(leftTC.size());
        rightNumTC.resize(rightTC.size());
        for(int i=0;i<leftTC.size();i++)          //QString ---> int
        {
            leftNumTC[i]=leftTC[i].toInt();
            rightNumTC[i]=rightTC[i].toInt();
        }

        for(int i=0;i<leftNumTC.size();i++)      //对输入的范围进行排序，以求从前到后读取数据
        {                                                /// 注意：不要用QString 来排序
            for(int j=i+1;j<leftNumTC.size();j++)
            {
                if(leftNumTC[i]>leftNumTC[j])
                {
                    tempLeftTC=leftNumTC[i];
                    leftNumTC[i]=leftNumTC[j];
                    leftNumTC[j]=tempLeftTC;

                    tempRightTC=rightNumTC[i];
                    rightNumTC[i]=rightNumTC[j];
                    rightNumTC[j]=tempRightTC;
                }
            }
        }

        leftNumTCReal.resize(leftTC.size());
        rightNumTCReal.resize(rightTC.size());
        rightNumTCMinus<<1;
        newTraceNum=0;
        for(int i=0;i<leftNumTC.size();i++)          //采样时间转换为实际的第几个数据点
        {
            leftNumTCReal[i]=(int)(leftNumTC[i]*1000/thTemp.dt);
            rightNumTCReal[i]=(int)(ceil(rightNumTC[i]*1000/thTemp.dt));
            rightNumTCMinus<<rightNumTCReal[i]+1;   //确保不将需要的读进去
            newTraceNum=newTraceNum+(rightNumTCReal[i]-leftNumTCReal[i])+1;
        }
        qDebug()<<leftNumTCReal<<rightNumTCReal;        //暂时没啥问题

        /// 特殊情况
        loopNum.resize(leftNumTCReal.size());
        if(flagResampleChange==true)
        {
            for(int i=0;i<leftNumTCReal.size();i++)
            {
                //计算每段落的循环次数
                loopNum[i]=(int)((rightNumTCReal[i]-leftNumTCReal[i]+1)/(ui->comboInterval->currentIndex()+2)); //实际个数除以重采样得到新的采样个数
                //计算总得个数，用来更新th.ns
                allPointNum=allPointNum+loopNum[i];
            }
        }


        for(int i=0;i<leftNumTC.size()-1;i++)
        {
            if(leftNumTC[i+1]<=rightNumTC[i])
            {
                msg.setText(tr("The number input have repetition. Please check out"));
                msg.exec();
                flagSetInfo=false;
                return;
            }
        }

        for(int i=0;i<rightNumTC.size();i++)
        {
            if(rightNumTC[i]>maxTime)
            {
                QMessageBox::information(this,tr("Over Range"),tr("Max Time Over the range!"));
                flagSetInfo=false;
                return;
            }
        }
    }

    /// 取道信息
    if(flagTraceScope==true)
    {
        left.clear();
        right.clear();
        leftNum.clear();
        rightNum.clear();
        dataRange.clear();
        rangeFinal.clear();
        rangeFinal=md->stringList();    //将数据清空

        QString temp;
        int tempLeft;
        int tempRight;
        QString too="~";        // 以～符号做为范围之间的符号
        QString too2="-";

        for(int i=0;i<rangeFinal.count();i++)       //将QStringList中的信息导出到QString的容器中
        {
            dataRange<<rangeFinal.at(i);
        }
        for(int i=0;i<dataRange.count();i++)
        {
            temp=dataRange[i];
            if(temp.contains(too))
            {
                left<<temp.left(temp.indexOf(too));
                right<<temp.right(temp.count()-temp.indexOf(too)-1);
            }
            if(temp.contains(too2))
            {
                left<<temp.left(temp.indexOf(too2));
                right<<temp.right(temp.count()-temp.indexOf(too2)-1);
            }
            if(!temp.contains(too) && !temp.contains(too2))     //如果用户没有输入”-“"~"，则判断为left=right=temp
            {
                left<<temp;
                right<<temp;
            }
            if(left[i].toInt()<0 || right[i].toInt()<0 || right[i].toInt()<left[i].toInt()
                || left[i].toInt()>count || right[i].toInt()>count)     //让用户自查输入不规范的地方
                {
                dataRange.clear();
                left.clear();
                right.clear();

                QMessageBox m;
                m.setText(tr("Please Check the Trace number, make sure after the fashion of Example and the MaxValue is under the max Trace "));
                m.exec();
                flagSetInfo=false;
                return;
            }
        }
        leftNum.resize(left.size());
        rightNum.resize(right.size());
        for(int i=0;i<left.size();i++)          //QString ---> int
        {
            leftNum[i]=left[i].toInt();
            rightNum[i]=right[i].toInt();
        }

        for(int i=0;i<leftNum.size();i++)      //对输入的范围进行排序，以求从前到后读取数据
        {                                                /// 不要用QString 来排序
            for(int j=i+1;j<leftNum.size();j++)
            {
                if(leftNum[i]>leftNum[j])
                {
                    tempLeft=leftNum[i];
                    leftNum[i]=leftNum[j];
                    leftNum[j]=tempLeft;

                    tempRight=rightNum[i];
                    rightNum[i]=rightNum[j];
                    rightNum[j]=tempRight;
                }
            }
        }
        for(int i=0;i<leftNum.size()-1;i++)
        {
            if(leftNum[i+1]<=rightNum[i])
            {
                msg.setText(tr("The number input have repetition. Please check out"));
                msg.exec();
                flagSetInfo=false;
                qDebug()<<leftNum<<rightNum;

                return;
            }
        }
    }

    /// 隔道取道
    if(ui->checkSkip->isChecked())
    {
        skipTrace=ui->spinSkip->value();

        if(flagTraceScope==true)
        {
            for(int i=0;i<leftNum.size();i++)           //确定循环次数
            {
                skipLoop<<(rightNum[i]-leftNum[i]+1)/(skipTrace+1);
            }
            qDebug()<<skipLoop;
        }
    }
    flagSetInfo=true;
}

void MainWindow::checkInfo()
{
    /// 检查输入的文件名
    if(ui->radioFormat->isChecked())
    {
        QString a=ui->lineConvertToFileName->text();
        QString b=a.left(a.lastIndexOf("/"));
        QString dot=".";
        QString suffix;
        if(a.contains(dot)==false)
        {
            QMessageBox::warning(this,tr("Attention"),tr("The output file name must have a Suffix!"));
            flagSetInfo=false;
        }
        suffix=a.right(a.count()-a.indexOf(dot)-1);
        if(suffix.contains("segy",Qt::CaseInsensitive)==false && suffix.contains("sgy",Qt::CaseInsensitive)==false)
        {
            QMessageBox::warning(this,tr("Attention"),tr("The output file name must be a SEGY file!"));
            flagSetInfo=false;
        }
        QDir dir(b);
        if (!dir.exists())
        {
            QMessageBox::warning(this,tr("Error"),tr("Cannot find the directory, check your input."));
            flagSetInfo=false;
        }
    }

    if(ui->radioConvert->isChecked())
    {
        QString a=ui->lineConvertToFileName->text();
        QString b=a.left(a.lastIndexOf("/"));
        QString dot=".";
        QString suffix;
        if(a.contains(dot)==false)
        {
            QMessageBox::warning(this,tr("Attention"),tr("The output file name must have a Suffix!"));
            flagSetInfo=false;
        }
        suffix=a.right(a.count()-a.indexOf(dot)-1);
        if(suffix.contains("su",Qt::CaseInsensitive)==false)
        {
            QMessageBox::warning(this,tr("Attention"),tr("The output file name must be a SU file!"));
            flagSetInfo=false;
        }
        QDir dir(b);
        if (!dir.exists())
        {
            QMessageBox::warning(this,tr("Error"),tr("Cannot find the directory, check your input."));
            flagSetInfo=false;
        }
    }

    /// 检查取道范围
    if(ui->checkTraceScope->isChecked())
    {
        if(leftNum.size()==0)       //没有填写，则默认为取最大值
        {
            leftNum<<1;
            rightNum<<count;
        }
    }
    /// 检查道头选择和取值范围
    if(ui->checkStandard->isChecked() && ui->radioStandard->isChecked())
    {
        if(leftNumTH.size()==0)
        {
            flagStandardDefault=true;
        }
    }
    if(ui->checkStandard->isChecked() && ui->radioNonStandard->isChecked())
    {
        if(leftNumTH.size()==0)
        {
            flagNonStandardDefault=true;
        }
    }

}


void  MainWindow::convert2SU()
{
    flagConvertComplete=false;
    float temp;
    QProgressDialog progress(tr("Converting files..."), tr("Abort"), 0,count-1, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(1000);
    QMessageBox msgBox;

    qDebug()<<"covertsu"<<isSu<<isSegy;
    if(isSegy==true)
    {
        fOpen.seek(headNum);
    }
    if(isSu==true)
    {
        qDebug()<<"seek 3600";
        fOpen.seek(headNum);
    }
    int j;
    for(j=0;j<count;j++)
    {
        read.readRawData((char *)&th,240);
        int saveNS;
        if(ui->radioOriginalBigEndian->isChecked()==true)
        {
            qDebug()<<"originbig";
            th.swap_header();
        }
        saveNS=th.ns;
        //        qDebug()<<th.ns;
        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
        {
            QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
            QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
        }
        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
        {
            QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
            QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
        }
        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
        {
            th.swap_header();
        }
        write.writeRawData((char *)&th,240);
        for(int i=0;i<saveNS;i++)
        {
            read.readRawData((char *)&temp,4);
            if(ui->radioOriginalBigEndian->isChecked()==true)
            {
                QJD::qjdswap_float_4(&temp);
            }
            // 转换ieee/ibm
            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
            {
                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
            }
            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
            {
                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
            }
            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
            {
                QJD::qjdswap_float_4(&temp);
            }
            write.writeRawData((char *)&temp,4);
        }
        progress.setValue(j);
        if (progress.wasCanceled())
        {
            msgBox.setText(tr("Convert has been terminated !"));
            msgBox.exec();
            break;
        }
    }
    if(j==count)
    {
        flagConvertComplete=true;
    }
}

void MainWindow::convert2SEGY()
{
    flagConvertComplete=false;

    //添加3600
    if(isSu==true)
    {
        bty.resize(3201);       //切勿忘记开空间，后果不堪设想
        *(bty.data()+3200)='\0';
        write.writeRawData(bty.data(),3200);
        bhTemp.hns=thTemp.ns;
        bhTemp.hdt=thTemp.dt;
        if(groupFormat.checkedButton()==ui->radioIEEE)      //设置目标文件的格式,可以一次性统一设好
        {
            bhTemp.format=5;
        }
        if(groupFormat.checkedButton()==ui->radioIBM)
        {
            bhTemp.format=1;
        }

        write<<bh;

        fOpen.seek(0);      //转移到道头准备转换
    }
    if(isSegy==true)
    {
        read3200();
        read>>bh;

        if(ui->radioIBM->isChecked())
            bh.format=1;
        else if(ui->radioIEEE->isChecked())
            bh.format=5;

        write<<bh;

    }
    int z;
    float temp;
    QProgressDialog progress(tr("Converting files..."), tr("Abort"), 0,count-1, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(1000);
    for(z=0;z<count;z++)
    {
        read.readRawData((char *)&th,240);
        // 转换little/big
        int saveNS;
        if(ui->radioOriginalBigEndian->isChecked()==true)
        {
            th.swap_header();
        }
        saveNS=th.ns;
        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
        {
            QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
            QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
        }
        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
        {
            QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
            QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
        }
        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
        {
            th.swap_header();
        }
        write.writeRawData((char *)&th,240);

        /// data
        for(int i=0;i<saveNS;i++)
        {
            read.readRawData((char *)&temp,4);
            if(ui->radioOriginalBigEndian->isChecked()==true)
            {
                QJD::qjdswap_float_4(&temp);
            }
            // 转换ieee/ibm
            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
            {
                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
            }
            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
            {
                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
            }
            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
            {
                QJD::qjdswap_float_4(&temp);
            }
            write.writeRawData((char *)&temp,4);
        }
        progress.setValue(z);
        if (progress.wasCanceled())
        {
            QMessageBox::warning(this,"Abort","Format has been terminated !");
            break;
        }
    }
    if(z==count)
    {
        flagConvertComplete=true;
    }

}

/// 在正常道头取值的情况下的所有组合
void MainWindow::convertStandard()
{
    flagConvertComplete=false;
    float temp;               //整个函数通用，非常重要
    float skipTemp;         //跳过一部分的数据的临时存放

    //read3200,read400都包含了原始为su的处理方法
    //3200
    read3200();

    //400
    read400();

    // 240+数据部分
    /// 有取道功能的情况下
    if(flagTraceScope==true)
    {
        int number;
        for(number=0;number<leftNum.size();number++)
        {
            fOpen.seek(headNum+(leftNum[number]-1)*traceLength);       //只需偏移到指定的地方，读取指定数据，再进行偏移即可
            qDebug()<<flagSkip;

            if(flagSkip==false)
            {
                QProgressDialog p1(tr("Converting..."), tr("Abort"), 0,rightNum[number]-leftNum[number], this);
                p1.setWindowModality(Qt::WindowModal);
                p1.setMinimumDuration(1000);

                qDebug()<<rightNum[number]-leftNum[number]+1;
                for(int j=0;j<rightNum[number]-leftNum[number]+1;j++)       //读取需要的道
                {
                    fOpen.seek(headNum+(leftNum[number]-1)*traceLength+j*traceLength);

                    /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                    read.readRawData((char *)&th,240);
                    int saveNS;
                    int saveDT;
                    if(ui->radioOriginalBigEndian->isChecked()==true)
                    {
                        th.swap_header();
                    }
                    saveNS=th.ns;
                    saveDT=th.dt;

                    if(flagStandardDefault==false)
                    {
                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decide(leftNumTH[a],rightNumTH[a],SCI)==true)
                            {
                                flagStandardUse=true;
                                break;
                            }
                            else flagStandardUse=false;
                        }
                    }
                    if(flagStandardDefault==true)
                        flagStandardUse=true;

                    if(flagStandardUse==true)
                    {
                        if(flagResample==true)
                        {
                            th.ns=bh.hns;
                            th.dt=bh.hdt;
                            saveNS=th.ns;
                            saveDT=th.dt;
                        }
                        /// write<<th;
                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                        {
                            QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                            QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                        }
                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                        {
                            QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                            QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                        }
                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                        {
                            th.swap_header();
                        }
                        write.writeRawData((char *)&th,240);
                    }

                    if(flagResample==false)
                    {
                        for(int i=0;i<saveNS;i++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            if(flagStandardUse==true)
                            {
                                /// write<<temp;
                                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                {
                                    QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                {
                                    QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                write.writeRawData((char *)&temp,4);
                            }
                        }
                    }
                    if(flagResample==true)
                    {
                        if(flagAcResample==false)               //不同的读写数据方式
                        {                                                 //正常读写
                            for(int b=0;b<saveNS;b++)
                            {
                                /// read>>temp;
                                read.readRawData((char *)&temp,4);
                                if(ui->radioOriginalBigEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                /// write<<temp;
                                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                {
                                    QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                {
                                    QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                write.writeRawData((char *)&temp,4);
                                for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                {
                                    /// read>>skipTemp;
                                    read.readRawData((char *)&skipTemp,4);  //彻底舍弃流
                                }
                            }
                        }
                        if(flagAcResample==true)
                        {                                                 //精确读写
                            for(int b=0;b<leftNumTCReal.size();b++)
                            {
                                for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                                {
                                    /// read>>temp;            //跳过不需要
                                    read.readRawData((char *)&temp,4);
                                }
                                if(flagResampleChange==false)
                                {
                                    for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                    {
                                        /// read>>temp;
                                        read.readRawData((char *)&temp,4);
                                        if(ui->radioOriginalBigEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        /// write<<temp;
                                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                        {
                                            QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                        {
                                            QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        write.writeRawData((char *)&temp,4);
                                    }
                                }
                                if(flagResampleChange==true)        //非常特殊的情况
                                {
                                    for(int d=0;d<loopNum[b];d++)
                                    {
                                        /// read>>temp;
                                        read.readRawData((char *)&temp,4);
                                        if(ui->radioOriginalBigEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        /// write<<temp;
                                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                        {
                                            QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                        {
                                            QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        write.writeRawData((char *)&temp,4);
                                        for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                        {
                                            read.readRawData((char *)&skipNum,4);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    p1.setValue(j);
                    if (p1.wasCanceled())
                    {
                        QMessageBox::warning(this,tr("Attention"),tr("Convert has been terminated !"));
                        break;
                    }
                }
            }

            if(flagSkip==true)
            {
                QProgressDialog p1(tr("Converting..."), tr("Abort"), 0,skipLoop[number], this);
                p1.setWindowModality(Qt::WindowModal);
                p1.setMinimumDuration(1000);

                for(int j=0;j<skipLoop[number];j++)       //读取需要的道
                {
                    fOpen.seek(headNum+(leftNum[number]-1)*traceLength+j*(skipTrace+1)*traceLength);
                    /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                    read.readRawData((char *)&th,240);
                    int saveNS;
                    int saveDT;
                    if(ui->radioOriginalBigEndian->isChecked()==true)
                    {
                        th.swap_header();
                    }
                    saveNS=th.ns;
                    saveDT=th.dt;

                    if(flagStandardDefault==false)
                    {
                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decide(leftNumTH[a],rightNumTH[a],SCI)==true)
                            {
                                flagStandardUse=true;
                                break;
                            }
                            else flagStandardUse=false;
                        }
                    }
                    if(flagStandardDefault==true)
                        flagStandardUse=true;

                    if(flagStandardUse==true)
                    {
                        if(flagResample==true)
                        {
                            th.ns=bh.hns;
                            th.dt=bh.hdt;
                            saveNS=th.ns;
                            saveDT=th.dt;
                        }
                        /// write<<th;
                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                        {
                            QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                            QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                        }
                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                        {
                            QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                            QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                        }
                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                        {
                            th.swap_header();
                        }
                        write.writeRawData((char *)&th,240);
                    }

                    if(flagResample==false)
                    {
                        for(int i=0;i<saveNS;i++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            if(flagStandardUse==true)
                            {
                                /// write<<temp;
                                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                {
                                    QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                {
                                    QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                write.writeRawData((char *)&temp,4);
                            }
                        }
                    }
                    if(flagResample==true)
                    {
                        if(flagAcResample==false)               //不同的读写数据方式
                        {                                                 //正常读写
                            for(int b=0;b<saveNS;b++)
                            {
                                /// read>>temp;
                                read.readRawData((char *)&temp,4);
                                if(ui->radioOriginalBigEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                /// write<<temp;
                                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                {
                                    QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                {
                                    QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                write.writeRawData((char *)&temp,4);
                                for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                {
                                    read.readRawData((char *)&skipNum,4);
                                }
                            }
                        }
                        if(flagAcResample==true)
                        {                                                 //精确读写
                            for(int b=0;b<leftNumTCReal.size();b++)
                            {
                                for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                                {
                                    read.readRawData((char *)&temp,4);
                                }
                                if(flagResampleChange==false)
                                {
                                    for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                    {
                                        /// read>>temp;
                                        read.readRawData((char *)&temp,4);
                                        if(ui->radioOriginalBigEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        /// write<<temp;
                                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                        {
                                            QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                        {
                                            QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        write.writeRawData((char *)&temp,4);
                                    }
                                }
                                if(flagResampleChange==true)        //非常特殊的情况
                                {
                                    for(int d=0;d<loopNum[b];d++)
                                    {
                                        /// read>>temp;
                                        read.readRawData((char *)&temp,4);
                                        if(ui->radioOriginalBigEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        /// write<<temp;
                                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                        {
                                            QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                        {
                                            QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        write.writeRawData((char *)&temp,4);
                                        for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                        {
                                            /// read>>skipTemp;
                                            read.readRawData((char *)&skipTemp,4);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    p1.setValue(j);
                    if (p1.wasCanceled())
                    {
                        QMessageBox::warning(this,tr("Attention"),tr("Convert has been terminated !"));
                        break;
                    }
                }
            }
        }
        if(number==leftNum.size())
        {
            flagConvertComplete=true;
            //QMessageBox::information(this,tr("Complete"),tr("Convert Complete."));
        }
    }

    /// 在没有取道的情况下
    if(flagTraceScope==false)
    {
        int z;
        if(flagSkip==false)
        {
            QProgressDialog p1(tr("Converting..."), tr("Abort"), 0,count-1, this);
            p1.setWindowModality(Qt::WindowModal);
            p1.setMinimumDuration(1000);

            for(z=0;z<count;z++)
            {
                fOpen.seek(headNum+z*traceLength);

                /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                read.readRawData((char *)&th,240);
                int saveNS;
                int saveDT;

                if(ui->radioOriginalBigEndian->isChecked()==true)
                {
                    th.swap_header();
                }
                saveNS=th.ns;
                saveDT=th.dt;

                if(flagStandardDefault==false)
                {
                    for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                    {
                        if(decide(leftNumTH[a],rightNumTH[a],SCI)==true)
                        {
                            flagStandardUse=true;
                            break;
                        }
                        else flagStandardUse=false;
                    }
                }
                if(flagStandardDefault==true)
                    flagStandardUse=true;

                if(flagStandardUse==true)
                {
                    if(flagResample==true)
                    {
                        th.ns=bh.hns;
                        th.dt=bh.hdt;
                        saveNS=th.ns;
                        saveDT=th.dt;
                    }
                    /// write<<th;
                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                    {
                        QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                        QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                    }
                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                    {
                        QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                        QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                    }
                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                    {
                        th.swap_header();
                    }
                    write.writeRawData((char *)&th,240);
                }
                if(flagResample==false)
                {
                    for(int i=0;i<saveNS;i++)
                    {
                        /// read>>temp;
                        read.readRawData((char *)&temp,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&temp);
                        }
                        if(flagStandardUse==true)
                        {
                            /// write<<temp;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            write.writeRawData((char *)&temp,4);
                        }
                    }
                }
                if(flagResample==true)
                {
                    if(flagAcResample==false)               //不同的读写数据方式
                    {                                                 //正常读写
                        for(int b=0;b<saveNS;b++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }

                            /// write<<temp;
                            for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                            {
                                read>>skipTemp;
                                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                {
                                    QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                {
                                    QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                write.writeRawData((char *)&temp,4);
                            }
                        }
                    }
                    if(flagAcResample==true)
                    {                                                 //精确读写
                        for(int b=0;b<leftNumTCReal.size();b++)
                        {
                            for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                            {
                                /// read>>temp;            //跳过不需要
                                read.readRawData((char *)&temp,4);
                            }
                            if(flagResampleChange==false)
                            {
                                for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);
                                }
                            }
                            if(flagResampleChange==true)        //非常特殊的情况
                            {
                                for(int d=0;d<loopNum[b];d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);
                                    for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                    {
                                        /// read>>skipTemp;
                                        read.readRawData((char *)&skipNum,4);
                                    }
                                }
                            }
                        }
                    }
                }
                p1.setValue(z);
            }
            if(z==count)
            {
                flagConvertComplete=true;
                // QMessageBox::information(this,tr("complete"),tr("Convert complete!"));
            }
        }

        if(flagSkip==true)
        {
            int b=count/(skipTrace+1);
            QProgressDialog p1(tr("Converting..."), tr("Abort"), 0,b-1, this);
            p1.setWindowModality(Qt::WindowModal);
            p1.setMinimumDuration(1000);

            for(z=0;z<b;z++)
            {
                fOpen.seek(headNum+z*(skipTrace+1)*traceLength);
                /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                read.readRawData((char *)&th,240);
                int saveNS;
                int saveDT;
                if(ui->radioOriginalBigEndian->isChecked()==true)
                {
                    th.swap_header();
                }
                saveNS=th.ns;
                saveDT=th.dt;

                if(flagStandardDefault==false)
                {
                    for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                    {
                        if(decide(leftNumTH[a],rightNumTH[a],SCI)==true)
                        {
                            flagStandardUse=true;
                            break;
                        }
                        else flagStandardUse=false;
                    }
                }
                if(flagStandardDefault==true)
                {
                    flagStandardUse=true;
                }

                if(flagStandardUse==true)
                {
                    if(flagResample==true)
                    {
                        th.ns=bh.hns;
                        th.dt=bh.hdt;
                        saveNS=th.ns;
                        saveDT=th.dt;
                    }
                    /// write<<th;
                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                    {
                        QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                        QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                    }
                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                    {
                        QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                        QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                    }
                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                    {
                        th.swap_header();
                    }
                    write.writeRawData((char *)&th,240);

                }
                if(flagResample==false)
                {
                    for(int i=0;i<saveNS;i++)
                    {
                        /// read>>temp;
                        read.readRawData((char *)&temp,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&temp);
                        }
                        if(flagStandardUse==true)
                        {
                            /// write<<temp;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            write.writeRawData((char *)&temp,4);
                        }
                    }
                }
                if(flagResample==true)
                {
                    if(flagAcResample==false)               //不同的读写数据方式
                    {                                                 //正常读写
                        for(int b=0;b<saveNS;b++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            /// write<<temp;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            write.writeRawData((char *)&temp,4);
                            for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                            {
                                /// read>>skipTemp;
                                read.readRawData((char *)&skipTemp,4);
                            }
                        }
                    }
                    if(flagAcResample==true)
                    {                                                 //精确读写
                        for(int b=0;b<leftNumTCReal.size();b++)
                        {
                            for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                            {
                                /// read>>temp;            //跳过不需要
                                read.readRawData((char *)&temp,4);
                            }
                            if(flagResampleChange==false)
                            {
                                for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);
                                }
                            }
                            if(flagResampleChange==true)        //非常特殊的情况
                            {
                                for(int d=0;d<loopNum[b];d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);

                                    for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                    {
                                        /// read>>skipTemp;
                                        read.readRawData((char *)&skipTemp,4);
                                    }
                                }
                            }
                        }
                    }
                }
                p1.setValue(z);
            }
            if(z==b)
            {
                flagConvertComplete=true;
                //   QMessageBox::information(this,tr("complete"),tr("Convert complete!"));
            }
        }
    }
}

/// 在非正常道头取值的情况下的所有组合
void MainWindow::convertNonStandard()
{
    flagConvertComplete=false;
    float temp;         //整个函数通用，非常重要
    float skipTemp; //跳过一部分的数据的临时存放

    // 3200
    read3200();

    //400
    read400();
    
    //240+数据
    /// 在有取道的情况下
    if(flagTraceScope==true)
    {
        int number;
        for(number=0;number<leftNum.size();number++)
        {
            fOpen.seek(headNum+(leftNum[number]-1)*traceLength);       //只需偏移到指定的地方，读取指定数据，再进行偏移即可
            if(flagSkip==false)
            {
                QProgressDialog p1(tr("Converting..."), tr("Abort"), 0,rightNum[number]-leftNum[number], this);
                p1.setWindowModality(Qt::WindowModal);
                p1.setMinimumDuration(1000);

                for(int j=0;j<rightNum[number]-leftNum[number]+1;j++)       //读取需要的道
                {
                    fOpen.seek(headNum+(leftNum[number]-1)*traceLength+j*traceLength);   //由于改变了采样时间，导致有可能一道数据不是正好取到结尾，所以每一道都要偏移道头上

                    seekSave=fOpen.pos();
                    fOpen.seek(seekSave+seekPos);       //先偏移到指定位置看看数据符合条件发，

                    if(flagNonStandardDefault==false)
                    {
                        switch(NSCI)                            ////符合的偏移回来读进去，不符合的偏移回来继续偏移到下一道
                        {
                        case 0:
                            /// read>>intx;
                            read.readRawData((char *)&intx,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_int_4(&intx);
                            }

                            for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                            {
                                if(decideInt(leftNumTH[a],rightNumTH[a])==true)
                                {
                                    flagUse=true;
                                    break;
                                }
                                else
                                {
                                    flagUse=false;
                                    fOpen.seek(seekSave+traceLength);
                                }
                            }

                            break;
                    case 1:
                            /// read>>qint16x;
                            read.readRawData((char *)&qint16x,2);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_short_2(&qint16x);
                            }

                            for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                            {
                                if(decideQint16(leftNumTH[a],rightNumTH[a])==true)
                                {
                                    flagUse=true;
                                    break;
                                }
                                else
                                {
                                    flagUse=false;
                                    fOpen.seek(seekSave+traceLength);
                                }
                            }
                            break;
                    case 2:
                            /// read>>floatx;
                            read.readRawData((char *)&floatx,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&floatx);
                            }

                            for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                            {
                                if(decideFloat(leftNumTH[a],rightNumTH[a])==true)
                                {
                                    flagUse=true;
                                    break;
                                }
                                else
                                {
                                    flagUse=false;
                                    fOpen.seek(seekSave+traceLength);
                                }
                            }
                            break;
                    case 3:
                            /// read>>quint16x;
                            read.readRawData((char *)&quint16x,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_u_short_2(&quint16x);
                            }

                            for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                            {
                                if(decideQuint16(leftNumTH[a],rightNumTH[a])==true)
                                {
                                    flagUse=true;
                                    break;
                                }
                                else
                                {
                                    flagUse=false;
                                    fOpen.seek(seekSave+traceLength);
                                }
                            }
                            break;
                        }
                    }
                    if(flagNonStandardDefault==true)
                        flagUse=true;

                    fOpen.seek(seekSave);
                    /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                    read.readRawData((char *)&th,240);
                    int saveNS;
                    int saveDT;
                    if(ui->radioOriginalBigEndian->isChecked()==true)
                    {
                        th.swap_header();
                    }
                    saveNS=th.ns;
                    saveDT=th.dt;

                    if(flagResample==true)
                    {
                        th.ns=bh.hns;
                        th.dt=bh.hdt;
                        saveNS=th.ns;
                        saveDT=th.dt;
                    }
                    if(flagUse==true)
                    {
                        /// write<<th;
                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                        {
                            QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                            QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                        }
                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                        {
                            QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                            QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                        }
                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                        {
                            th.swap_header();
                        }
                        write.writeRawData((char *)&th,240);
                    }
                    if(flagResample==false)
                    {
                        for(int i=0;i<saveNS;i++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            if(flagUse==true)
                            {
                                /// write<<temp;
                                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                {
                                    QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                {
                                    QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                write.writeRawData((char *)&temp,4);
                            }
                        }
                    }
                    if(flagResample==true)
                    {
                        if(flagAcResample==false)               //不同的读写数据方式
                        {                                                 //正常读写
                            for(int b=0;b<saveNS;b++)
                            {
                                /// read>>temp;
                                read.readRawData((char *)&temp,4);
                                if(ui->radioOriginalBigEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                /// write<<temp;
                                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                {
                                    QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                {
                                    QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                write.writeRawData((char *)&temp,4);

                                for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                {
                                    /// read>>skipTemp;
                                    read.readRawData((char *)&skipTemp,4);
                                }
                            }
                        }
                        if(flagAcResample==true)
                        {                                                 //精确读写
                            for(int b=0;b<leftNumTCReal.size();b++)
                            {
                                for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                                {
                                    /// read>>temp;            //跳过不需要
                                    read.readRawData((char *)&temp,4);
                                }
                                if(flagResampleChange==false)
                                {
                                    for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                    {
                                        /// read>>temp;
                                        read.readRawData((char *)&temp,4);
                                        if(ui->radioOriginalBigEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        /// write<<temp;
                                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                        {
                                            QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                        {
                                            QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        write.writeRawData((char *)&temp,4);
                                    }
                                }
                                if(flagResampleChange==true)        //非常特殊的情况
                                {
                                    for(int d=0;d<loopNum[b];d++)
                                    {
                                        /// read>>temp;
                                        read.readRawData((char *)&temp,4);
                                        if(ui->radioOriginalBigEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        /// write<<temp;
                                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                        {
                                            QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                        {
                                            QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        write.writeRawData((char *)&temp,4);
                                        for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                        {
                                            /// read>>skipTemp;
                                            read.readRawData((char *)&skipTemp,4);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    p1.setValue(j);
                    if (p1.wasCanceled())
                    {
                        QMessageBox::warning(this,tr("Attention"),tr("Convert has been terminated !"));
                        break;
                    }
                }

            }

            if(flagSkip==true)
            {
                QProgressDialog p1(tr("Converting..."),tr( "Abort"), 0,skipLoop[number], this);
                p1.setWindowModality(Qt::WindowModal);
                p1.setMinimumDuration(1000);

                for(int j=0;j<skipLoop[number]+1;j++)       //读取需要的道
                {
                    fOpen.seek(headNum+(leftNum[number]-1)*traceLength+j*(skipTrace+1)*traceLength);   //由于改变了采样时间，导致有可能一道数据不是正好取到结尾，所以每一道都要偏移道头上

                    seekSave=fOpen.pos();
                    fOpen.seek(seekSave+seekPos);       //先偏移到指定位置看看数据符合条件发，
                    if(flagNonStandardDefault==false)
                    {
                        switch(NSCI)                            ////符合的偏移回来读进去，不符合的偏移回来继续偏移到下一道
                        {
                        case 0:
                            /// read>>intx;
                            read.readRawData((char *)&intx,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_int_4(&intx);
                            }

                            for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                            {
                                if(decideInt(leftNumTH[a],rightNumTH[a])==true)
                                {
                                    flagUse=true;
                                    break;
                                }
                                else
                                {
                                    flagUse=false;
                                    fOpen.seek(seekSave+traceLength);
                                }
                            }

                            break;
                    case 1:
                            /// read>>qint16x;
                            read.readRawData((char *)&qint16x,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_short_2(&qint16x);
                            }

                            for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                            {
                                if(decideQint16(leftNumTH[a],rightNumTH[a])==true)
                                {
                                    flagUse=true;
                                    break;
                                }
                                else
                                {
                                    flagUse=false;
                                    fOpen.seek(seekSave+traceLength);
                                }
                            }
                            break;
                    case 2:
                            /// read>>floatx;
                            read.readRawData((char *)&floatx,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&floatx);
                            }

                            for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                            {
                                if(decideFloat(leftNumTH[a],rightNumTH[a])==true)
                                {
                                    flagUse=true;
                                    break;
                                }
                                else
                                {
                                    flagUse=false;
                                    fOpen.seek(seekSave+traceLength);
                                }
                            }
                            break;
                    case 3:
                            /// read>>quint16x;
                            read.readRawData((char *)&quint16x,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_u_short_2(&quint16x);
                            }

                            for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                            {
                                if(decideQuint16(leftNumTH[a],rightNumTH[a])==true)
                                {
                                    flagUse=true;
                                    break;
                                }
                                else
                                {
                                    flagUse=false;
                                    fOpen.seek(seekSave+traceLength);
                                }
                            }
                            break;
                        }
                    }
                    if(flagNonStandardDefault==true)
                        flagUse=true;


                    fOpen.seek(seekSave);
                    /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                    read.readRawData((char *)&th,240);
                    int saveNS;
                    int saveDT;
                    if(ui->radioOriginalBigEndian->isChecked()==true)
                    {
                        th.swap_header();
                    }
                    saveNS=th.ns;
                    saveDT=th.dt;
                    if(flagResample==true)
                    {
                        th.ns=bh.hns;
                        th.dt=bh.hdt;
                        saveNS=th.ns;
                        saveDT=th.dt;
                    }
                    if(flagUse==true)
                    {
                        /// write<<th;
                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                        {
                            QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                            QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                        }
                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                        {
                            QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                            QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                        }
                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                        {
                            th.swap_header();
                        }
                        write.writeRawData((char *)&th,240);
                    }
                    if(flagResample==false)
                    {
                        for(int i=0;i<saveNS;i++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            if(flagUse==true)
                            {
                                /// write<<temp;
                                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                {
                                    QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                {
                                    QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                write.writeRawData((char *)&temp,4);
                            }
                        }
                    }
                    if(flagResample==true)
                    {
                        if(flagAcResample==false)               //不同的读写数据方式
                        {                                                 //正常读写
                            for(int b=0;b<saveNS;b++)
                            {
                                /// read>>temp;
                                read.readRawData((char *)&temp,4);
                                if(ui->radioOriginalBigEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                /// write<<temp;
                                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                {
                                    QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                {
                                    QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                }
                                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                {
                                    QJD::qjdswap_float_4(&temp);
                                }
                                write.writeRawData((char *)&temp,4);
                                for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                {
                                    /// read>>skipTemp;
                                    read.readRawData((char *)&skipTemp,4);
                                }
                            }
                        }
                        if(flagAcResample==true)
                        {                                                 //精确读写
                            for(int b=0;b<leftNumTCReal.size();b++)
                            {
                                for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                                {
                                    /// read>>temp;            //跳过不需要
                                    read.readRawData((char *)&temp,4);
                                }
                                if(flagResampleChange==false)
                                {
                                    for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                    {
                                        /// read>>temp;
                                        read.readRawData((char *)&temp,4);
                                        if(ui->radioOriginalBigEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        /// write<<temp;
                                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                        {
                                            QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                        {
                                            QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        write.writeRawData((char *)&temp,4);
                                    }
                                }
                                if(flagResampleChange==true)        //非常特殊的情况
                                {
                                    for(int d=0;d<loopNum[b];d++)
                                    {
                                        /// read>>temp;
                                        read.readRawData((char *)&temp,4);
                                        if(ui->radioOriginalBigEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        /// write<<temp;
                                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                        {
                                            QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                        {
                                            QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                        }
                                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                        {
                                            QJD::qjdswap_float_4(&temp);
                                        }
                                        write.writeRawData((char *)&temp,4);

                                        for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                        {
                                            /// read>>skipTemp;
                                            read.readRawData((char *)&skipTemp,4);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    p1.setValue(j);
                    if (p1.wasCanceled())
                    {
                        QMessageBox::warning(this,tr("Attention"),tr("Convert has been terminated !"));
                        break;
                    }
                }
            }
        }
        if(number==leftNum.size())
        {
            flagConvertComplete=true;
            //QMessageBox::information(this,tr("Complete"),tr("Convert Complete."));
        }
    }

    /// 不取道时
    if(flagTraceScope==false)
    {
        int z;
        if(flagSkip==false)
        {
            QProgressDialog p1(tr("Converting..."), tr("Abort"), 0,count-1, this);
            p1.setWindowModality(Qt::WindowModal);
            p1.setMinimumDuration(1000);
            for(z=0;z<count;z++)
            {
                fOpen.seek(headNum+z*traceLength);

                seekSave=fOpen.pos();
                fOpen.seek(seekSave+seekPos);

                if(flagNonStandardDefault==false)
                {
                    switch(NSCI)                            ////符合的偏移回来读进去，不符合的偏移回来继续偏移到下一道
                    {
                    case 0:
                        /// read>>intx;
                        read.readRawData((char *)&intx,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_int_4(&intx);
                        }
                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decideInt(leftNumTH[a],rightNumTH[a])==true)
                            {
                                flagUse=true;
                                break;
                            }
                            else
                            {
                                flagUse=false;
                                fOpen.seek(seekSave+traceLength);
                            }
                        }
                        break;
            case 1:
                        /// read>>qint16x;
                        read.readRawData((char *)&qint16x,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_short_2(&qint16x);
                        }
                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decideQint16(leftNumTH[a],rightNumTH[a])==true)
                            {
                                flagUse=true;
                                break;
                            }
                            else
                            {
                                flagUse=false;
                                fOpen.seek(seekSave+traceLength);
                            }
                        }
                        break;
            case 2:
                        /// read>>floatx;
                        read.readRawData((char *)&floatx,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&floatx);
                        }
                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decideFloat(leftNumTH[a],rightNumTH[a])==true)
                            {
                                flagUse=true;
                                break;
                            }
                            else
                            {
                                flagUse=false;
                                fOpen.seek(seekSave+traceLength);
                            }
                        }
                        break;
            case 3:
                        /// read>>quint16x;
                        read.readRawData((char *)&quint16x,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_u_short_2(&quint16x);
                        }

                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decideQuint16(leftNumTH[a],rightNumTH[a])==true)
                            {
                                flagUse=true;
                                break;
                            }
                            else
                            {
                                flagUse=false;
                                fOpen.seek(seekSave+traceLength);
                            }
                        }
                        break;
                    }
                }
                if(flagNonStandardDefault==true)
                    flagUse=true;


                fOpen.seek(seekSave);
                /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                read.readRawData((char *)&th,240);
                int saveNS;
                int saveDT;
                if(ui->radioOriginalBigEndian->isChecked()==true)
                {
                    th.swap_header();
                }
                saveNS=th.ns;
                saveDT=th.dt;
                if(flagResample==true)
                {
                    th.ns=bh.hns;
                    th.dt=bh.hdt;
                    saveNS=th.ns;
                    saveDT=th.dt;
                }
                if(flagUse==true)
                {
                    /// write<<th;
                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                    {
                        QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                        QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                    }
                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                    {
                        QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                        QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                    }
                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                    {
                        th.swap_header();
                    }
                    write.writeRawData((char *)&th,240);
                }
                if(flagResample==false)
                {
                    for(int i=0;i<saveNS;i++)
                    {
                        /// read>>temp;
                        read.readRawData((char *)&temp,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&temp);
                        }
                        if(flagUse==true)
                        {
                            /// write<<temp;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            write.writeRawData((char *)&temp,4);
                        }
                    }
                }
                if(flagResample==true)
                {
                    if(flagAcResample==false)               //不同的读写数据方式
                    {                                                 //正常读写
                        for(int b=0;b<saveNS;b++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            /// write<<temp;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            write.writeRawData((char *)&temp,4);

                            for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                            {
                                /// read>>skipTemp;
                                read.readRawData((char *)&skipTemp,4);
                            }
                        }
                    }
                    if(flagAcResample==true)
                    {                                                 //精确读写
                        for(int b=0;b<leftNumTCReal.size();b++)
                        {
                            for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                            {
                                /// read>>temp;            //跳过不需要
                                read.readRawData((char *)&temp,4);
                            }
                            if(flagResampleChange==false)
                            {
                                for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);

                                }
                            }
                            if(flagResampleChange==true)        //非常特殊的情况
                            {
                                for(int d=0;d<loopNum[b];d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);

                                    for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                    {
                                        /// read>>skipTemp;
                                        read.readRawData((char *)&skipTemp,4);
                                    }
                                }
                            }
                        }
                    }
                }

                p1.setValue(z);
            }
            if(z==count)
            {
                flagConvertComplete=true;
                //    QMessageBox::information(this,tr("complete"),tr("Convert complete!"));
            }
        }

        if(flagSkip==true)
        {
            int b=count/(skipTrace+1);
            QProgressDialog p1(tr("Converting..."),tr( "Abort"), 0,b-1, this);
            p1.setWindowModality(Qt::WindowModal);
            p1.setMinimumDuration(1000);
            for(z=0;z<b;z++)
            {
                fOpen.seek(headNum+z*(skipTrace+1)*traceLength);

                seekSave=fOpen.pos();
                fOpen.seek(seekSave+seekPos);

                if(flagNonStandardDefault==false)
                {
                    switch(NSCI)                            ////符合的偏移回来读进去，不符合的偏移回来继续偏移到下一道
                    {
                    case 0:
                        /// read>>intx;
                        read.readRawData((char *)&intx,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_int_4(&intx);
                        }
                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decideInt(leftNumTH[a],rightNumTH[a])==true)
                            {
                                flagUse=true;
                                break;
                            }
                            else
                            {
                                flagUse=false;
                                fOpen.seek(seekSave+traceLength);
                            }
                        }
                        break;
            case 1:
                        /// read>>qint16x;
                        read.readRawData((char *)&qint16x,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_short_2(&qint16x);
                        }
                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decideQint16(leftNumTH[a],rightNumTH[a])==true)
                            {
                                flagUse=true;
                                break;
                            }
                            else
                            {
                                flagUse=false;
                                fOpen.seek(seekSave+traceLength);
                            }
                        }
                        break;
            case 2:
                        /// read>>floatx;
                        read.readRawData((char *)&floatx,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&floatx);
                        }
                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decideFloat(leftNumTH[a],rightNumTH[a])==true)
                            {
                                flagUse=true;
                                break;
                            }
                            else
                            {
                                flagUse=false;
                                fOpen.seek(seekSave+traceLength);
                            }
                        }
                        break;
            case 3:
                        /// read>>quint16x;
                        read.readRawData((char *)&quint16x,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_u_short_2(&quint16x);
                        }

                        for(int a=0;a<leftNumTH.size();a++)         //判断数据是否符合条件
                        {
                            if(decideQuint16(leftNumTH[a],rightNumTH[a])==true)
                            {
                                flagUse=true;
                                break;
                            }
                            else
                            {
                                flagUse=false;
                                fOpen.seek(seekSave+traceLength);
                            }
                        }
                        break;
                    }
                }
                if(flagNonStandardDefault==true)
                    flagUse=true;


                fOpen.seek(seekSave);
                /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                read.readRawData((char *)&th,240);
                int saveNS;
                int saveDT;
                if(ui->radioOriginalBigEndian->isChecked()==true)
                {
                    th.swap_header();
                }
                saveNS=th.ns;
                saveDT=th.dt;

                if(flagResample==true)
                {
                    th.ns=bh.hns;
                    th.dt=bh.hdt;
                    saveNS=th.ns;
                    saveDT=th.dt;
                }
                if(flagUse==true)
                {
                    /// write<<th;
                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                    {
                        QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                        QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                    }
                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                    {
                        QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                        QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                    }
                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                    {
                        th.swap_header();
                    }
                    write.writeRawData((char *)&th,240);
                }
                if(flagResample==false)
                {
                    for(int i=0;i<saveNS;i++)
                    {
                        /// read>>temp;
                        read.readRawData((char *)&temp,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&temp);
                        }
                        if(flagUse==true)
                        {
                            /// write<<temp;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            write.writeRawData((char *)&temp,4);
                        }
                    }
                }
                if(flagResample==true)
                {
                    if(flagAcResample==false)               //不同的读写数据方式
                    {                                                 //正常读写
                        for(int b=0;b<saveNS;b++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            /// write<<temp;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            write.writeRawData((char *)&temp,4);

                            for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                            {
                                /// read>>skipTemp;
                                read.readRawData((char *)&skipTemp,4);
                            }
                        }
                    }
                    if(flagAcResample==true)
                    {                                                 //精确读写
                        for(int b=0;b<leftNumTCReal.size();b++)
                        {
                            for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                            {
                                /// read>>temp;            //跳过不需要
                                read.readRawData((char *)&temp,4);
                            }
                            if(flagResampleChange==false)
                            {
                                for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);
                                }
                            }
                            if(flagResampleChange==true)        //非常特殊的情况
                            {
                                for(int d=0;d<loopNum[b];d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);

                                    for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                    {
                                        /// read>>skipTemp;
                                        read.readRawData((char *)&skipTemp,4);
                                    }
                                }
                            }
                        }
                    }
                }
                p1.setValue(z);
            }
            if(z==b)
            {
                flagConvertComplete=true;
                //    QMessageBox::information(this,tr("complete"),tr("Convert complete!"));
            }
        }
    }
}

/// 在取道时不考虑取值的情况下的组合
void MainWindow::convertTraceScope()
{
    flagConvertComplete=false;
    QMessageBox m1;

    float temp;         //整个函数通用，非常重要
    float skipTemp;     //跳过一部分的数据的临时存放

    // 3200
    read3200();

    //400
    read400();

    /// /////////////////////////////////////////////////////////////
    int number;
    for(number=0;number<leftNum.size();number++)
    {
        fOpen.seek(headNum+(leftNum[number]-1)*traceLength);       //只需偏移到指定的地方，读取指定数据，再进行偏移即可

        if(flagSkip==false)
        {
            QProgressDialog p1(tr("Scoping..."), tr("Abort"), 0,rightNum[number]-leftNum[number], this);
            p1.setWindowModality(Qt::WindowModal);
            p1.setMinimumDuration(1000);

            for(int j=0;j<rightNum[number]-leftNum[number]+1;j++)       //循环需要的道段中道的数量
            {
                fOpen.seek(3600+(leftNum[number]-1)*traceLength+j*traceLength);     //偏移到需要的道

                /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                read.readRawData((char *)&th,240);
                int saveNS;
                int saveDT;
                if(ui->radioOriginalBigEndian->isChecked()==true)
                {
                    th.swap_header();
                }
                saveNS=th.ns;
                saveDT=th.dt;

                if(flagResample==true)
                {
                    th.ns=bh.hns;       //写入最新更改过得数据
                    th.dt=bh.hdt;                    
                    saveNS=th.ns;
                    saveDT=th.dt;
                }
                /// write<<th;
                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                {
                    QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                    QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                }
                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                {
                    QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                    QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                }
                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                {
                    th.swap_header();
                }
                write.writeRawData((char *)&th,240);

                if(flagResample==false)
                {
                    for(int i=0;i<saveNS;i++)
                    {
                        /// read>>temp;
                        read.readRawData((char *)&temp,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&temp);
                        }

                        /// write<<temp;
                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                        {
                            QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                        }
                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                        {
                            QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                        }
                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&temp);
                        }
                        write.writeRawData((char *)&temp,4);
                    }
                }

                if(flagResample==true)
                {
                    if(flagAcResample==false)               //不同的读写数据方式
                    {                                                 //正常读写
                        for(int b=0;b<saveNS;b++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }

                            /// write<<temp;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            write.writeRawData((char *)&temp,4);
                            for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                            {
                                /// read>>skipTemp;
                                write.writeRawData((char *)&skipTemp,240);
                            }
                        }
                    }
                    if(flagAcResample==true)
                    {                                                 //精确读写
                        for(int b=0;b<leftNumTCReal.size();b++)
                        {
                            for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                            {
                                /// read>>temp;            //跳过不需要
                                read.readRawData((char *)&temp,4);
                            }
                            if(flagResampleChange==false)
                            {
                                for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);
                                }
                            }
                            if(flagResampleChange==true)        //非常特殊的情况
                            {
                                for(int d=0;d<loopNum[b];d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);

                                    for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                    {
                                        /// read>>skipTemp;
                                        read.readRawData((char *)&skipTemp,4);
                                    }
                                }
                            }
                        }
                    }
                }

                p1.setValue(j);
                if (p1.wasCanceled())
                {
                    m1.setText(tr("Scope has been terminated !"));
                    m1.exec();
                    break;
                }
            }
        }

        if(flagSkip==true)
        {
            QProgressDialog p1(tr("Scoping..."), tr("Abort"), 0,skipLoop[number], this);
            p1.setWindowModality(Qt::WindowModal);
            p1.setMinimumDuration(1000);

            for(int j=0;j<skipLoop[number];j++)       //读取需要的道
            {
                fOpen.seek(headNum+(leftNum[number]-1)*traceLength+j*(skipTrace+1)*traceLength);
                /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
                read.readRawData((char *)&th,240);
                int saveNS;
                int saveDT;
                if(ui->radioOriginalBigEndian->isChecked()==true)
                {
                    th.swap_header();
                }
                saveNS=th.ns;
                saveDT=th.dt;

                if(flagResample==true)
                {
                    //qDebug()<<bh.hns<<bh.hdt;    //读取正确。。。-.-
                    th.ns=bh.hns;
                    th.dt=bh.hdt;
                    saveNS=th.ns;
                    saveDT=th.dt;
                }
                /// write<<th;
                if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                {
                    QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                    QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
                }
                if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                {
                    QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                    QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
                }
                if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                {
                    th.swap_header();
                }
                write.writeRawData((char *)&th,240);

                if(flagResample==false)     //什么都不做，单纯的读出写入
                {
                    for(int i=0;i<th.ns;i++)
                    {
                        read>>temp;
                        write<<temp;
                    }
                }

                if(flagResample==true)
                {
                    if(flagAcResample==false)               //不同的读写数据方式
                    {                                                 //正常读写
                        for(int b=0;b<saveNS;b++)
                        {
                            /// read>>temp;
                            read.readRawData((char *)&temp,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            /// write<<temp;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp);
                            }
                            write.writeRawData((char *)&temp,4);

                            for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                            {
                                /// read>>skipTemp;
                                read.readRawData((char *)&temp,4);
                            }
                        }
                    }
                    if(flagAcResample==true)
                    {                                                 //精确读写
                        for(int b=0;b<leftNumTCReal.size();b++)
                        {
                            for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                            {
                                /// read>>temp;            //跳过不需要
                                read.readRawData((char *)&temp,4);
                            }
                            if(flagResampleChange==false)
                            {
                                for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }

                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);
                                }
                            }
                            if(flagResampleChange==true)        //非常特殊的情况
                            {
                                for(int d=0;d<loopNum[b];d++)
                                {
                                    /// read>>temp;
                                    read.readRawData((char *)&temp,4);
                                    if(ui->radioOriginalBigEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    /// write<<temp;
                                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                                    {
                                        QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                                    {
                                        QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
                                    }
                                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                                    {
                                        QJD::qjdswap_float_4(&temp);
                                    }
                                    write.writeRawData((char *)&temp,4);

                                    for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                                    {
                                        /// read>>skipTemp;
                                        read.readRawData((char *)&skipTemp,4);
                                    }
                                }
                            }
                        }
                    }
                }
                p1.setValue(j);
                if (p1.wasCanceled())
                {
                    m1.setText(tr("Scope has been terminated !"));
                    m1.exec();
                    break;
                }
            }
        }
    }
    if(number==leftNum.size())
    {
        flagConvertComplete=true;
        //        m1.setText(tr("Trace Scope Complete."));
        //        m1.exec();
    }
}

/// 在重采样的情况下+隔道取道
void MainWindow::convertResample()
{
    flagConvertComplete=false;
    QMessageBox m1;

    float temp1;         //整个函数通用，非常重要
    float skipTemp;     //跳过一部分的数据的临时存放
    // 3200
    read3200();

    //400
    read400();

    /// //////////////////////////////////////////
    int a;
    if(flagSkip==false)
    {
        QProgressDialog p1(tr("Resampling..."), tr("Abort"), 0,count-1, this);
        p1.setWindowModality(Qt::WindowModal);
        p1.setMinimumDuration(1000);
        for(a=0;a<count;a++)
        {
            fOpen.seek(headNum+a*traceLength);   //由于改变了采样时间，导致有可能一道数据不是正好取到结尾，所以每一道都要偏移道头上
            /// read>>th;
            read.readRawData((char *)&th,240);
            int saveNS;
            int saveDT;
            if(ui->radioOriginalBigEndian->isChecked()==true)
            {
                th.swap_header();
            }
            saveNS=th.ns;
            saveDT=th.dt;
            th.ns=bh.hns;
            th.dt=bh.hdt;
            saveNS=th.ns;
            saveDT=th.dt;

            /// write<<th;
            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
            {
                QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
            }
            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
            {
                QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
            }
            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
            {
                th.swap_header();
            }
            write.writeRawData((char *)&th,240);

            if(flagAcResample==false)               //不同的读写数据方式
            {                                                 //正常读写
                for(int b=0;b<saveNS;b++)
                {
                    /// read>>temp1;
                    read.readRawData((char *)&temp1,4);
                    if(ui->radioOriginalBigEndian->isChecked()==true)
                    {
                        QJD::qjdswap_float_4(&temp1);
                    }
                    /// write<<temp1;
                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                    {
                        QJD::qjdibm2ieee((int *)&temp1,(int *)&temp1,1,1);
                    }
                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                    {
                        QJD::qjdieee2ibm((int *)&temp1,(int *)&temp1,1,1);
                    }
                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                    {
                        QJD::qjdswap_float_4(&temp1);
                    }
                    write.writeRawData((char *)&temp,4);
                    for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                    {
                        /// read>>skipTemp;
                        read.readRawData((char *)&skipTemp,4);
                    }
                }
            }
            if(flagAcResample==true)
            {                                                 //精确读写
                for(int b=0;b<leftNumTCReal.size();b++)
                {
                    for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                    {
                        /// read>>temp1;            //跳过不需要
                        read.readRawData((char *)&temp1,4);
                    }
                    if(flagResampleChange==false)
                    {
                        for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                        {
                            /// read>>temp1;
                            read.readRawData((char *)&temp1,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp1);
                            }
                            /// write<<temp1;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp1,(int *)&temp1,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp1,(int *)&temp1,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp1);
                            }
                            write.writeRawData((char *)&temp,4);
                        }
                    }
                    if(flagResampleChange==true)        //非常特殊的情况
                    {
                        for(int d=0;d<loopNum[b];d++)
                        {
                            /// read>>temp1;
                            read.readRawData((char *)&temp1,4);
                            if(ui->radioOriginalBigEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp1);
                            }
                            /// write<<temp1;
                            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                            {
                                QJD::qjdibm2ieee((int *)&temp1,(int *)&temp1,1,1);
                            }
                            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                            {
                                QJD::qjdieee2ibm((int *)&temp1,(int *)&temp1,1,1);
                            }
                            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                            {
                                QJD::qjdswap_float_4(&temp1);
                            }
                            write.writeRawData((char *)&temp,4);
                            for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                            {
                                /// read>>skipTemp;
                                read.readRawData((char *)&skipTemp,4);
                            }
                        }
                    }
                }
            }
            p1.setValue(a);
            if (p1.wasCanceled())
            {
                m1.setText(tr("Resample has been terminated !"));
                m1.exec();
                break;
            }
        }
        if(a==count)
        {
            flagConvertComplete=true;
            //            m1.setText(tr("Resample Complete."));
            //            m1.exec();
        }
    }
    if(flagSkip==true)
    {
        int b=count/(skipTrace+1);
        QProgressDialog p1(tr("Resampling..."), tr("Abort"), 0,b-1, this);
        p1.setWindowModality(Qt::WindowModal);
        p1.setMinimumDuration(1000);
        for(a=0;a<b;a++)
        {
            fOpen.seek(headNum+a*(skipTrace+1)*traceLength);   //由于改变了采样时间，导致有可能一道数据不是正好取到结尾，所以每一道都要偏移道头上
            /// read>>th;
            read.readRawData((char *)&th,240);
            int saveNS;
            int saveDT;
            if(ui->radioOriginalBigEndian->isChecked()==true)
            {
                th.swap_header();
            }
            saveNS=th.ns;
            saveDT=th.dt;
            th.ns=bh.hns;
            th.dt=bh.hdt;
            saveNS=th.ns;
            saveDT=th.dt;

            /// write<<th;
            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
            {
                QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
                QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
            }
            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
            {
                QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
                QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
            }
            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
            {
                th.swap_header();
            }
            write.writeRawData((char *)&th,240);

            if(flagAcResample==false)               //不同的读写数据方式
            {                                                 //正常读写
                for(int b=0;b<saveNS;b++)
                {
                    /// read>>temp1;
                    read.readRawData((char *)&temp1,4);
                    if(ui->radioOriginalBigEndian->isChecked()==true)
                    {
                        QJD::qjdswap_float_4(&temp1);
                    }
                    /// write<<temp1;
                    if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                    {
                        QJD::qjdibm2ieee((int *)&temp1,(int *)&temp1,1,1);
                    }
                    if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                    {
                        QJD::qjdieee2ibm((int *)&temp1,(int *)&temp1,1,1);
                    }
                    if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                    {
                        QJD::qjdswap_float_4(&temp1);
                    }
                    write.writeRawData((char *)&temp1,4);

                    for(int c=0;c<skipNum;c++)      //略过指定数量的数据
                    {
                        /// read>>skipTemp;
                        read.readRawData((char *)&skipTemp,4);
                    }
                }
            }
            if(flagAcResample==true)
            {                                                 //精确读写
                for(int b=0;b<leftNumTCReal.size();b++)
                {
                    for(int c=0;c<leftNumTCReal[b]-rightNumTCMinus[b];c++)
                    {
                        /// read>>temp1;            //跳过不需要
                        read.readRawData((char *)&temp1,4);
                    }
                    for(int d=0;d<rightNumTCReal[b]-leftNumTCReal[b]+1;d++)
                    {
                        /// read>>temp1;
                        read.readRawData((char *)&temp1,4);
                        if(ui->radioOriginalBigEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&temp1);
                        }
                        /// write<<temp1;
                        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
                        {
                            QJD::qjdibm2ieee((int *)&temp1,(int *)&temp1,1,1);
                        }
                        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
                        {
                            QJD::qjdieee2ibm((int *)&temp1,(int *)&temp1,1,1);
                        }
                        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
                        {
                            QJD::qjdswap_float_4(&temp1);
                        }
                        write.writeRawData((char *)&temp1,4);

                    }
                }
            }
            p1.setValue(a);
            if (p1.wasCanceled())
            {
                m1.setText(tr("Resample has been terminated !"));
                m1.exec();
                break;
            }
        }
        if(a==b)
        {
            flagConvertComplete=true;
            //            m1.setText(tr("Resample Complete."));
            //            m1.exec();
        }
    }

}

/// 仅仅为隔道取道的情况下
void MainWindow::convertSkipTrace()
{
    flagConvertComplete=false;
    float temp;

    QMessageBox msgBox;

    //3200
    read3200();

    //400
    if(isSegy==true)
    {
         read>>bh;
//        read.readRawData((char *)&bh,400);
//        if(ui->radioOriginalBigEndian->isChecked()==true)
//        {
//            bh.swap_header();
//        }

        if(ui->radioIBM->isChecked())
            bh.format=1;
        else if(ui->radioIEEE->isChecked())
            bh.format=5;
        if(flagSU==false)
        {
             write<<bh;
//            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
//            {
//                bh.swap_header();
//            }
//            write.writeRawData((char *)&bh,400);
        }
        qDebug()<<bh.hns<<bh.hdt;
    }
    if(isSu==true)
    {
        bhTemp.hns=thTemp.ns;
        bhTemp.hdt=thTemp.dt;
        if(groupFormat.checkedButton()==ui->radioIEEE)      //设置目标文件的格式,可以一次性统一设好
        {
            bhTemp.format=5;
        }
        if(groupFormat.checkedButton()==ui->radioIBM)
        {
            bhTemp.format=1;
        }
        if(flagSU==false)
        {
             write<<bhTemp;
//            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
//            {
//                bhTemp.swap_header();
//            }
//            write.writeRawData((char *)&bhTemp,400);
        }
        fOpen.seek(headNum);      //转移到道头准备转换
    }

    int b;
    b=count/(skipTrace+1);
    qDebug()<<"b"<<b;
    QProgressDialog progress(tr("Converting files..."), tr("Abort"), 0,b-1, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(1000);

    int z;
    for(z=0;z<b;z++)
    {
        fOpen.seek(headNum+z*(skipTrace+1)*traceLength);

        /// read>>th;       //将开头第一个的240赋给th，这样就可用ns,dt;
        read.readRawData((char *)&th,240);
        int saveNS;
        int saveDT;
        if(ui->radioOriginalBigEndian->isChecked()==true)
        {
            th.swap_header();
        }
        saveNS=th.ns;
        saveDT=th.dt;
        /// write<<th;
        if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
        {
            QJD::qjdibm2ieee((int *)&th.cx,(int *)&th.cx,1,1);
            QJD::qjdibm2ieee((int *)&th.cy,(int *)&th.cy,1,1);
        }
        if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
        {
            QJD::qjdieee2ibm((int *)&th.cx,(int *)&th.cx,1,1);
            QJD::qjdieee2ibm((int *)&th.cy,(int *)&th.cy,1,1);
        }
        if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
        {
            th.swap_header();
        }
        write.writeRawData((char *)&th,240);

        for(int i=0;i<saveNS;i++)
        {
            /// read>>temp;
            read.readRawData((char *)&temp,4);
            if(ui->radioOriginalBigEndian->isChecked()==true)
            {
                QJD::qjdswap_float_4(&temp);
            }
            /// write<<temp;
            if(ui->radioOriginalIBM->isChecked()==true && ui->radioIEEE->isChecked()==true)
            {
                QJD::qjdibm2ieee((int *)&temp,(int *)&temp,1,1);
            }
            if(ui->radioOriginalIEEE->isChecked()==true && ui->radioIBM->isChecked()==true)
            {
                QJD::qjdieee2ibm((int *)&temp,(int *)&temp,1,1);
            }
            if(ui->radioBigEndian->isChecked()==true && ui->radioOriginalLittleEndian->isChecked()==true)
            {
                QJD::qjdswap_float_4(&temp);
            }
            write.writeRawData((char *)&temp,4);
        }
        progress.setValue(z);
        if (progress.wasCanceled())
        {
            msgBox.setText(tr("Format has been terminated !"));
            msgBox.exec();
            break;
        }

    }
    if(z==count)
    {
        flagConvertComplete=true;
    }
}


void MainWindow::lastClear()
{
    fOpen.close();
    fConvert.close();
    read.setDevice(0);
    write.setDevice(0);
    bty.clear();
    ui->labelOpenFileName->setText("");
    ui->lineConvertToFileName->setText("");
    ui->btnConvert->setEnabled(false);

    ui->groupTraceHeader->hide();
    ui->groupTraceHeader->hide();
    ui->groupResample->hide();
    ui->groupTraceScope->hide();
    ui->groupChoose->hide();
    ui->groupOption->hide();

    ui->label_14->hide();
    ui->listTimeRange->hide();
    ui->label_11->hide();
    ui->labelMaxTime->hide();
    ui->toolAddTime->hide();
    ui->toolRemoveTime->hide();

    ui->spinSkip->setVisible(false);

    ui->btnAD->setChecked(false);
    ui->btnConvert->setEnabled(false);
    ui->groupBox_3->setEnabled(false);
    ui->groupOutput->setEnabled(false);
    ui->groupOption->setEnabled(false);

    ui->checkResample->setChecked(false);
    ui->checkSkip->setChecked(false);
    ui->checkStandard->setChecked(false);
    ui->checkTraceScope->setChecked(false);
    ui->checkAcResample->setChecked(false);

    flagSetInfo=true;

    if(flagSkip==true)
    {
        ui->spinSkip->setValue(ui->spinSkip->minimum());
    }

    if(flagTraceScope==true)
    {
        left.clear();
        right.clear();
        leftNum.clear();
        rightNum.clear();
        dataRange.clear();
        rangeFinal.clear();
        md->setStringList(rangeFinal);
        ui->listTraceRange->setModel(md);
    }

    if(flagStandard==true || flagNonStandard==true)
    {
        ui->combo240->clear();
        ui->lineStPos->setText("");

        leftTH.clear();
        rightTH.clear();
        leftNumTH.clear();
        rightNumTH.clear();
        dataRangeTH.clear();
        rangeFinalTH.clear();
        mdTH->setStringList(rangeFinalTH);
        ui->listTraceHeadRange->setModel(mdTH);
    }
    
    if(flagResample==true)
    {
        leftTC.clear();
        rightTC.clear();
        leftNumTC.clear();
        rightNumTC.clear();
        leftNumTCReal.clear();
        rightNumTCReal.clear();
        rightNumTCMinus.clear();
        dataRangeTC.clear();
        rangeFinalTC.clear();
        mdTC->setStringList(rangeFinalTC);
        ui->listTraceHeadRange->setModel(mdTC);
    }
}

void MainWindow::on_combo240_currentIndexChanged(int index)
{
    //qDebug()<<reference(index);
    ui->labelReference->setText(QString::number(reference(index)));
}

int MainWindow::reference(int index)
{
    switch(index)
    {
    case 0:
        return thTemp.tracl;
    case 1:
        return thTemp.tracr;
    case 2:
        return thTemp.fldr;
    case 3:
        return thTemp.tracf;
    case 4:
        return thTemp.ep;
    case 5:
        return thTemp.cdp;
    case 6:
        return thTemp.cdpt;
    case 7:
        return thTemp.trid;
    case 8:
        return thTemp.nvs;
    case 9:
        return thTemp.nhs;
    case 10:
        return thTemp.duse;
    case 11:
        return thTemp.offset;
    case 12:
        return thTemp.gelev;
    case 13:
        return thTemp.selev;
    case 14:
        return thTemp.sdepth;
    case 15:
        return thTemp.gdel;
    case 16:
        return thTemp.sdel;
    case 17:
        return thTemp.swdep;
    case 18:
        return thTemp.gwdep;
    case 19:
        return thTemp.scalel;
    case 20:
        return thTemp.scalco;
    case 21:
        return thTemp.sx;
    case 22:
        return thTemp.sy;
    case 23:
        return thTemp.gx;
    case 24:
        return thTemp.gy;
    case 25:
        return thTemp.counit;
    case 26:
        return thTemp.wevel;
    case 27:
        return thTemp.swevel;
    case 28:
        return thTemp.sut;
    case 29:
        return thTemp.gut;
    case 30:
        return thTemp.sstat;
    case 31:
        return thTemp.gstat;
    case 32:
        return thTemp.tstat;
    case 33:
        return thTemp.laga;
    case 34:
        return thTemp.lagb;
    case 35:
        return thTemp.delrt;
    case 36:
        return thTemp.muts;
    case 37:
        return thTemp.mute;
    case 38:
        return thTemp.ns;
    case 39:
        return thTemp.dt;
    case 40:
        return thTemp.gain;
    case 41:
        return thTemp.igc;
    case 42:
        return thTemp.igi;
    case 43:
        return thTemp.corr;
    case 44:
        return thTemp.sfs;
    case 45:
        return thTemp.sfe;

    case 46:
        return thTemp.slen;

    case 47:
        return thTemp.styp;

    case 48:
        return thTemp.stas;

    case 49:
        return thTemp.stae;

    case 50:
        return thTemp.tatyp;

    case 51:
        return thTemp.afilf;

    case 52:
        return thTemp.afils;

    case 53:
        return thTemp.nofilf;

    case 54:
        return thTemp.nofils;

    case 55:
        return thTemp.lcf;

    case 56:
        return thTemp.hcf;

    case 57:
        return thTemp.lcs;

    case 58:
        return thTemp.hcs;

    case 59:
        return thTemp.year;

    case 60:
        return thTemp.day;

    case 61:
        return thTemp.hour;

    case 62:
        return thTemp.minute;

    case 63:
        return thTemp.sec;

    case 64:
        return thTemp.timbas;

    case 65:
        return thTemp.trwf;

    case 66:
        return thTemp.grnors;

    case 67:
        return thTemp.grnofr;

    case 68:
        return thTemp.grnlof;

    case 69:
        return thTemp.gaps;

    case 70:
        return thTemp.otrav;
    default:
        return false;
    }

}

void MainWindow::flagClear()
{
    flagNonStandard=false;
    flagResample=false;
    flagStandard=false;
    flagTraceScope=false;
    flagSkip=false;
    flagSU=false;
    flagBHis0=false;
    flagTHis0=false;
    flagSetInfo=true;
    flagStandardDefault=false;
    flagNonStandardDefault=false;
    flagAcResample=false;
    flagResampleChange=false;

    if(flagConvertComplete==true)
    {
        QMessageBox::information(this,"Success","Everything is done!");
    }
    flagConvertComplete=false;
}

void MainWindow::on_btnAD_clicked(bool checked)
{
    if(checked==false)
    {
        ui->checkStandard->setChecked(false);
        ui->checkResample->setChecked(false);
        ui->checkTraceScope->setChecked(false);
        ui->checkSkip->setChecked(false);

        ui->groupTraceHeader->hide();
        ui->groupResample->hide();
        ui->groupTraceScope->hide();
        ui->spinSkip->hide();

    }
}

//3200
void MainWindow::read3200()
{
    if(isSegy==true)
    {
        bty.resize(3201);       //切勿忘记开空间，后果不堪设想
        fOpen.seek(0);
        read.readRawData(bty.data(),3200);
        *(bty.data()+3200)='\0';
        if(flagSU==false)
        {
            write.writeRawData(bty.data(),3200);
        }
    }
    if(isSu==true)
    {
        bty.resize(3201);       //切勿忘记开空间，后果不堪设想
        *(bty.data()+3200)='\0';
        if(flagSU==false)
        {
            write.writeRawData(bty.data(),3200);
        }
    }
}

//400 修改之后有大问题,不修改为妙
void MainWindow::read400()
{
    if(isSegy==true)
    {
        read>>bh;
        if(flagResample==true)
        {
            if(flagBHis0==false)
            {
                oriDt=bhTemp.hdt;
                oriNs=bhTemp.hns;
            }
            else if(flagBHis0==true)
            {
                oriDt=thTemp.dt;
                oriNs=thTemp.ns;
            }
            nowNs=oriDt*oriNs/nowDt;
            bh.hdt=nowDt;       //将改动信息写入文件
            bh.hns=nowNs;
            if(flagAcResample==true)             //2.精确采样情况下重新写入信息
            {
                //现在的个数＊以前的采样间隔/现在的采样间隔
                newTraceNum=newTraceNum*oriDt/nowDt;
                bh.hns=newTraceNum;       /// bh也要写入，否则trace display无法正确显示
            }
            if(flagResampleChange==true)      //3.单纯和精确重采样结合情况下写入信息
            {
                bh.hns=allPointNum;
            }
        }
        if(ui->radioIBM->isChecked())
            bh.format=1;
        else if(ui->radioIEEE->isChecked())
            bh.format=5;
        if(flagSU==false)
        {
            write<<bh;
        }
    }

    if(isSu==true)
    {
        bhTemp.hns=thTemp.ns;
        bhTemp.hdt=thTemp.dt;
        if(groupFormat.checkedButton()==ui->radioIEEE)      //设置目标文件的格式,可以一次性统一设好
        {
            bhTemp.format=5;
        }
        if(groupFormat.checkedButton()==ui->radioIBM)
        {
            bhTemp.format=1;
        }
        if(flagSU==false)
        {
            write<<bhTemp;
        }
        fOpen.seek(headNum);      //转移到道头准备转换
    }
}
