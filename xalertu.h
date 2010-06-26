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

//Threading
#include <qt4/Qt/qthread.h> 

//Motion sensor
#include "usbpad.h"

//Phonon
// #include <KDE/Phonon/MediaObject>

//Configuration
#include "ui_xalertuConfig.h"

class QSizeF;
class QGraphicsSceneMouseEvent;

class xAlertUThread : public QThread {
  Q_OBJECT
  public:
    // Constructor
    xAlertUThread(const char *dev);
    virtual void run();
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
    //void createConfigurationInterface(KConfigDialog *parent);
    //The paintInterface procedure paints the applet to screen
    void paintInterface(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         const QRect& contentsRect);
                         void init();     
  public slots:
    void pressed();      
    void fireAlarm();
    void createConfigurationInterface(KConfigDialog *parent);
    
  private:
    Plasma::Svg m_svg;
    KIcon m_icon;
    
    QStringList     m_code_list;
    
    //Config dialog
    Ui::xalertuConfig ui;
    
    bool m_mouse_pressed;
    bool armed;
    KNotification *alarm_notification;
//     Phonon::MediaObject *media;
    inline void readConfiguration();
    
  protected slots:
    void configAccepted();
};

// This links the applet to the .desktop file
K_EXPORT_PLASMA_APPLET(xalertu, xAlertU)
#endif
