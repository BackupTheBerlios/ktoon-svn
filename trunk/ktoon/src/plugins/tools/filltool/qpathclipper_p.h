/****************************************************************************
**
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#ifndef QPATHCLIPPER_P_H
#define QPATHCLIPPER_P_H

#include <QtGui/qpainterpath.h>
#include <QtCore/qlist.h>

QT_BEGIN_HEADER

QT_MODULE(Gui)

class PathVertex;

class Q_GUI_EXPORT QPathClipper
{
public:
    enum Operation {
        BoolAnd,
        BoolOr,
        BoolSub,
        BoolInSub
    };
public:
    QPathClipper();
    QPathClipper(const QPainterPath &subject,
                 const QPainterPath &clip);
    ~QPathClipper();

    void setSubjectPath(const QPainterPath &path);
    QPainterPath subjectPath() const;

    void setClipPath(const QPainterPath &path);
    QPainterPath clipPath() const;

    QPainterPath clip(Operation op=BoolAnd);
    bool intersect();
    bool contains();
private:
    Q_DISABLE_COPY(QPathClipper)
    class Private;
    Private *d;
};

QT_END_HEADER

#endif // QPATHCLIPPER_P_H
