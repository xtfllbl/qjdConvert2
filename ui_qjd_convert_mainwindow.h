/********************************************************************************
** Form generated from reading UI file 'qjd_convert_mainwindow.ui'
**
** Created: Fri Aug 5 10:14:19 2011
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QJD_CONVERT_MAINWINDOW_H
#define UI_QJD_CONVERT_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QWidget *centralWidget;
    QFormLayout *formLayout;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupInput;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *labelOpenFileName;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QRadioButton *radioOriginalIEEE;
    QRadioButton *radioOriginalIBM;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QRadioButton *radioOriginalLittleEndian;
    QRadioButton *radioOriginalBigEndian;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QRadioButton *radioFormat;
    QRadioButton *radioConvert;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_17;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btnAD;
    QGroupBox *groupOption;
    QGridLayout *gridLayout;
    QCheckBox *checkStandard;
    QCheckBox *checkTraceScope;
    QGroupBox *groupChoose;
    QGridLayout *gridLayout_7;
    QRadioButton *radioStandard;
    QRadioButton *radioNonStandard;
    QCheckBox *checkResample;
    QHBoxLayout *horizontalLayout_14;
    QCheckBox *checkSkip;
    QSpinBox *spinSkip;
    QSpacerItem *horizontalSpacer_9;
    QGroupBox *groupOutput;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QRadioButton *radioIEEE;
    QRadioButton *radioIBM;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QRadioButton *radioLittleEndian;
    QRadioButton *radioBigEndian;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *lineConvertToFileName;
    QPushButton *btnConvertTo;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnConvert;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupTraceHeader;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_2;
    QLabel *label_9;
    QComboBox *combo240;
    QLabel *label_10;
    QLabel *labelReference;
    QLabel *label_23;
    QLineEdit *lineStPos;
    QLabel *label_12;
    QLabel *label_24;
    QComboBox *comboDataType;
    QLabel *labelBlank;
    QLabel *labelBlank2;
    QLabel *labelBlank3;
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_19;
    QListView *listTraceHeadRange;
    QVBoxLayout *verticalLayout_10;
    QToolButton *toolTHAdd;
    QToolButton *toolTHRemove;
    QSpacerItem *horizontalSpacer_8;
    QGroupBox *groupTraceScope;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_2;
    QLabel *labelMaxTrace;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_13;
    QHBoxLayout *horizontalLayout_11;
    QListView *listTraceRange;
    QVBoxLayout *verticalLayout_3;
    QToolButton *toolAdd;
    QToolButton *toolRemove;
    QSpacerItem *horizontalSpacer_7;
    QGroupBox *groupResample;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_15;
    QLabel *labelCurrentInterval;
    QLabel *label_17;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_16;
    QComboBox *comboInterval;
    QLabel *label_18;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout_19;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_14;
    QListView *listTimeRange;
    QVBoxLayout *verticalLayout_5;
    QToolButton *toolAddTime;
    QToolButton *toolRemoveTime;
    QSpacerItem *horizontalSpacer_12;
    QCheckBox *checkAcResample;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_11;
    QLabel *labelMaxTime;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(998, 798);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(600, 0));
        MainWindow->setMaximumSize(QSize(50000, 50000));
        QIcon icon;
        icon.addFile(QString::fromUtf8("images/actions/gtk-sort-descending.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/image/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/image/edit-delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setMaximumSize(QSize(2000, 2000));
        formLayout = new QFormLayout(centralWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        groupInput = new QGroupBox(centralWidget);
        groupInput->setObjectName(QString::fromUtf8("groupInput"));
        groupInput->setMaximumSize(QSize(550, 2000));
        verticalLayout_2 = new QVBoxLayout(groupInput);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(groupInput);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 30));
        label->setMaximumSize(QSize(120, 50));

        horizontalLayout_2->addWidget(label);

        labelOpenFileName = new QLabel(groupInput);
        labelOpenFileName->setObjectName(QString::fromUtf8("labelOpenFileName"));
        labelOpenFileName->setMinimumSize(QSize(200, 0));
        labelOpenFileName->setFrameShape(QFrame::Panel);
        labelOpenFileName->setFrameShadow(QFrame::Sunken);
        labelOpenFileName->setMidLineWidth(0);

        horizontalLayout_2->addWidget(labelOpenFileName);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(groupInput);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(150, 0));

        horizontalLayout_6->addWidget(label_6);

        radioOriginalIEEE = new QRadioButton(groupInput);
        radioOriginalIEEE->setObjectName(QString::fromUtf8("radioOriginalIEEE"));
        radioOriginalIEEE->setMinimumSize(QSize(100, 0));

        horizontalLayout_6->addWidget(radioOriginalIEEE);

        radioOriginalIBM = new QRadioButton(groupInput);
        radioOriginalIBM->setObjectName(QString::fromUtf8("radioOriginalIBM"));

        horizontalLayout_6->addWidget(radioOriginalIBM);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_7 = new QLabel(groupInput);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(150, 0));

        horizontalLayout_7->addWidget(label_7);

        radioOriginalLittleEndian = new QRadioButton(groupInput);
        radioOriginalLittleEndian->setObjectName(QString::fromUtf8("radioOriginalLittleEndian"));
        radioOriginalLittleEndian->setMinimumSize(QSize(100, 0));

        horizontalLayout_7->addWidget(radioOriginalLittleEndian);

        radioOriginalBigEndian = new QRadioButton(groupInput);
        radioOriginalBigEndian->setObjectName(QString::fromUtf8("radioOriginalBigEndian"));

        horizontalLayout_7->addWidget(radioOriginalBigEndian);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_7);


        verticalLayout_6->addWidget(groupInput);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(0, 0));
        groupBox_3->setMaximumSize(QSize(550, 16777215));
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(150, 0));

        horizontalLayout_9->addWidget(label_8);

        radioFormat = new QRadioButton(groupBox_3);
        radioFormat->setObjectName(QString::fromUtf8("radioFormat"));
        radioFormat->setMinimumSize(QSize(100, 0));

        horizontalLayout_9->addWidget(radioFormat);

        radioConvert = new QRadioButton(groupBox_3);
        radioConvert->setObjectName(QString::fromUtf8("radioConvert"));

        horizontalLayout_9->addWidget(radioConvert);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_6);


        gridLayout_6->addLayout(horizontalLayout_9, 0, 0, 1, 1);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_5);

        btnAD = new QPushButton(groupBox_3);
        btnAD->setObjectName(QString::fromUtf8("btnAD"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/image/go-next.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAD->setIcon(icon3);
        btnAD->setCheckable(true);

        horizontalLayout_17->addWidget(btnAD);


        gridLayout_6->addLayout(horizontalLayout_17, 1, 0, 1, 1);


        verticalLayout_6->addWidget(groupBox_3);

        groupOption = new QGroupBox(centralWidget);
        groupOption->setObjectName(QString::fromUtf8("groupOption"));
        groupOption->setMaximumSize(QSize(550, 16777215));
        gridLayout = new QGridLayout(groupOption);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkStandard = new QCheckBox(groupOption);
        checkStandard->setObjectName(QString::fromUtf8("checkStandard"));

        gridLayout->addWidget(checkStandard, 0, 0, 1, 1);

        checkTraceScope = new QCheckBox(groupOption);
        checkTraceScope->setObjectName(QString::fromUtf8("checkTraceScope"));

        gridLayout->addWidget(checkTraceScope, 3, 0, 1, 1);

        groupChoose = new QGroupBox(groupOption);
        groupChoose->setObjectName(QString::fromUtf8("groupChoose"));
        gridLayout_7 = new QGridLayout(groupChoose);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        radioStandard = new QRadioButton(groupChoose);
        radioStandard->setObjectName(QString::fromUtf8("radioStandard"));
        radioStandard->setMinimumSize(QSize(0, 40));

        gridLayout_7->addWidget(radioStandard, 0, 0, 1, 1);

        radioNonStandard = new QRadioButton(groupChoose);
        radioNonStandard->setObjectName(QString::fromUtf8("radioNonStandard"));
        radioNonStandard->setMinimumSize(QSize(40, 40));

        gridLayout_7->addWidget(radioNonStandard, 0, 1, 1, 1);


        gridLayout->addWidget(groupChoose, 1, 0, 1, 1);

        checkResample = new QCheckBox(groupOption);
        checkResample->setObjectName(QString::fromUtf8("checkResample"));
        QFont font;
        font.setKerning(true);
        checkResample->setFont(font);

        gridLayout->addWidget(checkResample, 5, 0, 1, 1);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        checkSkip = new QCheckBox(groupOption);
        checkSkip->setObjectName(QString::fromUtf8("checkSkip"));
        checkSkip->setMinimumSize(QSize(150, 0));

        horizontalLayout_14->addWidget(checkSkip);

        spinSkip = new QSpinBox(groupOption);
        spinSkip->setObjectName(QString::fromUtf8("spinSkip"));
        spinSkip->setMinimumSize(QSize(100, 0));
        spinSkip->setMinimum(1);

        horizontalLayout_14->addWidget(spinSkip);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_9);


        gridLayout->addLayout(horizontalLayout_14, 6, 0, 1, 1);


        verticalLayout_6->addWidget(groupOption);

        groupOutput = new QGroupBox(centralWidget);
        groupOutput->setObjectName(QString::fromUtf8("groupOutput"));
        groupOutput->setMinimumSize(QSize(0, 0));
        groupOutput->setMaximumSize(QSize(550, 2000));
        verticalLayout = new QVBoxLayout(groupOutput);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(groupOutput);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(150, 0));

        horizontalLayout_3->addWidget(label_3);

        radioIEEE = new QRadioButton(groupOutput);
        radioIEEE->setObjectName(QString::fromUtf8("radioIEEE"));
        radioIEEE->setMinimumSize(QSize(100, 0));

        horizontalLayout_3->addWidget(radioIEEE);

        radioIBM = new QRadioButton(groupOutput);
        radioIBM->setObjectName(QString::fromUtf8("radioIBM"));

        horizontalLayout_3->addWidget(radioIBM);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(groupOutput);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(150, 0));

        horizontalLayout_4->addWidget(label_4);

        radioLittleEndian = new QRadioButton(groupOutput);
        radioLittleEndian->setObjectName(QString::fromUtf8("radioLittleEndian"));
        radioLittleEndian->setMinimumSize(QSize(100, 0));

        horizontalLayout_4->addWidget(radioLittleEndian);

        radioBigEndian = new QRadioButton(groupOutput);
        radioBigEndian->setObjectName(QString::fromUtf8("radioBigEndian"));

        horizontalLayout_4->addWidget(radioBigEndian);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(groupOutput);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(90, 0));
        label_5->setMaximumSize(QSize(90, 16777215));

        horizontalLayout_5->addWidget(label_5);

        lineConvertToFileName = new QLineEdit(groupOutput);
        lineConvertToFileName->setObjectName(QString::fromUtf8("lineConvertToFileName"));

        horizontalLayout_5->addWidget(lineConvertToFileName);

        btnConvertTo = new QPushButton(groupOutput);
        btnConvertTo->setObjectName(QString::fromUtf8("btnConvertTo"));
        btnConvertTo->setMinimumSize(QSize(30, 30));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/image/document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnConvertTo->setIcon(icon4);

        horizontalLayout_5->addWidget(btnConvertTo);


        verticalLayout->addLayout(horizontalLayout_5);


        verticalLayout_6->addWidget(groupOutput);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnConvert = new QPushButton(centralWidget);
        btnConvert->setObjectName(QString::fromUtf8("btnConvert"));
        btnConvert->setMinimumSize(QSize(100, 0));
        btnConvert->setMaximumSize(QSize(120, 16777215));
        btnConvert->setAutoFillBackground(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/image/dialog-ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnConvert->setIcon(icon5);

        horizontalLayout->addWidget(btnConvert);


        verticalLayout_6->addLayout(horizontalLayout);


        formLayout->setLayout(0, QFormLayout::LabelRole, verticalLayout_6);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupTraceHeader = new QGroupBox(centralWidget);
        groupTraceHeader->setObjectName(QString::fromUtf8("groupTraceHeader"));
        sizePolicy.setHeightForWidth(groupTraceHeader->sizePolicy().hasHeightForWidth());
        groupTraceHeader->setSizePolicy(sizePolicy);
        groupTraceHeader->setMinimumSize(QSize(0, 0));
        groupTraceHeader->setMaximumSize(QSize(550, 16777215));
        gridLayout_5 = new QGridLayout(groupTraceHeader);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_9 = new QLabel(groupTraceHeader);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy1);
        label_9->setMinimumSize(QSize(150, 0));

        gridLayout_2->addWidget(label_9, 0, 0, 1, 1);

        combo240 = new QComboBox(groupTraceHeader);
        combo240->setObjectName(QString::fromUtf8("combo240"));
        combo240->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
        combo240->setMinimumContentsLength(10);

        gridLayout_2->addWidget(combo240, 0, 1, 1, 1);

        label_10 = new QLabel(groupTraceHeader);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 0, 2, 1, 1);

        labelReference = new QLabel(groupTraceHeader);
        labelReference->setObjectName(QString::fromUtf8("labelReference"));

        gridLayout_2->addWidget(labelReference, 0, 3, 1, 1);

        label_23 = new QLabel(groupTraceHeader);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        sizePolicy1.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy1);
        label_23->setMinimumSize(QSize(150, 0));

        gridLayout_2->addWidget(label_23, 1, 0, 1, 1);

        lineStPos = new QLineEdit(groupTraceHeader);
        lineStPos->setObjectName(QString::fromUtf8("lineStPos"));
        lineStPos->setMaximumSize(QSize(200, 16777215));

        gridLayout_2->addWidget(lineStPos, 1, 1, 1, 1);

        label_12 = new QLabel(groupTraceHeader);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_2->addWidget(label_12, 1, 2, 1, 1);

        label_24 = new QLabel(groupTraceHeader);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        sizePolicy1.setHeightForWidth(label_24->sizePolicy().hasHeightForWidth());
        label_24->setSizePolicy(sizePolicy1);
        label_24->setMinimumSize(QSize(150, 0));

        gridLayout_2->addWidget(label_24, 2, 0, 1, 1);

        comboDataType = new QComboBox(groupTraceHeader);
        comboDataType->setObjectName(QString::fromUtf8("comboDataType"));
        comboDataType->setMaximumSize(QSize(200, 16777215));

        gridLayout_2->addWidget(comboDataType, 2, 1, 1, 1);

        labelBlank = new QLabel(groupTraceHeader);
        labelBlank->setObjectName(QString::fromUtf8("labelBlank"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labelBlank->sizePolicy().hasHeightForWidth());
        labelBlank->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(labelBlank, 1, 3, 1, 1);

        labelBlank2 = new QLabel(groupTraceHeader);
        labelBlank2->setObjectName(QString::fromUtf8("labelBlank2"));
        sizePolicy2.setHeightForWidth(labelBlank2->sizePolicy().hasHeightForWidth());
        labelBlank2->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(labelBlank2, 2, 2, 1, 1);

        labelBlank3 = new QLabel(groupTraceHeader);
        labelBlank3->setObjectName(QString::fromUtf8("labelBlank3"));
        sizePolicy2.setHeightForWidth(labelBlank3->sizePolicy().hasHeightForWidth());
        labelBlank3->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(labelBlank3, 2, 3, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 0, 0, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_19 = new QLabel(groupTraceHeader);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setMinimumSize(QSize(150, 0));
        label_19->setFrameShape(QFrame::Box);
        label_19->setFrameShadow(QFrame::Sunken);

        horizontalLayout_8->addWidget(label_19);

        listTraceHeadRange = new QListView(groupTraceHeader);
        listTraceHeadRange->setObjectName(QString::fromUtf8("listTraceHeadRange"));
        sizePolicy1.setHeightForWidth(listTraceHeadRange->sizePolicy().hasHeightForWidth());
        listTraceHeadRange->setSizePolicy(sizePolicy1);
        listTraceHeadRange->setMinimumSize(QSize(0, 0));
        listTraceHeadRange->setMaximumSize(QSize(150, 70));

        horizontalLayout_8->addWidget(listTraceHeadRange);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        toolTHAdd = new QToolButton(groupTraceHeader);
        toolTHAdd->setObjectName(QString::fromUtf8("toolTHAdd"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/image/Crystal_Clear_action_edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolTHAdd->setIcon(icon6);

        verticalLayout_10->addWidget(toolTHAdd);

        toolTHRemove = new QToolButton(groupTraceHeader);
        toolTHRemove->setObjectName(QString::fromUtf8("toolTHRemove"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/image/Crystal_Clear_action_edit_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolTHRemove->setIcon(icon7);

        verticalLayout_10->addWidget(toolTHRemove);


        horizontalLayout_8->addLayout(verticalLayout_10);


        horizontalLayout_10->addLayout(horizontalLayout_8);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_8);


        gridLayout_5->addLayout(horizontalLayout_10, 1, 0, 1, 1);


        verticalLayout_4->addWidget(groupTraceHeader);

        groupTraceScope = new QGroupBox(centralWidget);
        groupTraceScope->setObjectName(QString::fromUtf8("groupTraceScope"));
        groupTraceScope->setMinimumSize(QSize(0, 0));
        groupTraceScope->setMaximumSize(QSize(550, 200));
        gridLayout_3 = new QGridLayout(groupTraceScope);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_2 = new QLabel(groupTraceScope);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(150, 16777215));

        horizontalLayout_16->addWidget(label_2);

        labelMaxTrace = new QLabel(groupTraceScope);
        labelMaxTrace->setObjectName(QString::fromUtf8("labelMaxTrace"));

        horizontalLayout_16->addWidget(labelMaxTrace);


        gridLayout_3->addLayout(horizontalLayout_16, 0, 0, 1, 1);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_13 = new QLabel(groupTraceScope);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(150, 0));
        label_13->setFrameShape(QFrame::Box);
        label_13->setFrameShadow(QFrame::Sunken);

        horizontalLayout_12->addWidget(label_13);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        listTraceRange = new QListView(groupTraceScope);
        listTraceRange->setObjectName(QString::fromUtf8("listTraceRange"));
        listTraceRange->setMaximumSize(QSize(150, 70));

        horizontalLayout_11->addWidget(listTraceRange);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        toolAdd = new QToolButton(groupTraceScope);
        toolAdd->setObjectName(QString::fromUtf8("toolAdd"));
        toolAdd->setIcon(icon6);

        verticalLayout_3->addWidget(toolAdd);

        toolRemove = new QToolButton(groupTraceScope);
        toolRemove->setObjectName(QString::fromUtf8("toolRemove"));
        toolRemove->setIcon(icon7);

        verticalLayout_3->addWidget(toolRemove);


        horizontalLayout_11->addLayout(verticalLayout_3);


        horizontalLayout_12->addLayout(horizontalLayout_11);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_7);


        gridLayout_3->addLayout(horizontalLayout_12, 1, 0, 1, 1);


        verticalLayout_4->addWidget(groupTraceScope);

        groupResample = new QGroupBox(centralWidget);
        groupResample->setObjectName(QString::fromUtf8("groupResample"));
        groupResample->setMinimumSize(QSize(0, 0));
        groupResample->setMaximumSize(QSize(550, 16777215));
        gridLayout_4 = new QGridLayout(groupResample);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_15 = new QLabel(groupResample);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(160, 0));

        horizontalLayout_13->addWidget(label_15);

        labelCurrentInterval = new QLabel(groupResample);
        labelCurrentInterval->setObjectName(QString::fromUtf8("labelCurrentInterval"));
        labelCurrentInterval->setMinimumSize(QSize(150, 0));

        horizontalLayout_13->addWidget(labelCurrentInterval);

        label_17 = new QLabel(groupResample);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayout_13->addWidget(label_17);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_10);


        gridLayout_4->addLayout(horizontalLayout_13, 0, 0, 1, 1);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_16 = new QLabel(groupResample);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(160, 0));

        horizontalLayout_15->addWidget(label_16);

        comboInterval = new QComboBox(groupResample);
        comboInterval->setObjectName(QString::fromUtf8("comboInterval"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(comboInterval->sizePolicy().hasHeightForWidth());
        comboInterval->setSizePolicy(sizePolicy3);
        comboInterval->setMinimumSize(QSize(150, 0));

        horizontalLayout_15->addWidget(comboInterval);

        label_18 = new QLabel(groupResample);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_15->addWidget(label_18);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_11);


        gridLayout_4->addLayout(horizontalLayout_15, 1, 0, 1, 1);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        label_14 = new QLabel(groupResample);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(160, 0));
        label_14->setFrameShape(QFrame::Box);
        label_14->setFrameShadow(QFrame::Sunken);

        horizontalLayout_20->addWidget(label_14);

        listTimeRange = new QListView(groupResample);
        listTimeRange->setObjectName(QString::fromUtf8("listTimeRange"));
        listTimeRange->setMaximumSize(QSize(150, 70));

        horizontalLayout_20->addWidget(listTimeRange);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        toolAddTime = new QToolButton(groupResample);
        toolAddTime->setObjectName(QString::fromUtf8("toolAddTime"));
        toolAddTime->setIcon(icon6);

        verticalLayout_5->addWidget(toolAddTime);

        toolRemoveTime = new QToolButton(groupResample);
        toolRemoveTime->setObjectName(QString::fromUtf8("toolRemoveTime"));
        toolRemoveTime->setIcon(icon7);

        verticalLayout_5->addWidget(toolRemoveTime);


        horizontalLayout_20->addLayout(verticalLayout_5);


        horizontalLayout_19->addLayout(horizontalLayout_20);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_19->addItem(horizontalSpacer_12);


        gridLayout_4->addLayout(horizontalLayout_19, 5, 0, 1, 1);

        checkAcResample = new QCheckBox(groupResample);
        checkAcResample->setObjectName(QString::fromUtf8("checkAcResample"));

        gridLayout_4->addWidget(checkAcResample, 3, 0, 1, 1);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        label_11 = new QLabel(groupResample);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMaximumSize(QSize(220, 16777215));

        horizontalLayout_18->addWidget(label_11);

        labelMaxTime = new QLabel(groupResample);
        labelMaxTime->setObjectName(QString::fromUtf8("labelMaxTime"));

        horizontalLayout_18->addWidget(labelMaxTime);


        gridLayout_4->addLayout(horizontalLayout_18, 4, 0, 1, 1);


        verticalLayout_4->addWidget(groupResample);


        formLayout->setLayout(0, QFormLayout::FieldRole, verticalLayout_4);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionExit);

        retranslateUi(MainWindow);
        QObject::connect(checkResample, SIGNAL(clicked(bool)), groupResample, SLOT(setVisible(bool)));
        QObject::connect(checkTraceScope, SIGNAL(clicked(bool)), groupTraceScope, SLOT(setVisible(bool)));
        QObject::connect(checkStandard, SIGNAL(clicked(bool)), groupTraceHeader, SLOT(setVisible(bool)));
        QObject::connect(checkStandard, SIGNAL(clicked(bool)), groupChoose, SLOT(setVisible(bool)));
        QObject::connect(btnAD, SIGNAL(clicked(bool)), groupOption, SLOT(setVisible(bool)));
        QObject::connect(checkAcResample, SIGNAL(clicked(bool)), label_14, SLOT(setVisible(bool)));
        QObject::connect(checkAcResample, SIGNAL(clicked(bool)), listTimeRange, SLOT(setVisible(bool)));
        QObject::connect(checkAcResample, SIGNAL(clicked(bool)), toolAddTime, SLOT(setVisible(bool)));
        QObject::connect(checkAcResample, SIGNAL(clicked(bool)), toolRemoveTime, SLOT(setVisible(bool)));
        QObject::connect(checkAcResample, SIGNAL(clicked(bool)), label_11, SLOT(setVisible(bool)));
        QObject::connect(checkAcResample, SIGNAL(clicked(bool)), labelMaxTime, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SEGY&SU Convert 2", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "F1", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("MainWindow", "Esc", 0, QApplication::UnicodeUTF8));
        groupInput->setTitle(QApplication::translate("MainWindow", "Input File", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Process File Name:", 0, QApplication::UnicodeUTF8));
        labelOpenFileName->setText(QApplication::translate("MainWindow", "The File Name you opened will display here", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Open file format:", 0, QApplication::UnicodeUTF8));
        radioOriginalIEEE->setText(QApplication::translate("MainWindow", "IEEE", 0, QApplication::UnicodeUTF8));
        radioOriginalIBM->setText(QApplication::translate("MainWindow", "IBM", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Open file byteorder:", 0, QApplication::UnicodeUTF8));
        radioOriginalLittleEndian->setText(QApplication::translate("MainWindow", "LittleEndian", 0, QApplication::UnicodeUTF8));
        radioOriginalBigEndian->setText(QApplication::translate("MainWindow", "BigEndian", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Choose Output Format:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Choose Function:", 0, QApplication::UnicodeUTF8));
        radioFormat->setText(QApplication::translate("MainWindow", "Convert To SEGY", 0, QApplication::UnicodeUTF8));
        radioConvert->setText(QApplication::translate("MainWindow", "Convert To SU", 0, QApplication::UnicodeUTF8));
        btnAD->setText(QApplication::translate("MainWindow", "Advanced Function", 0, QApplication::UnicodeUTF8));
        groupOption->setTitle(QApplication::translate("MainWindow", "Choose Function ", 0, QApplication::UnicodeUTF8));
        checkStandard->setText(QApplication::translate("MainWindow", " Standard or Non-Standard Trace Header", 0, QApplication::UnicodeUTF8));
        checkTraceScope->setText(QApplication::translate("MainWindow", " Trace Scope", 0, QApplication::UnicodeUTF8));
        groupChoose->setTitle(QApplication::translate("MainWindow", "Choose:", 0, QApplication::UnicodeUTF8));
        radioStandard->setText(QApplication::translate("MainWindow", "Standard Trace Header", 0, QApplication::UnicodeUTF8));
        radioNonStandard->setText(QApplication::translate("MainWindow", "Non-Standard Trace Header", 0, QApplication::UnicodeUTF8));
        checkResample->setText(QApplication::translate("MainWindow", " Time Resample", 0, QApplication::UnicodeUTF8));
        checkSkip->setText(QApplication::translate("MainWindow", " Trace Resample", 0, QApplication::UnicodeUTF8));
        groupOutput->setTitle(QApplication::translate("MainWindow", "Output", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Target File Format:", 0, QApplication::UnicodeUTF8));
        radioIEEE->setText(QApplication::translate("MainWindow", "IEEE", 0, QApplication::UnicodeUTF8));
        radioIBM->setText(QApplication::translate("MainWindow", "IBM", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Target File Byteorder:", 0, QApplication::UnicodeUTF8));
        radioLittleEndian->setText(QApplication::translate("MainWindow", "LittleEndian", 0, QApplication::UnicodeUTF8));
        radioBigEndian->setText(QApplication::translate("MainWindow", "BigEndian", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Save File To:", 0, QApplication::UnicodeUTF8));
        btnConvertTo->setText(QString());
        btnConvert->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        groupTraceHeader->setTitle(QApplication::translate("MainWindow", "Trace Header", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Choose Trace Header:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "    Reference:", 0, QApplication::UnicodeUTF8));
        labelReference->setText(QString());
        label_23->setText(QApplication::translate("MainWindow", "Start Position:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "(1~240)", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MainWindow", "Data type:", 0, QApplication::UnicodeUTF8));
        labelBlank->setText(QString());
        labelBlank2->setText(QString());
        labelBlank3->setText(QString());
        label_19->setText(QApplication::translate("MainWindow", "Set the Range:", 0, QApplication::UnicodeUTF8));
        toolTHAdd->setText(QString());
        toolTHRemove->setText(QString());
        groupTraceScope->setTitle(QApplication::translate("MainWindow", "Trace Scope", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "File Max Trace Number:", 0, QApplication::UnicodeUTF8));
        labelMaxTrace->setText(QString());
        label_13->setText(QApplication::translate("MainWindow", "Add the trace rage: ", 0, QApplication::UnicodeUTF8));
        toolAdd->setText(QString());
        toolRemove->setText(QApplication::translate("MainWindow", "-", 0, QApplication::UnicodeUTF8));
        groupResample->setTitle(QApplication::translate("MainWindow", "Time Resample", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MainWindow", "Current Sample Interval:", 0, QApplication::UnicodeUTF8));
        labelCurrentInterval->setText(QString());
        label_17->setText(QApplication::translate("MainWindow", "(us)", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MainWindow", "Change to:", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("MainWindow", "(us) ", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "Add the time scale (ms): ", 0, QApplication::UnicodeUTF8));
        toolAddTime->setText(QString());
        toolRemoveTime->setText(QApplication::translate("MainWindow", "-", 0, QApplication::UnicodeUTF8));
        checkAcResample->setText(QApplication::translate("MainWindow", "Accuracy Resample", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "File Max Trace Time Range (ms): 0 ~", 0, QApplication::UnicodeUTF8));
        labelMaxTime->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QJD_CONVERT_MAINWINDOW_H
