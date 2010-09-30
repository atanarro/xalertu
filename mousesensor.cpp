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

#include "mousesensor.h"

MouseSensor::MouseSensor()
{
  timerId = startTimer(50);
  cont = 1;
}


MouseSensor::~MouseSensor()
{
  killTimer(timerId);
}


void MouseSensor::timerEvent(QTimerEvent *e)
{
  if (e->timerId() != timerId) {
    return;
  }
  
  QPoint absMousePos = QCursor::pos();
  
  if (absMousePos == previousMousePos) {
    if (timerInterval > 300)
      return;
    timerInterval += 50;
    killTimer(timerId);
    timerId = startTimer(timerInterval);
    return;
  }
  
  if (timerInterval != 50) {
    timerInterval = 50;
    killTimer(timerId);
    timerId = startTimer(timerInterval);
  }
  if (cont--==0)
  {
    cont=-1;
    qDebug()<< "fire?"<< absMousePos-previousMousePos;
    
    // Fire notification
    emit fire();
  }
  
  previousMousePos = absMousePos;
}
