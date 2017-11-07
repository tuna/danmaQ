/*
 * This file is part of danmaQ.
 * 
 * DanmaQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DanmaQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DANMAKU_H__
#define __DANMAKU_H__

#include "danmaku_ui.h"
#include "danmaku_window.h"
#include "subscriber.h"
#include "danmaQ_app.h"

#if defined _WIN32 || defined __CYGWIN__
#define myDebug (qDebug() << __LINE__)
#else
#define myDebug (qDebug() << "\x1b[34;1m" <<__PRETTY_FUNCTION__ << ":" << __LINE__ << "\x1b[0m")
#endif

#endif 
