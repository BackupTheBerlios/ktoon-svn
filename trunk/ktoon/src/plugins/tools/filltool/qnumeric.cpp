/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "qnumeric.h"
#include "qnumeric_p.h"

Q_CORE_EXPORT bool qIsInf(double d) { return qt_is_inf(d); }
Q_CORE_EXPORT bool qIsNan(double d) { return qt_is_nan(d); }
Q_CORE_EXPORT bool qIsFinite(double d) { return qt_is_finite(d); }
Q_CORE_EXPORT bool qIsInf(float f) { return qt_is_inf(f); }
Q_CORE_EXPORT bool qIsNan(float f) { return qt_is_nan(f); }
Q_CORE_EXPORT bool qIsFinite(float f) { return qt_is_finite(f); }
Q_CORE_EXPORT double qSNan() { return qt_snan(); }
Q_CORE_EXPORT double qQNan() { return qt_qnan(); }
Q_CORE_EXPORT double qInf() { return qt_inf(); }

