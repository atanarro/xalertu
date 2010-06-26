/********************************************************************************
** Form generated from reading ui file 'xalertuConfign19958.ui'
**
** Created: Sat Aug 22 17:40:42 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef XALERTUCONFIGN19958_H
#define XALERTUCONFIGN19958_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QWidget>
#include "kdialog.h"
#include "klineedit.h"
#include "ktabwidget.h"

QT_BEGIN_NAMESPACE

class Ui_xalertuConfig
{
  public:
    QGridLayout *gridLayout;
    KTabWidget *ktabwidget;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QLabel *label_11;
    KLineEdit *klineedit;
    QWidget *tab;
    QGridLayout *gridLayout_4;
    QLabel *label_8;
    QCheckBox *soundEnabled;
    QLabel *soundVolumeLabel;
    QSlider *soundVolume;
    QWidget *tab_3;
    QGridLayout *gridLayout_3;
    KLineEdit *klineedit_2;
    QLabel *label_9;
    QLabel *label_10;
    QWidget *tab_4;
    QLabel *label_12;
    KLineEdit *klineedit_3;
    QLabel *label_13;
    KDialog *kdialog;
    
    void setupUi(QWidget *xalertuConfig)
    {
      if (xalertuConfig->objectName().isEmpty())
        xalertuConfig->setObjectName(QString::fromUtf8("xalertuConfig"));
      xalertuConfig->resize(443, 193);
      gridLayout = new QGridLayout(xalertuConfig);
      gridLayout->setMargin(5);
      gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
      ktabwidget = new KTabWidget(xalertuConfig);
      ktabwidget->setObjectName(QString::fromUtf8("ktabwidget"));
      tab_2 = new QWidget();
      tab_2->setObjectName(QString::fromUtf8("tab_2"));
      gridLayout_2 = new QGridLayout(tab_2);
      gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
      label_7 = new QLabel(tab_2);
      label_7->setObjectName(QString::fromUtf8("label_7"));
      
      gridLayout_2->addWidget(label_7, 0, 3, 1, 2);
      
      label_11 = new QLabel(tab_2);
      label_11->setObjectName(QString::fromUtf8("label_11"));
      
      gridLayout_2->addWidget(label_11, 1, 5, 1, 1);
      
      klineedit = new KLineEdit(tab_2);
      klineedit->setObjectName(QString::fromUtf8("klineedit"));
      
      gridLayout_2->addWidget(klineedit, 0, 5, 1, 1);
      
      ktabwidget->addTab(tab_2, QString());
      tab = new QWidget();
      tab->setObjectName(QString::fromUtf8("tab"));
      gridLayout_4 = new QGridLayout(tab);
      gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
      label_8 = new QLabel(tab);
      label_8->setObjectName(QString::fromUtf8("label_8"));
      
      gridLayout_4->addWidget(label_8, 0, 0, 1, 1);
      
      soundEnabled = new QCheckBox(tab);
      soundEnabled->setObjectName(QString::fromUtf8("soundEnabled"));
      
      gridLayout_4->addWidget(soundEnabled, 0, 1, 1, 1);
      
      soundVolumeLabel = new QLabel(tab);
      soundVolumeLabel->setObjectName(QString::fromUtf8("soundVolumeLabel"));
      soundVolumeLabel->setEnabled(false);
      
      gridLayout_4->addWidget(soundVolumeLabel, 1, 0, 1, 1);
      
      soundVolume = new QSlider(tab);
      soundVolume->setObjectName(QString::fromUtf8("soundVolume"));
      soundVolume->setEnabled(false);
      soundVolume->setOrientation(Qt::Horizontal);
      
      gridLayout_4->addWidget(soundVolume, 1, 1, 1, 1);
      
      ktabwidget->addTab(tab, QString());
      tab_3 = new QWidget();
      tab_3->setObjectName(QString::fromUtf8("tab_3"));
      gridLayout_3 = new QGridLayout(tab_3);
      gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
      klineedit_2 = new KLineEdit(tab_3);
      klineedit_2->setObjectName(QString::fromUtf8("klineedit_2"));
      
      gridLayout_3->addWidget(klineedit_2, 1, 1, 1, 1);
      
      label_9 = new QLabel(tab_3);
      label_9->setObjectName(QString::fromUtf8("label_9"));
      
      gridLayout_3->addWidget(label_9, 1, 0, 1, 1);
      
      label_10 = new QLabel(tab_3);
      label_10->setObjectName(QString::fromUtf8("label_10"));
      
      gridLayout_3->addWidget(label_10, 2, 1, 1, 1);
      
      ktabwidget->addTab(tab_3, QString());
      tab_4 = new QWidget();
      tab_4->setObjectName(QString::fromUtf8("tab_4"));
      label_12 = new QLabel(tab_4);
      label_12->setObjectName(QString::fromUtf8("label_12"));
      label_12->setGeometry(QRect(30, 30, 377, 83));
      klineedit_3 = new KLineEdit(tab_4);
      klineedit_3->setObjectName(QString::fromUtf8("klineedit_3"));
      klineedit_3->setGeometry(QRect(33, 0, 377, 26));
      label_13 = new QLabel(tab_4);
      label_13->setObjectName(QString::fromUtf8("label_13"));
      label_13->setGeometry(QRect(0, 0, 29, 26));
      ktabwidget->addTab(tab_4, QString());
      
      gridLayout->addWidget(ktabwidget, 0, 0, 1, 1);
      
      kdialog = new KDialog(xalertuConfig);
      kdialog->setObjectName(QString::fromUtf8("kdialog"));
      
      gridLayout->addWidget(kdialog, 1, 0, 1, 1);
      
      
      retranslateUi(xalertuConfig);
      
      ktabwidget->setCurrentIndex(2);
      
      
      QMetaObject::connectSlotsByName(xalertuConfig);
    } // setupUi
    
    void retranslateUi(QWidget *xalertuConfig)
    {
      xalertuConfig->setWindowTitle(QApplication::translate("xalertuConfig", "Configure xAlertU", 0, QApplication::UnicodeUTF8));
      label_7->setText(QApplication::translate("xalertuConfig", "Path:", 0, QApplication::UnicodeUTF8));
      label_11->setText(QApplication::translate("xalertuConfig", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
      "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
      "p, li { white-space: pre-wrap; }\n"
      "</style></head><body style=\" font-family:'Lucida Grande'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
      "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">You can change the path to your motion sensor.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
      klineedit->setText(QApplication::translate("xalertuConfig", "/dev/input/js0", 0, QApplication::UnicodeUTF8));
      ktabwidget->setTabText(ktabwidget->indexOf(tab_2), QApplication::translate("xalertuConfig", "Motion Sensor", 0, QApplication::UnicodeUTF8));
      label_8->setText(QApplication::translate("xalertuConfig", "Sound enabled:", 0, QApplication::UnicodeUTF8));
      soundEnabled->setText(QString());
      soundVolumeLabel->setText(QApplication::translate("xalertuConfig", "Volume:", 0, QApplication::UnicodeUTF8));
      ktabwidget->setTabText(ktabwidget->indexOf(tab), QApplication::translate("xalertuConfig", "Sound", 0, QApplication::UnicodeUTF8));
      klineedit_2->setText(QApplication::translate("xalertuConfig", "xalertu", 0, QApplication::UnicodeUTF8));
      label_9->setText(QApplication::translate("xalertuConfig", "Command:", 0, QApplication::UnicodeUTF8));
      label_10->setText(QApplication::translate("xalertuConfig", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
      "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
      "p, li { white-space: pre-wrap; }\n"
      "</style></head><body style=\" font-family:'Lucida Grande'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
      "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">You can choose the command to execute when alarm is </span></p>\n"
      "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">fired to record an small video or to take a picture.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
      ktabwidget->setTabText(ktabwidget->indexOf(tab_3), QApplication::translate("xalertuConfig", "WebCam", 0, QApplication::UnicodeUTF8));
      label_12->setText(QApplication::translate("xalertuConfig", "Path:", 0, QApplication::UnicodeUTF8));
      klineedit_3->setText(QApplication::translate("xalertuConfig", "nuse", 0, QApplication::UnicodeUTF8));
      label_13->setText(QApplication::translate("xalertuConfig", "Path:", 0, QApplication::UnicodeUTF8));
      ktabwidget->setTabText(ktabwidget->indexOf(tab_4), QApplication::translate("xalertuConfig", "Remote", 0, QApplication::UnicodeUTF8));
      Q_UNUSED(xalertuConfig);
      } // retranslateUi
      
      };
      
      namespace Ui {
        class xalertuConfig: public Ui_xalertuConfig {};
      } // namespace Ui
      
      QT_END_NAMESPACE
      
      #endif // XALERTUCONFIGN19958_H
      