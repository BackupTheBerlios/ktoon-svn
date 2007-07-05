/****************************************************************************
**
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QNUMERIC_H
#define QNUMERIC_H

#include <QtCore/qglobal.h>

QT_BEGIN_HEADER

QT_MODULE(Core)

Q_CORE_EXPORT bool qIsInf(double d);
Q_CORE_EXPORT bool qIsNan(double d);
Q_CORE_EXPORT bool qIsFinite(double d);
Q_CORE_EXPORT bool qIsInf(float f);
Q_CORE_EXPORT bool qIsNan(float f);
Q_CORE_EXPORT bool qIsFinite(float f);
Q_CORE_EXPORT double qSNan();
Q_CORE_EXPORT double qQNan();
Q_CORE_EXPORT double qInf();

#define Q_INFINITY (::qInf())
#define Q_SNAN (::qSNan())
#define Q_QNAN (::qQNan())

QT_END_HEADER

#endif // QNUMERIC_H
