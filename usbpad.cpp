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

#include "usbpad.h"

usbpad::usbpad(char *device, int sensitibity) {
  sens = sensitibity;
  pad.fd = open(device, O_RDONLY);
  if (pad.fd > 0)
  {
    qDebug ("device opened OK: %s\n", device);
    // Get pad info
    ioctl(pad.fd, JSIOCGAXES, &pad.axisCount);
    ioctl(pad.fd, JSIOCGBUTTONS, &pad.buttonCount);
    ioctl(pad.fd, JSIOCGVERSION, &pad.version);
    ioctl(pad.fd, JSIOCGNAME(80), &pad.devName);
    fcntl(pad.fd, F_SETFL, O_NONBLOCK);
    
    // set default values
    for (int i=0;i<pad.axisCount;i++) pad.aPos[i]=0;
    for (int i=0;i<pad.buttonCount;i++) pad.bPos[i]=0;
    
    qDebug ("axis : %d\n", pad.axisCount);
    qDebug ("buttons : %d\n", pad.buttonCount);
    qDebug ("version : %d\n", pad.version);
    qDebug ("name : %s\n", pad.devName);
    
  } else
  {
    qDebug ("failed to open device: %s\n", device);
  }
  pad.changed = false;
}

void usbpad::updateData() {
  //qDebug("updateData()\n");
  if (pad.fd > 0)
  {
    
    result = read(pad.fd, &pad.ev, sizeof(pad.ev));
    if (result > 0)
    {
      switch (pad.ev.type)
      {
        case JS_EVENT_INIT:
        case JS_EVENT_INIT | JS_EVENT_AXIS:
        case JS_EVENT_INIT | JS_EVENT_BUTTON:
          break;
        case JS_EVENT_AXIS:
          //qDebug() << "==> i-1=: " << pad.aPos[pad.ev.number] << " i: " << pad.ev.value << " sens: " << sens;
          if ( (pad.ev.value > (pad.aPos[pad.ev.number]+sens))
            || (pad.ev.value < (pad.aPos[pad.ev.number]-sens)) ) {
            pad.aPos[pad.ev.number] = pad.ev.value;
          pad.changed = true;
          }
          break;
        case JS_EVENT_BUTTON:
          pad.bPos[pad.ev.number] = pad.ev.value;
          pad.changed = true;
          break;
        default:
          //qDebug ("Other event ? %d\nnumber=%d\nvalue=%d\n",
          //         pad.ev.type,pad.ev.number, pad.ev.value);
          break;
      }
    }
  }
}

void usbpad::getData() {
  qDebug("getData()\n");
  
  if (pad.changed)
  {
    
    qDebug ("------------------[Pad Info]------------------\n");
    qDebug ("axis : %d\n", pad.axisCount);
    qDebug ("buttons : %d\n", pad.buttonCount);
    qDebug ("version : %d\n", pad.version);
    qDebug ("name : %s\n", pad.devName);
    qDebug ("----------------------------------------------\n");
    qDebug ("last ev time : %d\n", pad.ev.time);
    
    for (int i=0;i<pad.axisCount;i++) qDebug (" Axis %2d |",i);
    qDebug ("\n");
    for (int i=0;i<pad.axisCount;i++) qDebug (" %7d |",pad.aPos[i]);
    qDebug ("\n\n");
    for (int i=0;i<pad.buttonCount;i++) qDebug (" Btn.%2d |",i);
    qDebug ("\n");
    for (int i=0;i<pad.buttonCount;i++) qDebug (" %2d |",pad.bPos[i]);
    qDebug ("\n");
    pad.changed = false;
  }
}
int usbpad::hasChanged(){
  if (pad.changed)
    return 1;
  else return 0;
}

usbpad::~usbpad() {
  close(pad.fd);
}
