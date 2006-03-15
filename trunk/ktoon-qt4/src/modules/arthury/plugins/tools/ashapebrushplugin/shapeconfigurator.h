/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#ifndef SHAPECONFIGURATOR_H
#define SHAPECONFIGURATOR_H

#include "ktdisplaybrush.h"
#include "ktbrusheditor.h"

#include "deditspinbox.h"
#include "dimagebutton.h"
#include "ktbrusheslist.h"

#include <QBoxLayout>
#include <QLineEdit>

#include "dtabwidget.h"
#include "dtoolbox.h"

#include <QGridLayout>

#include "ktbrush.h"

/**
* @author David Cuadrado <krawek@toonka.com>
*/
class ShapeConfigurator : public QWidget
{
	Q_OBJECT
	public:
		ShapeConfigurator(QWidget *parent = 0);
		~ShapeConfigurator();
		QPainterPath shape() const;
		
	private:
		void setupDisplay();
		void setupBrushManager();
		void setupButtons();
		void createDefaultBrushes();
		void setupCustomBrushes();
		
		QDomElement path2xml(const QPainterPath &path,QDomDocument &doc);
		
	private slots:
		void editBrush();
		void addBrush();
		void removeBrush();
		
	public slots:
		void selectBrush(DCellViewItem * item);
		
	private:
		KTBrushEditor *m_brushEditor;
		
		DImageButton *m_addBrush, *m_removeBrush;
		KTBrushesList *m_defaultBrushesList;
		
		QList<QPainterPath> m_customShapes;
		
	private:
		QPushButton *m_editShapeButton;
		
		QGridLayout *m_layout;
		int m_currentShapeIndex;
		
		QPainterPath m_currentShape;
};

#endif
