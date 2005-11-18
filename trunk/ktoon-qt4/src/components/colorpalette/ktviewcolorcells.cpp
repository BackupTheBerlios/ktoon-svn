/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
#include "ktviewcolorcells.h"

KTViewColorCells::KTViewColorCells(QWidget *parent)
 : QFrame(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	setFrameStyle ( QFrame::Box | QFrame::Raised);
	setupForm();
}


KTViewColorCells::~KTViewColorCells()
{
}

void KTViewColorCells::setupForm()
{
	m_chooserPalette = new QComboBox(this);
	m_chooserPalette->addItem(tr("Default Palette"));
	m_containerPalette = new QStackedWidget(this);
	
	layout()->addWidget(m_chooserPalette);
	layout()->addWidget(m_containerPalette);
	
// 	m_defaultPalette = new  KTImagesTable(20,20, m_containerPalette);
// 	
// 	KTImagesTableItem *item = new KTImagesTableItem;
// 	
// 	QImage img(22,22,QImage::Format_RGB32);
// 	img.fill(qRgb(234,23,56));
// 	QPainter pimg(&img);
// 	
// 	pimg.drawLine(0,0,20,20);
// 	item->setImage(img);
// 	m_defaultPalette->setItem(0,0, item);
// 	m_defaultPalette->setItem(0,1, item);
// 	m_defaultPalette->setItem(0,2, item);
	
	m_defaultPalette = new KTColorCells(m_containerPalette);

	m_defaultPalette->fillColorsDefault();
	
	connect(m_defaultPalette, SIGNAL(changeColor( const QColor& )), this, SIGNAL(selectColor(const QColor &)));
	
	m_containerPalette->addWidget(m_defaultPalette);
	
}

void KTViewColorCells::addCustomColor(QColor c)
{
	
}

void KTViewColorCells::addCustomPalette(QString name)
{
	
}
