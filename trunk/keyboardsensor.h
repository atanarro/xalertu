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

#ifndef KEYBOARDSENSOR_H
#define KEYBOARDSENSOR_H

#include <KDE/Plasma/Applet>

#include <QtGui>

class KeyboardSensor : public QObject
{
  public:
    // Constructor
    KeyboardSensor();
    // Destructor
    ~KeyboardSensor();
    
  signals:
    void Changed();
  private:
    int timerInterval;
    int timerId;
    int previousKey;
    
    void keyPressEvent(QKeyEvent * event);
    void timerEvent(QTimerEvent *e);
};

#endif // MOUSESENSOR_H
