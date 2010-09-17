/***********************************************************************************
* xAlertU: Plasmoid inspired by iAlertU.
* Copyright (C) 2009 Alvaro Tanarro <tanarro@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
***********************************************************************************/

#ifndef XALERTU_HEADER
#define XALERTU_HEADER

//Plasma Applet headers
#include <KDE/KIcon>
#include <KDE/KConfigDialog>
#include <KDE/Plasma/Applet>
#include <KDE/Plasma/Svg>
#include <KDE/KLocale>

#include <plasma/widgets/iconwidget.h>
#include <plasma/svg.h>
#include <plasma/theme.h>

#include <qt4/QtGui/QPainter>
#include <qt4/QtGui/QFontMetrics>
#include <qt4/QtCore/QSizeF>
#include <QtGui>

//KDE notification system
#include <KDE/KNotification>

//DBus
#include <QDBusInterface>
#include <QDBusReply>

//Threading
#include <qt4/Qt/qthread.h> 

//Motion sensor
#include "usbpad.h"

//Phonon
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/Path>
#include <Phonon/AudioOutput>
#include <Phonon/Global>

//Configuration UI
#include "ui_xalertuConfigWebCam.h"
#include "ui_xalertuConfigSound.h"
#include "ui_xalertuConfigRemote.h"
#include "ui_xalertuConfigMotionSensor.h"
#include "ui_xalertuConfig.h"

class QSizeF;
class QGraphicsSceneMouseEvent;

class xAlertUThread : public QThread {
  Q_OBJECT
  public:
    // Constructor
    xAlertUThread(const char *dev);
    virtual void run();
//     virtual void start();
    virtual void stop();
  private:
    usbpad *motion_sensor;
    bool stop_flag;
    const char *device;
  signals:
    void fire( void );
};

class xAlertU : public Plasma::Applet
{
    Q_OBJECT
    Plasma::IconWidget *_pIcon;
    xAlertUThread th_motion_sensor;
    
  public:
    // Basic Create/Destroy
    xAlertU(QObject *parent, const QVariantList &args);
    ~xAlertU();
    
    void arm();
    void disarm();
    void paintInterface(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         const QRect& contentsRect);
                         void init();     
  public slots:
    void pressed();      
    void fireAlarm();
    void createConfigurationInterface(KConfigDialog *parent);
    
  private slots:
    void aboutToFinish();
    
  private:
    Plasma::Svg m_svg;
    KIcon m_icon;
    
    Phonon::MediaObject *sound_arm, *sound_disarm, *sound_fire;                    
    Phonon::AudioOutput *audioOutput;
			
    //Config dialog
    Ui::xalertuConfigWebCam uiWebCam;
    Ui::xalertuConfigSound uiSound;
    Ui::xalertuConfigRemote uiRemote;
    Ui::xalertuConfigMotionSensor uiMotionSensor;
    Ui::xalertuConfig ui;
    
    bool m_mouse_pressed;
    bool armed;
    
    // configuration params
    bool lock_screen;
    
    bool sound_enabled;
    int sound_level;
    
    QString motion_sensor_device;
    QString webcam_command;
    
    //KNotification *alarm_notification;

    inline void readConfig();
    inline void writeConfig();
    
  protected slots:
    void configAccepted();
};

// This links the applet to the .desktop file
K_EXPORT_PLASMA_APPLET(xalertu, xAlertU)
#endif
