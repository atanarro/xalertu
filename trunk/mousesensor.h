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

#ifndef MOUSESENSOR_H
#define MOUSESENSOR_H

#include <KDE/Plasma/Applet>

#include <QtGui>
#include <QObject>

class MouseSensor : public QObject
{
  Q_OBJECT
  public:
    // Constructor
    MouseSensor();
    // Destructor
    ~MouseSensor();
    
  private:
    int timerInterval;
    int timerId;
    int cont;
    QPoint previousMousePos;

    void timerEvent(QTimerEvent *e);
    
  signals:
    void fire( void );
};

#endif // MOUSESENSOR_H
