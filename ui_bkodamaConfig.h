#include <kdialog.h>
#include <klocale.h>

/********************************************************************************
** Form generated from reading ui file 'bkodamaConfig.ui'
**
** Created: Sun Jun 28 14:19:17 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_BKODAMACONFIG_H
#define UI_BKODAMACONFIG_H

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
#include "ktabwidget.h"

QT_BEGIN_NAMESPACE

class Ui_bkodamaConfig
{
public:
    QGridLayout *gridLayout;
    KTabWidget *ktabwidget;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QCheckBox *soundEnabled;
    QLabel *soundVolumeLabel;
    QSlider *soundVolume;

    void setupUi(QWidget *bkodamaConfig)
    {
        if (bkodamaConfig->objectName().isEmpty())
            bkodamaConfig->setObjectName(QString::fromUtf8("xalertuConfig"));
        bkodamaConfig->resize(323, 183);
        gridLayout = new QGridLayout(bkodamaConfig);
        gridLayout->setMargin(5);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ktabwidget = new KTabWidget(bkodamaConfig);
        ktabwidget->setObjectName(QString::fromUtf8("ktabwidget"));
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        soundEnabled = new QCheckBox(tab_2);
        soundEnabled->setObjectName(QString::fromUtf8("soundEnabled"));

        gridLayout_2->addWidget(soundEnabled, 0, 1, 1, 1);

        soundVolumeLabel = new QLabel(tab_2);
        soundVolumeLabel->setObjectName(QString::fromUtf8("soundVolumeLabel"));
        soundVolumeLabel->setEnabled(false);

        gridLayout_2->addWidget(soundVolumeLabel, 1, 0, 1, 1);

        soundVolume = new QSlider(tab_2);
        soundVolume->setObjectName(QString::fromUtf8("soundVolume"));
        soundVolume->setEnabled(false);
        soundVolume->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(soundVolume, 1, 1, 1, 1);

        ktabwidget->addTab(tab_2, QString());

        gridLayout->addWidget(ktabwidget, 0, 0, 1, 1);


        retranslateUi(bkodamaConfig);

        QMetaObject::connectSlotsByName(bkodamaConfig);
    } // setupUi

    void retranslateUi(QWidget *bkodamaConfig)
    {
        bkodamaConfig->setWindowTitle(tr2i18n("Configure BKodama", 0));
        label_7->setText(tr2i18n("Sound enabled:", 0));
        soundEnabled->setText(QString());
        soundVolumeLabel->setText(tr2i18n("Volume:", 0));
        ktabwidget->setTabText(ktabwidget->indexOf(tab_2), tr2i18n("Sound", 0));
        Q_UNUSED(bkodamaConfig);
    } // retranslateUi

};

namespace Ui {
    class bkodamaConfig: public Ui_bkodamaConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BKODAMACONFIG_H

