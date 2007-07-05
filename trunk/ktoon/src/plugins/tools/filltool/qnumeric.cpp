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

