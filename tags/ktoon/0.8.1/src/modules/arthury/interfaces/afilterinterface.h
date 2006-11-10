//
// C++ Interface: afilterinterface
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AFILTERINTERFACE_H
#define AFILTERINTERFACE_H

#include <QStringList>
#include <QRect>
#include <QPoint>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPainterPath>
#include <QHash>

#include <cmath> // sin,cos

#include "ktkeyframe.h"
#include <daction.h>

#include "qplugin.h" // Q_EXPORT_PLUGIN

class QKeySequence;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Q_DECL_EXPORT AFilterInterface
{
	public:
		virtual ~AFilterInterface() {}
		virtual QStringList keys() const = 0;
		virtual void filter(const QString &filter, const QList<AGraphicComponent *> &frame) = 0;
		
		virtual QHash<QString, DAction *>actions() = 0;
};

Q_DECLARE_INTERFACE(AFilterInterface, "com.toonka.ktoon.AFilterInterface/0.1");

#endif
