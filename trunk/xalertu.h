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
#include <KDE/KConfig>
#include <KDE/KShell>
#include <KDE/Plasma/Applet>
//#include <KDE/Plasma/Svg>
#include <KDE/KLocale>
#include <KDE/KProcess>
#include <KDE/KRun>

#include <plasma/widgets/iconwidget.h>
// #include <plasma/svg.h>
#include <plasma/theme.h>
#include <Plasma/DataEngine>

#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtCore/QSizeF>
#include <QtGui>
#include <QDirModel>

//KDE notification system
#include <KDE/KNotification>

//DBus
#include <QDBusInterface>
#include <QDBusReply>

//Threading
#include <Qt/qthread.h>

//Motion sensor
#include "usbpad.h"
//Mouse Sensor
#include "mousesensor.h"
//Keyboard Sensor
#include "keyboardsensor.h"

//Phonon
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/Path>
#include <Phonon/AudioOutput>
#include <Phonon/Global>

//Configuration UI
#include "ui_xalertuConfigWebCam.h"
#include "ui_xalertuConfigScreenshot.h"
#include "ui_xalertuConfigSound.h"
#include "ui_xalertuConfigRemote.h"
#include "ui_xalertuConfigMotionSensor.h"
#include "ui_xalertuConfigEmail.h"
#include "ui_xalertuConfig.h"

class QSizeF;
class QGraphicsSceneMouseEvent;


class xAlertUThread : public QThread {
  Q_OBJECT
  public:
    // Constructor
    xAlertUThread(const char *dev);
    void setDevice(const char *dev);
    void setSensibility(int s);
    virtual void run();
    virtual void stop();
  private:
    usbpad *motion_sensor;
    bool stop_flag;
    char device[256];
    int sensibility;
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
    
  public Q_SLOTS:
    void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);
    
  private slots:
    void aboutToFinish();
    
  private:
    Plasma::Svg m_svg;
    KIcon m_icon;
    
    Phonon::MediaObject *sound_arm, *sound_disarm, *sound_fire;                    
    Phonon::AudioOutput *audioOutput;
    Phonon::MediaObject *mediaObject;
			
    //Config dialog
    Ui::xalertuConfigWebCam uiWebCam;
    Ui::xalertuConfigScreenshot uiScreenshot;
    Ui::xalertuConfigSound uiSound;
    Ui::xalertuConfigRemote uiRemote;
    Ui::xalertuConfigEmail uiEmail;
    Ui::xalertuConfigMotionSensor uiMotionSensor;
    Ui::xalertuConfig ui;
    
    bool m_mouse_pressed;
    bool armed;
    bool ac_plugged;
    qint8 ac_cont;
    qreal alarm_counter;
    
    KProcess m_proc;
    
    KProcess m_procWC;
    KProcess m_procSC;
    KProcess m_procMail;
    
    MouseSensor *mouse_sensor;
    KeyboardSensor *keyboard_sensor;
    
    // configuration params
    bool lock_screen;
    bool activate_motion_sensor;
    bool activate_mouse_sensor;
    bool activate_keyboard_sensor;
    bool activate_ac_sensor;
    
    bool sound_enabled;
    qreal sound_level;
    bool alarm_loop;
    qreal alarm_duration;
    
    QString last_armed;
    QString last_send;
    QString last_activation;
    QString type;
    quint32 times_activated;
    
    QString motion_sensor_device;
    int motion_sensor_sensibility;
    
    QString webcam_command;
    QString webcam_input;
    QString webcam_size;
    QString webcam_format;
    QString webcam_dir;
    
    QString screenshot_command;
    QString screenshot_input;
    QString screenshot_size;
    QString screenshot_format;
    QString screenshot_dir;
    
    QString mailto;
    bool email_alarms;
    bool attach_screenshot;
    bool attach_webcam_image;
    
    //KNotification *alarm_notification;
    
    inline void readConfig();
    inline void writeConfig();
    void setVolume();
    
  protected slots:
    void configAccepted();
    void configDiscarded();
    void soundEnabledConfigChanged(int state);
    void alarmLoopConfigChanged(int state);
    void emailConfigChanged(int state);
    void emailConfigTestButtonPressed();
    void screenshotConfigTestButtonPressed();
    void screenshotConfigChanged();
    void webCamConfigTestButtonPressed();
    void webCamConfigChanged();
    void setImageWC();
    void setImageSC();
    void runWebCamCommand();
    void runScreenshotCommand();
    void runEmailCommand();
    void disconnectSecuence();
};

// This links the applet to the .desktop file
K_EXPORT_PLASMA_APPLET(xalertu, xAlertU)
#endif
