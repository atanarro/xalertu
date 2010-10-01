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
#include <stdio.h>

//time
#include <time.h>

#define LOCK_SCREEN_CONFIG_STRING "lock_screen"
#define ACTIVATE_MOTION_SENSOR_CONFIG_STRING "activate_motion_sensor"
#define ACTIVATE_MOUSE_SENSOR_CONFIG_STRING "activate_mouse_sensor"
#define ACTIVATE_AC_SENSOR_CONFIG_STRING "activate_ac_sensor"
#define ACTIVATE_KEYBOARD_SENSOR_CONFIG_STRING "activate_keyboard_sensor"
#define MOTION_SENSOR_DEVICE_CONFIG_STRING "motion_sensor_path"
#define MOTION_SENSOR_SENSIBILITY_CONFIG_STRING "motion_sensor_sensibility"
#define SOUND_LEVEL_CONFIG_STRING "sound_volume_level"
#define SOUND_ENABLED_CONFIG_STRING "sound_enabled"
#define ALARM_LOOP_CONFIG_STRING "alarm_loop"
#define ALARM_DURATION_CONFIG_STRING "alarm_duration"
#define LAST_ARMED_CONFIG_STRING "last_armed"
#define LAST_SEND_CONFIG_STRING "last_send"
#define LAST_ACTIVATION_CONFIG_STRING "last_activation"
#define TYPE_CONFIG_STRING "type"
#define TIMES_ACTIVATED_CONFIG_STRING "times_activated"
#define WEBCAM_COMMAND_CONFIG_STRING "take_photo_command"
#define INPUT_CONFIG_STRING "input"
#define SIZE_CONFIG_STRING "size"
#define FORMAT_CONFIG_STRING "format"
#define DIRECTORY_CONFIG_STRING "directory"
#define SCREENSHOT_COMMAND_CONFIG_STRING "take_screenshot_command"
#define SCINPUT_CONFIG_STRING "scinput"
#define SCSIZE_CONFIG_STRING "scsize"
#define SCFORMAT_CONFIG_STRING "scformat"
#define SCDIRECTORY_CONFIG_STRING "scdirectory"
#define MAILTO_CONFIG_STRING "mail_to"
#define MAIL_ALARMS_CONFIG_STRING "email_alarms"
#define ATTACH_SCREENSHOT_CONFIG_STRING "attach_screenshot"
#define ATTACH_WEBCAM_IMAGE_CONFIG_STRING "attach_webcam_image"

/**
Thread
*/
xAlertUThread::xAlertUThread(const char *dev = "/dev/input/js0") {
  sprintf(device, "%s", dev);
}


void xAlertUThread::run()
{
  stop_flag = false;
  motion_sensor = new usbpad(device, sensibility);
  while(!stop_flag) {
    motion_sensor->updateData();
    if (motion_sensor->hasChanged())
    {
      // Fire notification
      emit fire();    
      qDebug( "Ping!" );
      break;
    }
    usleep(50);
  }
}


void xAlertUThread::setDevice(const char *dev) {
  sprintf(device, "%s", dev);
}


void xAlertUThread::setSensibility(int s) {
  sensibility=s;
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
    
    activate_keyboard_sensor = false;

    setHasConfigurationInterface (true);

    setAspectRatioMode(Plasma::Square);
    resize(256, 256);
    
    ac_cont = 1;
    
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
  if (sound_arm) delete sound_arm;
  if (sound_disarm) delete sound_disarm;
  if (sound_fire) delete sound_fire;
}

void xAlertU::arm()
{
    time_t tstamp;
    struct tm *local;
    qDebug("arm()\n");
    armed = true;
    _pIcon->setIcon("armed");
           
    //stats
    times_activated++;
    tstamp = time(NULL);
    local = localtime(&tstamp);
    last_armed = asctime(local);
    type = "clicked";
    
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
        qDebug("cannot open dbus interface\n");
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
    
    //enable motion sensor
    if (activate_motion_sensor)
    {
      th_motion_sensor.setDevice(motion_sensor_device.toLocal8Bit().data());
      th_motion_sensor.setSensibility(motion_sensor_sensibility);
      th_motion_sensor.start();
    }

    //enable mouse sensor
    if (activate_mouse_sensor)
    {
      mouse_sensor = new MouseSensor();
      connect(mouse_sensor, SIGNAL(fire()), this, SLOT(fireAlarm()));
    }
    
    //enable AC sensor
    if (activate_ac_sensor)
    {
      dataEngine("powermanagement")->connectSource("AC Adapter", this);
    }
    
    //enable keyboard sensor
    if (activate_keyboard_sensor)
    {
      keyboard_sensor = new KeyboardSensor();
    }
    
    
}


void xAlertU::fireAlarm() {
  
  time_t tstamp;
  struct tm *local;
  
  qDebug("fireAlarm()\n");
  
  disconnect(&m_procWC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(setImageWC()));
  disconnect(&m_procSC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(setImageSC()));
  //disable AC sensor
  if (activate_ac_sensor)
  {
    dataEngine("powermanagement")->disconnectSource("AC Adapter", this);
  }
  
  if (activate_mouse_sensor)
    disconnect(mouse_sensor, SIGNAL(fire()), this, SLOT(fireAlarm()));
  
  //stats
  tstamp = time(NULL);
  local = localtime(&tstamp);
  last_activation = asctime(local);
  
  //take a screenshot when webcam command finish
  connect(&m_procWC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(runScreenshotCommand()));
  //runScreenshotCommand();
  
  //Send email when screenshot command finish
  connect(&m_procSC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(runEmailCommand()));
  
  //Disconect the secuence when email command finish
  connect(&m_procMail, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(disconnectSecuence()));
  //runEmailCommand();

  //take a photo
  runWebCamCommand();
  
  if (sound_enabled) {
    //Set Mute off and volume level
    setVolume();
    alarm_counter = 1;
    sound_disarm->stop();
    sound_arm->stop();
    sound_fire->play();
  }
}

void xAlertU::disconnectSecuence()
{
  disconnect(&m_procWC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(runScreenshotCommand()));
  disconnect(&m_procSC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(runEmailCommand()));
  disconnect(&m_procMail, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(disconnectSecuence())); 
}


void xAlertU::runWebCamCommand() { 
  QString aux = webcam_command;
  aux.replace( "%t", 
           last_activation.left(last_activation.size()-1).replace(" ", "\\ ").replace(":","\\:")
           );

  QString command = QDir::cleanPath( KShell::tildeExpand( aux ) );
  qDebug("Taking a video/photo...\n");
  qDebug() << command;
  m_procWC.setShellCommand(command);
  m_procWC.start();
}


void xAlertU::runScreenshotCommand() { 
  QString aux = screenshot_command;
  aux.replace( "%t", 
               last_activation.left(last_activation.size()-1).replace(" ", "\\ ").replace(":","\\:")
               );
               
  QString command = QDir::cleanPath( KShell::tildeExpand( aux ) );
  qDebug("Taking a screenshot...\n");
  qDebug() << command;
  m_procSC.setShellCommand(command);
  m_procSC.start();
}

void xAlertU::runEmailCommand() { 
  QString command = "echo ";
  command += "\""+i18n("xAlertU fired an alarm on %1", last_activation.left(last_activation.size()-1))+"\"";
  command += "|mail -s \"";
  command += i18n("Alert from xAlertU")+ "\"";
  
  if (attach_webcam_image)
  {
    command += " -a \"";
    command += QDir::cleanPath( KShell::tildeExpand( 
    screenshot_dir
    +"/alarm-fired-"
    +last_activation.left(last_activation.size()-1)
    +".png" ) );
    command += "\"";
  }
  
  if (attach_screenshot)
  {
    command += " -a \"";
    command += QDir::cleanPath( KShell::tildeExpand( 
    screenshot_dir
    +"/alarm-fired-screenshot-"
    +last_activation.left(last_activation.size()-1)
    +".jpg" ) );
    command += "\"";
  }
  
  command += " \"" + mailto + "\"";
  qDebug("Sending the email...\n");
  qDebug() << command;
  m_procMail.setShellCommand(command);
  m_procMail.start();
}

void xAlertU::disarm()
{
  qDebug("disarm()\n");
  
  //enable motion sensor
  if (activate_motion_sensor)
  {
    th_motion_sensor.stop();
    th_motion_sensor.wait();
    th_motion_sensor.setDevice(motion_sensor_device.toLocal8Bit().data());
    th_motion_sensor.setSensibility(motion_sensor_sensibility);
  }
  
  //disable mouse sensor
  if (activate_mouse_sensor)
  {
    if (mouse_sensor) delete mouse_sensor;
  }
  
  //disable AC sensor
  if (activate_ac_sensor)
  {
    dataEngine("powermanagement")->disconnectSource("AC Adapter", this);
  }  
  //disable keyboard sensor
  if (activate_keyboard_sensor)
  {
    if (keyboard_sensor) delete keyboard_sensor;
  }
  
  armed = false;
  _pIcon->setIcon("disarmed");
  
  // Fire notification  
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


void setVolume()
{
  //open DBus interface
  QDBusInterface dBusIf("org.kde.kmix", "/Mixer0", "");
  if (!dBusIf.isValid())  {
    qDebug("cannot open dbus interface\n");
  } else {
    // call setMute method
    QDBusReply<void> dBusReply = dBusIf.call("setMute", "Master:0", false);
    if ( !dBusReply.isValid() )  {
      qDebug("error %s : %s\n", dBusReply.error().name().toAscii().data(), 
             dBusReply.error().message().toAscii().data() );
    }
    // call setVolume method
    dBusReply = dBusIf.call("setVolume", "Master:0", sound_level);
    if ( !dBusReply.isValid() )  {
      qDebug("error %s : %s\n", dBusReply.error().name().toAscii().data(), 
             dBusReply.error().message().toAscii().data() );
    }
  }
}


void xAlertU::init()
{
    // A small demonstration of the setFailedToLaunch function
    if (m_icon.isNull()) {
      setFailedToLaunch(true, i18n("xAlertU failed to launch..."));
    }
    
    readConfig();

    th_motion_sensor.setDevice(motion_sensor_device.toLocal8Bit().data());
    th_motion_sensor.setSensibility(motion_sensor_sensibility);
    
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
    sound_disarm = Phonon::createPlayer(Phonon::NotificationCategory, 
                                        Phonon::MediaSource("/usr/share/sounds/disarm.ogg"));
    sound_fire = Phonon::createPlayer(Phonon::NotificationCategory, 
                                      Phonon::MediaSource("/usr/share/sounds/caralarm.ogg"));
    //audioOutput = new Phonon::AudioOutput(Phonon::NotificationCategory, this);

    //Phonon::createPath(sound_arm, audioOutput); //sound_arm
    //Phonon::createPath(sound_disarm, audioOutput);
    //Phonon::createPath(sound_fire, audioOutput);
    
    //Phonon::createPath(sound_disarm, audioOutput);
    //Phonon::createPath(sound_fire, audioOutput);
    
    //Phonon::AudioOutput ao = new Phonon::AudioOutput();    
    connect(sound_fire, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));    
}

void xAlertU::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
{
  qDebug() << "dataUpdated()";
  if (source == "AC Adapter") {
    if (ac_cont-- == 1)
    {
      ac_plugged = data["Plugged in"].toBool();
    } else if (armed && (ac_plugged != data["Plugged in"].toBool()))
    {
      qDebug( "Ping!" ); 
      fireAlarm();
    } 
  }
}

void xAlertU::setImageWC() {
  
  qDebug("setImageWC()\n");
  
  uiWebCam.imagePrev->setPixmap(QPixmap(
  QDir::cleanPath( KShell::tildeExpand( 
  webcam_dir
  +"/alarm-fired-"
  +last_activation.left(last_activation.size()-1)
  +".png" ) )));
}


void xAlertU::setImageSC() 
{
  
  qDebug("setImageSC()\n");
  
  uiScreenshot.imagePrev->setPixmap(QPixmap(
  QDir::cleanPath( KShell::tildeExpand( 
  screenshot_dir
  +"/alarm-fired-screenshot-"
  +last_activation.left(last_activation.size()-1)
  +".jpg" ) )));
}


void xAlertU::aboutToFinish()
{
  qDebug("aboutToFinish()\n");
  if (alarm_loop || alarm_counter++ < alarm_duration)
  {
    setVolume();
    sound_fire->enqueue(Phonon::MediaSource("/usr/share/sounds/caralarm.ogg"));
  }
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
  
  QWidget *confScreenshotWidget = new QWidget;
  uiScreenshot.setupUi (confScreenshotWidget);
  
  QWidget *confEmailWidget = new QWidget;
  uiEmail.setupUi (confEmailWidget);
  
  QWidget *confRemoteWidget = new QWidget;
  uiRemote.setupUi (confRemoteWidget);
  
  /*QWidget *confAlarmWidget = new QWidget;
  ui.setupUi (confAlarmWidget);*/
  
  aParent->addPage(confGeneralWidget, i18n("Preferences"), icon());
  aParent->addPage(confMotionSensorWidget, i18n("Accelerometer"), "accelerometer");
  aParent->addPage(confSoundWidget, i18n("Sound"), "preferences-desktop-sound");
  aParent->addPage(confWebCamWidget, i18n("WebCam"), "camera-web");
  aParent->addPage(confScreenshotWidget, i18n("Screensot"), "ksnapshot");
  aParent->addPage(confEmailWidget, i18n("Email"), "email");

  //aParent->addPage(confRemoteWidget, i18n("Remote"), "infrared-remote");
  //aParent->addPage(confAlarmWidget, i18n("Alarm"), "preferences-desktop-notification-bell");
  
  connect(aParent, SIGNAL(accepted()), this, SLOT(configAccepted()));
  connect(aParent, SIGNAL(cancelClicked()), this, SLOT(configDiscarded()));
  connect(aParent, SIGNAL(closeClicked()), this, SLOT(configDiscarded()));
  
  ui.lockSesion->setChecked(lock_screen);
  ui.activate_motion_sensor->setChecked(activate_motion_sensor);
  ui.activate_ac_sensor->setChecked(activate_ac_sensor);
  ui.activate_mouse_sensor->setChecked(activate_mouse_sensor);
  
  uiSound.soundEnabled->setChecked(sound_enabled);
  uiSound.soundVolume->setValue(sound_level);
  uiSound.soundVolume->setDisabled(!sound_enabled);
  uiSound.alarmLoop->setChecked(alarm_loop);
  uiSound.alarmDuration->setValue(alarm_duration);
  uiSound.alarmDuration->setDisabled(alarm_loop);

  uiMotionSensor.motionsensorPath->setText(motion_sensor_device);
  uiMotionSensor.accelerometerSensivity->setValue(motion_sensor_sensibility);
  
  uiWebCam.webcamCommand->setText(webcam_command);
  uiWebCam.comboFormat->setEditText(webcam_format);
  uiWebCam.comboSize->setCurrentIndex(uiWebCam.comboSize->findText(webcam_size));
  uiWebCam.destinationDirectory->setText(webcam_dir);
  uiWebCam.inputDevice->setText(webcam_input);
  
  uiScreenshot.screenshotCommand->setText(screenshot_command);
  uiScreenshot.comboFormat->setEditText(screenshot_format);
  uiScreenshot.comboSize->setCurrentIndex(uiScreenshot.comboSize->findText(screenshot_size));
  uiScreenshot.destinationDirectory->setText(screenshot_dir);
  uiScreenshot.inputDevice->setEditText(screenshot_input);
  
  uiEmail.emailAlarms->setChecked(email_alarms);
  uiEmail.emailto->setText(mailto);
  uiEmail.attachimage->setChecked(attach_webcam_image);
  uiEmail.attachscreenshot->setChecked(attach_screenshot);
  
  uiEmail.configParams->setDisabled(!email_alarms);
  uiEmail.attachimage->setDisabled(!email_alarms);
  uiEmail.attachscreenshot->setDisabled(!email_alarms);
  uiEmail.sendTestEmail->setDisabled(!email_alarms);
  
  ui.last_activation->setText(last_activation);
  ui.last_armed->setText(last_armed);
  ui.last_sent->setText(last_send);
  ui.type->setText(type);
  ui.times_activated->setText(QVariant(times_activated).toString());
  
  connect(uiSound.soundEnabled, SIGNAL(stateChanged(int)), this, SLOT(soundEnabledConfigChanged(int)));
  connect(uiSound.alarmLoop, SIGNAL(stateChanged(int)), this, SLOT(alarmLoopConfigChanged(int)));
  connect(uiEmail.emailAlarms, SIGNAL(stateChanged(int)), this, SLOT(emailConfigChanged(int)));
  connect(uiScreenshot.test_command, SIGNAL(pressed()), this, SLOT(screenshotConfigTestButtonPressed()));
  connect(uiWebCam.test_command, SIGNAL(pressed()), this, SLOT(webCamConfigTestButtonPressed()));
  connect(uiWebCam.comboFormat, SIGNAL(editTextChanged(QString)), this, SLOT(webCamConfigChanged()));
  connect(uiWebCam.comboSize, SIGNAL(currentIndexChanged(int)), this, SLOT(webCamConfigChanged()));
  connect(uiWebCam.destinationDirectory, SIGNAL(textChanged(QString)), this, SLOT(webCamConfigChanged()));
  connect(uiWebCam.inputDevice, SIGNAL(textChanged(QString)), this, SLOT(webCamConfigChanged()));
  connect(uiScreenshot.comboFormat, SIGNAL(editTextChanged(QString)), this, SLOT(screenshotConfigChanged()));
  connect(uiScreenshot.comboSize, SIGNAL(currentIndexChanged(int)), this, SLOT(screenshotConfigChanged()));
  connect(uiScreenshot.destinationDirectory, SIGNAL(textChanged(QString)), this, SLOT(screenshotConfigChanged()));
  connect(uiScreenshot.inputDevice, SIGNAL(editTextChanged(QString)), this, SLOT(screenshotConfigChanged()));
  connect(&m_procWC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(setImageWC()));
  connect(&m_procSC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(setImageSC()));
  connect(uiEmail.sendTestEmail, SIGNAL(pressed()), this, SLOT(emailConfigTestButtonPressed()));
}


void xAlertU::configAccepted()
{
  qDebug("configAccepted()\n");
  
  lock_screen = (ui.lockSesion->checkState() == Qt::Checked);
  activate_motion_sensor = (ui.activate_motion_sensor->checkState() == Qt::Checked);
  activate_ac_sensor = (ui.activate_ac_sensor->checkState() == Qt::Checked);
  activate_mouse_sensor = (ui.activate_mouse_sensor->checkState() == Qt::Checked);
  
  sound_enabled = (uiSound.soundEnabled->checkState() == Qt::Checked);
  sound_level = uiSound.soundVolume->value();
  alarm_loop = (uiSound.alarmLoop->checkState() == Qt::Checked);
  alarm_duration = uiSound.alarmDuration->value();
    
  //audioOutput->setVolume(sound_level);
  
  motion_sensor_device = uiMotionSensor.motionsensorPath->text();
  motion_sensor_sensibility = uiMotionSensor.accelerometerSensivity->value();
  
  webcam_command = uiWebCam.webcamCommand->text();
  webcam_input = uiWebCam.inputDevice->text();
  webcam_size = uiWebCam.comboSize->currentText();
  webcam_format = uiWebCam.comboFormat->currentText();
  webcam_dir = uiWebCam.destinationDirectory->text();
  
  screenshot_command = uiScreenshot.screenshotCommand->text();
  screenshot_input = uiScreenshot.inputDevice->currentText();
  screenshot_size = uiScreenshot.comboSize->currentText();
  screenshot_format = uiScreenshot.comboFormat->currentText();
  screenshot_dir = uiScreenshot.destinationDirectory->text();  
  
  email_alarms = (uiEmail.emailAlarms->checkState() == Qt::Checked);
  mailto = uiEmail.emailto->text();
  attach_webcam_image = (uiEmail.attachimage->checkState() == Qt::Checked);
  attach_screenshot = (uiEmail.attachscreenshot->checkState() == Qt::Checked);
  
  writeConfig();
  
  th_motion_sensor.setDevice(motion_sensor_device.toLocal8Bit().data());
  th_motion_sensor.setSensibility(motion_sensor_sensibility);
  
  update();
}


void xAlertU::configDiscarded()
{
  readConfig();
}


void xAlertU::soundEnabledConfigChanged(int state)
{
  qDebug("soundEnabledConfigChanged()\n");
  uiSound.soundVolume->setDisabled(state != Qt::Checked);
}


void xAlertU::alarmLoopConfigChanged(int state)
{
  qDebug("alarmLoopConfigChanged()\n");
  uiSound.alarmDuration->setDisabled(state == Qt::Checked);
}


void xAlertU::emailConfigChanged(int state)
{
  qDebug("emailConfigChanged()\n");
  if ( state == Qt::Checked ) {
    uiEmail.configParams->setDisabled(false);
    uiEmail.attachimage->setDisabled(false);
    uiEmail.attachscreenshot->setDisabled(false);
    uiEmail.sendTestEmail->setDisabled(false);
  } else {
    uiEmail.configParams->setDisabled(true);
    uiEmail.attachimage->setDisabled(true);
    uiEmail.attachscreenshot->setDisabled(true);
    uiEmail.sendTestEmail->setDisabled(true);
    
  }
}


void xAlertU::emailConfigTestButtonPressed() 
{
  qDebug("emailConfigTestButtonPressed()\n");
  
  //take a screenshot when webcam command finish
  connect(&m_procWC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(runScreenshotCommand()));
  //runScreenshotCommand();
  
  //Send email when screenshot command finish
  connect(&m_procSC, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(runEmailCommand()));
  
  //Disconect the secuence when email command finish
  connect(&m_procMail, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(disconnectSecuence()));
  //runEmailCommand();
  
  //take a photo
  runWebCamCommand();
  
  
  mailto = uiEmail.emailto->text();
  attach_webcam_image = (uiEmail.attachimage->checkState() == Qt::Checked);
  attach_screenshot = (uiEmail.attachscreenshot->checkState() == Qt::Checked);

}

void xAlertU::webCamConfigTestButtonPressed()
{
  QString aux = webcam_dir;
  //QString aux2 = last_activation;
  time_t tstamp;
  struct tm *local;
  
  //qDebug() << QFileSystemModel("~").fileName();
  
  //stats
  tstamp = time(NULL);
  local = localtime(&tstamp);
  
  last_activation = asctime(local);
  
  qDebug("webCamConfigTestButtonPressed()\n");
  webcam_command = uiWebCam.webcamCommand->text();
  //checks if ffmpeg existe and alert the user if it doesn't
  KRun::runCommand( "ffmpeg", NULL );
  runWebCamCommand();
  //last_activation = aux2;
}

void xAlertU::screenshotConfigTestButtonPressed() {
  QString aux = screenshot_dir;
  //QString aux2 = last_activation;
  time_t tstamp;
  struct tm *local;
  
  //qDebug() << QFileSystemModel("~").fileName();
  
  //stats
  tstamp = time(NULL);
  local = localtime(&tstamp);
  
  last_activation = asctime(local);
  
  qDebug("screenshotConfigTestButtonPressed()\n");
  screenshot_command = uiScreenshot.screenshotCommand->text();
  //checks if ffmpeg existe and alert the user if it doesn't
  KRun::runCommand( "ffmpeg", NULL );
  runScreenshotCommand();
  //last_activation = aux2;
}


void xAlertU::webCamConfigChanged()
{
  qDebug("webCamConfigChanged()\n");
  uiWebCam.webcamCommand->setText("ffmpeg -f "
  +uiWebCam.comboFormat->currentText()
  +" -s "
  +uiWebCam.comboSize->currentText()
  +" -r 5 -vframes 1 -i "
  +uiWebCam.inputDevice->text()
  +" -y "
  +uiWebCam.destinationDirectory->text()
  +"/alarm-fired-%t.png");
}


void xAlertU::screenshotConfigChanged()
{
  qDebug("screenshotConfigChanged()\n");
  uiScreenshot.screenshotCommand->setText("ffmpeg -f "
  +uiScreenshot.comboFormat->currentText()
  +" -s "
  +uiScreenshot.comboSize->currentText()
  +" -r 5 -vframes 1 -i "
  +uiScreenshot.inputDevice->currentText()
  +" -y "
  +uiScreenshot.destinationDirectory->text()
  +"/alarm-fired-screenshot-%t.jpg");
}


void xAlertU::readConfig()
{
  qDebug("readConfig()\n");
  
  KConfigGroup cg = config();
   
  lock_screen = cg.readEntry(LOCK_SCREEN_CONFIG_STRING, true);
  activate_motion_sensor = cg.readEntry(ACTIVATE_MOTION_SENSOR_CONFIG_STRING, true);
  activate_mouse_sensor = cg.readEntry(ACTIVATE_MOUSE_SENSOR_CONFIG_STRING, false);
  activate_ac_sensor = cg.readEntry(ACTIVATE_AC_SENSOR_CONFIG_STRING, true);
  
  motion_sensor_device = cg.readEntry(MOTION_SENSOR_DEVICE_CONFIG_STRING, "/dev/input/js0" );
  motion_sensor_sensibility = cg.readEntry(MOTION_SENSOR_SENSIBILITY_CONFIG_STRING, 0 );
    
  sound_level = cg.readEntry(SOUND_LEVEL_CONFIG_STRING, 100);
  sound_enabled = cg.readEntry(SOUND_ENABLED_CONFIG_STRING, true);
  alarm_loop = cg.readEntry(ALARM_LOOP_CONFIG_STRING, true);
  alarm_duration = cg.readEntry(ALARM_DURATION_CONFIG_STRING, 12.0);
  
  last_armed = cg.readEntry(LAST_ARMED_CONFIG_STRING, "");
  last_send = cg.readEntry(LAST_SEND_CONFIG_STRING, "");
  last_activation = cg.readEntry(LAST_ACTIVATION_CONFIG_STRING, "");
  type = cg.readEntry(TYPE_CONFIG_STRING, "");
  times_activated = cg.readEntry(TIMES_ACTIVATED_CONFIG_STRING, 0);
  
  webcam_command = cg.readEntry(WEBCAM_COMMAND_CONFIG_STRING, 
                                "ffmpeg -f video4linux2 -s vga -r 5 -vframes 1 -i /dev/video0 -y /tmp/alarm-fired-%t.png");                              
  webcam_input = cg.readEntry(INPUT_CONFIG_STRING, "/dev/video0");
  webcam_size = cg.readEntry(SIZE_CONFIG_STRING, "vga");
  webcam_format = cg.readEntry(FORMAT_CONFIG_STRING, "video4linux2");
  webcam_dir = cg.readEntry(DIRECTORY_CONFIG_STRING, "/tmp");
  
  screenshot_command = cg.readEntry(SCREENSHOT_COMMAND_CONFIG_STRING, 
                                    "ffmpeg -f x11grab -s 1280x1024 -r 5 -vframes 1 -i :0.0 -y /tmp/alarm-fired-screenshot-%t.jpg");                              
  screenshot_input = cg.readEntry(SCINPUT_CONFIG_STRING, ":0.0");
  screenshot_size = cg.readEntry(SCSIZE_CONFIG_STRING, "1280x1024");
  screenshot_format = cg.readEntry(SCFORMAT_CONFIG_STRING, "x11grab");
  screenshot_dir = cg.readEntry(SCDIRECTORY_CONFIG_STRING, "/tmp");
  
  mailto = cg.readEntry(MAILTO_CONFIG_STRING, "");
  email_alarms = cg.readEntry(MAIL_ALARMS_CONFIG_STRING, false);
  attach_screenshot = cg.readEntry(ATTACH_SCREENSHOT_CONFIG_STRING, false);
  attach_webcam_image = cg.readEntry(ATTACH_WEBCAM_IMAGE_CONFIG_STRING, false);
    
  /*
  var = cg.readEntry("remote_control_staff", "");
  */
}


void xAlertU::writeConfig() {
  
  qDebug("writeConfig()\n");
  
  KConfigGroup cg = config();
  
  cg.writeEntry(LOCK_SCREEN_CONFIG_STRING, lock_screen);
  cg.writeEntry(ACTIVATE_MOTION_SENSOR_CONFIG_STRING, activate_motion_sensor);
  cg.writeEntry(ACTIVATE_MOUSE_SENSOR_CONFIG_STRING, activate_mouse_sensor );
  cg.writeEntry(ACTIVATE_AC_SENSOR_CONFIG_STRING, activate_ac_sensor);
  
  cg.writeEntry(MOTION_SENSOR_DEVICE_CONFIG_STRING, motion_sensor_device);
  cg.writeEntry(MOTION_SENSOR_SENSIBILITY_CONFIG_STRING, motion_sensor_sensibility );
  
  cg.writeEntry(SOUND_LEVEL_CONFIG_STRING, sound_level);
  cg.writeEntry(SOUND_ENABLED_CONFIG_STRING, sound_enabled);
  cg.writeEntry(ALARM_LOOP_CONFIG_STRING, alarm_loop);
  cg.writeEntry(ALARM_DURATION_CONFIG_STRING, alarm_duration);
  
  cg.writeEntry(LAST_ARMED_CONFIG_STRING, last_armed);
  cg.writeEntry(LAST_SEND_CONFIG_STRING, last_send);
  cg.writeEntry(LAST_ACTIVATION_CONFIG_STRING, last_activation);
  cg.writeEntry(TYPE_CONFIG_STRING, type);
  cg.writeEntry(TIMES_ACTIVATED_CONFIG_STRING, times_activated);

  cg.writeEntry(WEBCAM_COMMAND_CONFIG_STRING, webcam_command);
  cg.writeEntry(INPUT_CONFIG_STRING, webcam_input);
  cg.writeEntry(SIZE_CONFIG_STRING, webcam_size);
  cg.writeEntry(FORMAT_CONFIG_STRING, webcam_format);
  cg.writeEntry(DIRECTORY_CONFIG_STRING, webcam_dir);
  
  cg.writeEntry(SCREENSHOT_COMMAND_CONFIG_STRING, screenshot_command);
  cg.writeEntry(SCINPUT_CONFIG_STRING, screenshot_input);
  cg.writeEntry(SCSIZE_CONFIG_STRING, screenshot_size);
  cg.writeEntry(SCFORMAT_CONFIG_STRING, screenshot_format);
  cg.writeEntry(SCDIRECTORY_CONFIG_STRING, screenshot_dir);
    
  cg.writeEntry(MAILTO_CONFIG_STRING, mailto);
  cg.writeEntry(MAIL_ALARMS_CONFIG_STRING, email_alarms);
  cg.writeEntry(ATTACH_SCREENSHOT_CONFIG_STRING, attach_screenshot);
  cg.writeEntry(ATTACH_WEBCAM_IMAGE_CONFIG_STRING, attach_webcam_image);  

  //cg.writeEntry("remote_control_staff", "");
  emit configNeedsSaving();
}

#include "xalertu.moc"
