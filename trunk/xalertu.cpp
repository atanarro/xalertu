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

#include "xalertu.h"

//mail
#include <libesmtp.h>

/**
Thread
*/

xAlertUThread::xAlertUThread(const char *dev = "/dev/input/js0") {
  device = dev;
}

void xAlertUThread::run()
{
  stop_flag = false;
  motion_sensor = new usbpad(device);
  while(!stop_flag) {
    motion_sensor->updateData();
    if (motion_sensor->hasChanged())
    {
      // Fire notification
      emit fire();    
      qDebug( "Ping!" );
      break;
    }
    usleep(1);
  }
}

void xAlertUThread::stop() {
  stop_flag = true;
}

/**
  Plasmoid
*/


xAlertU::xAlertU(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_svg(this),
    m_icon("plasma")
{
    m_mouse_pressed = false;
    armed = false;

    setHasConfigurationInterface (true);

    setAspectRatioMode(Plasma::Square);
    resize(200, 200);
}


xAlertU::~xAlertU()
{
    if (hasFailedToLaunch()) {
      qWarning() << "xAlertU has failed to launch";
    } else {
        // Save settings
    }
    if (_pIcon) delete _pIcon;
    
}

void xAlertU::arm()
{
    QString eventId;
    QString message;
    qDebug("arm()\n");    armed = true;
    //_pIcon->setIcon("/home/alvaro/c/xAlertU/images/menuBarIconRed");
    _pIcon->setIcon(KIcon("user-busy"));

    message = i18n("xAlarmU activated");
    
    eventId = "xalarmu-activated";
    
    // Fire notification
    alarm_notification->event(
      eventId,
      message,
      QPixmap(),
      0,
      //KNotification::CloseOnTimeout,
      KNotification::Persistent,
      KComponentData("xalertu-plasmoid", "xalertu-plasmoid", KComponentData::SkipMainComponentRegistration)
      );
      
    th_motion_sensor.start();
}

void xAlertU::fireAlarm() {
  QString eventId;
  QString message;
  qDebug("fireAlarm()\n");
  
  message = i18n("Alarm fired");
  eventId = "xalarmu-firealarm";

  //take a photo
  
  //Send email
  /*smtp_session_t my_smtp_session = smtp_create_session();
  smtp_message_t my_smtp_message = smtp_add_message(my_smtp_session);
  smtp_set_server(my_smtp_session, "smtp.gmail.com:25");
  smtp_add_recipient(my_smtp_message, "tanarro@gmail.com");
  */
  
  // Fire notification
  alarm_notification->event(
    eventId,
    message,
    QPixmap(),
    0,
    KNotification::CloseOnTimeout,
    KComponentData("xalertu-plasmoid", "xalertu-plasmoid", KComponentData::SkipMainComponentRegistration)
    );
  
}

void xAlertU::disarm()
{
  QString eventId;
  QString message;
  qDebug("disarm()\n");
  
  th_motion_sensor.stop();
  th_motion_sensor.wait();
  
  //alarm_notification->update();

  armed = false;
  //_pIcon->setIcon("/home/alvaro/c/xAlertU/images/menuBarIconBlack.svgz");
  _pIcon->setIcon(KIcon("user-offline"));
  
  message = i18n("xAlarmU deactivated");
  
  eventId = "xalarmu-deactivated";
  
  // Fire notification
  //KNotification::
  //alarm_notification = new KNotification();
  //media->setCurrentSource(Phonon::MediaSource(QString("/home/alvaro/c/xAlertU/sounds/caralarm.ogg")));
  //media->setCurrentSource(QString("/home/alvaro/c/xAlertU/sounds/caralarm.ogg"));
  //media->setCurrentSource();
  //setCurrentSource("/home/alvaro/c/xAlertU/sounds/caralarm.ogg");
  //media->play();
  //Phonon::MediaObject::play();
  alarm_notification->event(
    eventId,
    message,
    QPixmap(),
    0,
    KNotification::CloseOnTimeout,
    KComponentData("xalertu-plasmoid", "xalertu-plasmoid", KComponentData::SkipMainComponentRegistration)
    );
}

void xAlertU::pressed()
{
  qDebug("pressed()\n");
  if (armed) disarm();
  else arm();
}

void xAlertU::init()
{
    // A small demonstration of the setFailedToLaunch function
    if (m_icon.isNull()) {
      setFailedToLaunch(true, i18n("xAlertU failed to launch..."));
    }
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    // create icon
    //_pIcon = new Plasma::IconWidget(KIcon("xalertu/menuBarIconBlack.svgz"), QString(), this);
    _pIcon = new Plasma::IconWidget(KIcon("user-offline"), QString(), this);
    layout->addItem(_pIcon);
    registerAsDragHandle(_pIcon);
    connect(_pIcon, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(&th_motion_sensor, SIGNAL(fire()), this, SLOT(fireAlarm()));
}


void xAlertU::paintInterface(QPainter *p,
        const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);
}

void xAlertU::createConfigurationInterface(KConfigDialog *aParent)
{
  QWidget *widget = new QWidget;
  ui.setupUi (widget);
  
  connect(aParent, SIGNAL(accepted()), this, SLOT(configAccepted()));
  /*
  //Sound
  ui.soundEnabled->setChecked (m_sound_enabled);
  ui.soundVolumeLabel->setEnabled (m_sound_enabled);
  ui.soundVolume->setEnabled (m_sound_enabled);
  ui.soundVolume->setSliderPosition (m_sound_volume);
  */
  aParent->addPage(widget, i18n("General"), icon());
}

void xAlertU::configAccepted()
{
  KConfigGroup cg = config();
  
  //Sound
  /*m_sound_enabled = (ui.soundEnabled->checkState () == Qt::Checked);
  cg.writeEntry ("SoundEnabled", m_sound_enabled);
  if ( m_sound_enabled )
  {
    m_sound_volume = ui.soundVolume->value ();
    cg.writeEntry ("SoundVolume", m_sound_volume);
    initSound();
  }
  
  //mouse - undo the mouse clicked
  m_mouse_pressed = false;
  */
  update();
}

void xAlertU::readConfiguration()
{
  KConfigGroup cg = config();
  
  //Sound
  /*m_sound_enabled = cg.readEntry ("SoundEnabled", true);
  m_sound_volume = cg.readEntry ("SoundVolume", 100);*/
}

#include "xalertu.moc"
