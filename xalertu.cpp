/**********************************************************************************
*                    xAlertU: Plasmoid inspired by iAlertU                        *
***********************************************************************************
*                                                                                 *
*             Copyright (C) 2009 Alvaro Tanarro <tanarro@gmail.com>               *
*                                                                                 *
* This program is free software; you can redistribute it and/or                   *
* modify it under the terms of the GNU General Public License                     *
* as published by the Free Software Foundation; either version 2                  *
* of the License, or (at your option) any later version.                          *
*                                                                                 *
* This program is distributed in the hope that it will be useful,                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   *
* GNU General Public License for more details.                                    *
*                                                                                 *
* You should have received a copy of the GNU General Public License               *
* along with this program; if not, write to the Free Software                     *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. *
*                                                                                 *
***********************************************************************************/

#include "xalertu.h"

//mail
#include <libesmtp.h>

//time
#include <time.h>

#define LOCK_SCREEN_CONFIG_STRING "lock_screen"
#define MOTION_SENSOR_DEVICE_CONFIG_STRING "motion_sensor_path"
#define WEBCAM_COMMAND_CONFIG_STRING "take_photo_command"
#define SOUND_LEVEL_CONFIG_STRING "sound_volume_level"
#define SOUND_ENABLED_CONFIG_STRING "sound_enabled"

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
    resize(256, 256);
}


xAlertU::~xAlertU()
{
    if (hasFailedToLaunch()) {
      qWarning() << "xAlertU has failed to launch";
    } else {
        // Save settings
        writeConfig();
    }
    if (_pIcon) delete _pIcon;
    
}

void xAlertU::arm()
{
    QString eventId;
    QString message;
    qDebug("arm()\n");
    armed = true;
    _pIcon->setIcon("armed");
        
    
    message = i18n("xAlarmU activated");
    
    eventId = "xalarmu-activated";
    
    if (sound_enabled) {
      sound_disarm->stop();
      sound_fire->stop();
      sound_arm->play();
    }
    //Lock Screen
    if (lock_screen) {
      //open DBus interface
      QDBusInterface dBusIf("org.kde.screensaver", "/ScreenSaver", "");
      if (!dBusIf.isValid())  {
        printf("cannot open dbus interface\n");
      } else {
        // call Lock method
        QDBusReply<void> dBusReply = dBusIf.call("Lock");
        if ( !dBusReply.isValid() )  {
          qDebug("error %s : %s\n", dBusReply.error().name().toAscii().data(), 
                dBusReply.error().message().toAscii().data() );
        }
        qDebug("screen locked\n");
      }
    }
      
    th_motion_sensor.start();
}

void xAlertU::fireAlarm() {
  QString eventId;
  QString message;
  
  //char filename[50];
  char *rcommand;
  
  time_t tstamp;
  struct tm *local;
  QByteArray bytes;
  
  QString aux = webcam_command;
  
  qDebug("fireAlarm()\n");
  
  message = i18n("Alarm fired");
  eventId = "xalarmu-firealarm";

  //take a photo
  
  tstamp = time(NULL);
  local = localtime(&tstamp);
  
//   sprintf(filename, "\"${HOME}/xalertu_%s", asctime(local));
//   filename[strlen(filename)-1]='\0';
//   sprintf(filename, "%s.mpg\"", filename);
  QString temp = asctime(local);
    
  aux.replace( "%t", temp.left(temp.size()-1).replace(" ", "\\ ").replace(":","\\:") );
  bytes = aux.toAscii();
  rcommand = bytes.data();
  
  //take a video
  printf("Taking a video/photo...\n");
  printf("$ %s\n", rcommand);
  
  if (system(rcommand) == -1)
    qDebug("Unable to fork the command.\n");
  
  //Send email
  /*smtp_session_t my_smtp_session = smtp_create_session();
  smtp_message_t my_smtp_message = smtp_add_message(my_smtp_session);
  smtp_set_server(my_smtp_session, "smtp.gmail.com:25");
  smtp_add_recipient(my_smtp_message, "tanarro@gmail.com");
  */
  
  // Fire notification
  /*alarm_notification->event(
    eventId,
    message,
    QPixmap(),
    0,
    KNotification::CloseOnTimeout,
    KComponentData("xalertu-plasmoid", "xalertu-plasmoid", 
    KComponentData::SkipMainComponentRegistration)
    );*/
  if (sound_enabled) {
    sound_disarm->stop();
    sound_arm->stop();
    sound_fire->play();
  }
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
  _pIcon->setIcon("disarmed");
  //_pIcon->setIcon(KIcon("user-offline");
  
  message = i18n("xAlarmU deactivated");
  
  eventId = "xalarmu-deactivated";
  
  // Fire notification
  /*KNotification::
  alarm_notification = new KNotification();
  media->setCurrentSource(
    Phonon::MediaSource(QString("/usr/share/sounds/caralarm.ogg")));
  media->setCurrentSource(QString("/usr/share/sounds/caralarm.ogg"));
  media->setCurrentSource();
  setCurrentSource("/usr/share/sounds/caralarm.ogg");
  media->play();
  Phonon::MediaObject::play();
  alarm_notification->event(
    eventId,
    message,
    QPixmap(),
    0,
    KNotification::CloseOnTimeout,
    KComponentData("xalertu-plasmoid", "xalertu-plasmoid",
      KComponentData::SkipMainComponentRegistration)
    );*/
  
  if (sound_enabled) {
    sound_arm->stop();
    sound_fire->stop();
    sound_disarm->play();
  }
  
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
    
    readConfig();
    
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    _pIcon = new Plasma::IconWidget(KIcon("disarmed"), QString(), this);
    
    layout->addItem(_pIcon);
    registerAsDragHandle(_pIcon);
    connect(_pIcon, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(&th_motion_sensor, SIGNAL(fire()), this, SLOT(fireAlarm()));
    
    sound_arm = Phonon::createPlayer(Phonon::NotificationCategory, 
				     Phonon::MediaSource("/usr/share/sounds/arm.ogg"));
    /*sound_arm = Phonon::createPlayer(Phonon::NotificationCategory, 
				     Phonon::MediaSource("arm.ogg"));*/
    
    sound_disarm = Phonon::createPlayer(Phonon::NotificationCategory, 
					Phonon::MediaSource("/usr/share/sounds/disarm.ogg"));
    sound_fire = Phonon::createPlayer(Phonon::NotificationCategory, 
				      Phonon::MediaSource("/usr/share/sounds/caralarm.ogg"));
    audioOutput = new Phonon::AudioOutput(Phonon::NotificationCategory, this);

    Phonon::createPath(sound_arm, audioOutput);
    Phonon::createPath(sound_disarm, audioOutput);
    Phonon::createPath(sound_fire, audioOutput);
    
    //Phonon::AudioOutput ao = new Phonon::AudioOutput();
    
    //
    audioOutput->setVolume(100);
    
    connect(sound_fire, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
    
}

void xAlertU::aboutToFinish()
{
  qDebug("aboutToFinish()\n");
  sound_fire->enqueue(Phonon::MediaSource("/home/alvaro/c/xalertu/sounds/caralarm.ogg"));
}



void xAlertU::paintInterface(QPainter *p,
        const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    Q_UNUSED(option)
    Q_UNUSED(contentsRect)
    
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);
}

void xAlertU::createConfigurationInterface(KConfigDialog *aParent)
{
  
  qDebug("createConfigurationInterface()\n");
    
  QWidget *confGeneralWidget = new QWidget;
  ui.setupUi (confGeneralWidget);
  
  QWidget *confMotionSensorWidget = new QWidget;
  uiMotionSensor.setupUi (confMotionSensorWidget);
  
  QWidget *confSoundWidget = new QWidget;
  uiSound.setupUi (confSoundWidget);
  
  QWidget *confWebCamWidget = new QWidget;
  uiWebCam.setupUi (confWebCamWidget);
  
  QWidget *confRemoteWidget = new QWidget;
  uiRemote.setupUi (confRemoteWidget);
  
  /*QWidget *confAlarmWidget = new QWidget;
  ui.setupUi (confAlarmWidget);*/
  
  aParent->addPage(confGeneralWidget, i18n("General"), icon());
  aParent->addPage(confMotionSensorWidget, i18n("Motion sensor"), "input-gaming");
  aParent->addPage(confSoundWidget, i18n("Sound"), "preferences-desktop-sound");
  aParent->addPage(confWebCamWidget, i18n("WebCam"), "camera-web");
  //aParent->addPage(confRemoteWidget, i18n("Remote"), "infrared-remote");
  //aParent->addPage(confAlarmWidget, i18n("Alarm"), "preferences-desktop-notification-bell");
  
  connect(aParent, SIGNAL(accepted()), this, SLOT(configAccepted()));
  
  ui.lockSesion->setChecked(lock_screen);
  
  uiSound.soundEnabled->setChecked(sound_enabled);
  //uiSound.soundEnabled->addAction(uiSound.soundVolume->setEnabled(false));
  uiSound.soundVolume->setValue(sound_level);

  uiMotionSensor.motionsensorPath->setText(motion_sensor_device);
  uiWebCam.webcamCommand->setText(webcam_command);
  
}

void xAlertU::configAccepted()
{
  qDebug("configAccepted()\n");
  
  lock_screen = (ui.lockSesion->checkState() == Qt::Checked);
  
  sound_enabled = (uiSound.soundEnabled->checkState() == Qt::Checked);
  sound_level = uiSound.soundVolume->value();
  
  motion_sensor_device = uiMotionSensor.motionsensorPath->text();
  webcam_command = uiWebCam.webcamCommand->text();
  
  /*
  printf("webcamCommand: ");
  printf("%s", uiWebCam.webcamCommand->text());
  printf("\n");
  */
  
  writeConfig();
  
  update();
}

void xAlertU::readConfig()
{
  qDebug("readConfig()\n");
  
  KConfigGroup cg = config();
    
  // cg.readEntry( KEY , DEFAULT_VALUE);
  
  lock_screen = cg.readEntry(LOCK_SCREEN_CONFIG_STRING, true);
  
  motion_sensor_device = cg.readEntry(MOTION_SENSOR_DEVICE_CONFIG_STRING, "/dev/input/js0" );
  webcam_command = cg.readEntry(WEBCAM_COMMAND_CONFIG_STRING, 
				"ffmpeg -f video4linux2 -s vga -r 5 -vframes 1 -i /dev/video0 -y ${HOME}/alarm-fired-%t.png");
  
  sound_level = cg.readEntry(SOUND_LEVEL_CONFIG_STRING, 100);
  sound_enabled = cg.readEntry(SOUND_ENABLED_CONFIG_STRING, true);
  
  /*
  var = cg.readEntry("remote_control_staff", "");
  */
    
}

void xAlertU::writeConfig() {
  
  qDebug("writeConfig()\n");
  
  KConfigGroup cg = config();
  
  cg.writeEntry(LOCK_SCREEN_CONFIG_STRING, lock_screen);
  
  cg.writeEntry(MOTION_SENSOR_DEVICE_CONFIG_STRING, motion_sensor_device);
  cg.writeEntry(WEBCAM_COMMAND_CONFIG_STRING, webcam_command);
  
  cg.writeEntry(SOUND_LEVEL_CONFIG_STRING, sound_level);
  cg.writeEntry(SOUND_ENABLED_CONFIG_STRING, sound_enabled);
  
  //cg.writeEntry("remote_control_staff", "");
  
  emit configNeedsSaving();
  
}

#include "xalertu.moc"
