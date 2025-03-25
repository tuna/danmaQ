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

#ifndef COMMON_HPP
#define COMMON_HPP

#include "DMCanvas.hpp"
#include "DMMainWindow.hpp"
#include "Danmaku.hpp"
#include "Subscriber.hpp"

#define DANMAQ_VERSION "0.3.0"

#if defined _WIN32 || defined __CYGWIN__
#define myDebug (qDebug() << "[" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "]")
#else
#define myDebug (qDebug() << "\x1b[34;1m" << __FILE__ << ":" << __LINE__ << ":" << __PRETTY_FUNCTION__ << "\x1b[0m")
#endif

#endif
