/********************************************************************************
** Form generated from reading UI file 'help.ui'
**
** Created: Thu Aug 4 15:59:22 2011
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELP_H
#define UI_HELP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_help
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *help)
    {
        if (help->objectName().isEmpty())
            help->setObjectName(QString::fromUtf8("help"));
        help->resize(400, 300);
        pushButton = new QPushButton(help);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(130, 90, 85, 27));

        retranslateUi(help);

        QMetaObject::connectSlotsByName(help);
    } // setupUi

    void retranslateUi(QWidget *help)
    {
        help->setWindowTitle(QApplication::translate("help", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("help", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class help: public Ui_help {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_H
