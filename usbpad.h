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

#ifndef USBPAD_H
#define USBPAD_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <KDE/Plasma/Applet>
#include <QtGui>

#include <linux/joystick.h>

#define MAX_AXIS 16
#define MAX_BUTTON 16

struct padData {
  unsigned char axisCount;
  unsigned char buttonCount;
  int fd;
  int version;
  char devName[80];
  int aPos[MAX_AXIS];
  int bPos[MAX_BUTTON];
  bool changed;
  js_event ev;
};

class usbpad : public QObject
{
    //Q_OBJECT
  public:
    // Constructor
    usbpad(char *device, int sensitibity);
    // Funciones miembro de la clase
    void updateData();
    void getData();
    int hasChanged();
    // Destructor
    ~usbpad();
  signals:
    void Changed();
  private:
    // Datos miembro de la clase
    padData pad;
    int result;
    int sens;
  public:
};

#endif
