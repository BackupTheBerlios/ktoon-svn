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
#include <QImage>
#include <QAction>
#include <QHash>

#include "qplugin.h" // Q_EXPORT_PLUGIN

class QKeySequence;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class AFilterInterface
{
	public:
		virtual ~AFilterInterface() {}
		virtual QStringList keys() const = 0;
		virtual QImage filter(const QString &filter, const QImage &image, QWidget *parent) = 0;
		
		virtual QHash<QString, QAction *>actions() = 0;
};

Q_DECLARE_INTERFACE(AFilterInterface, "com.toonka.ktoon.AFilterInterface/0.1");

#endif
