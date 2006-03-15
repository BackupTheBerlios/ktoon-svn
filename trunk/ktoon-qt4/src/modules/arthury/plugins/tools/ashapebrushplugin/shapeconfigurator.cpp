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
 
#include "shapeconfigurator.h"

#include "dglobal.h"
#include "ddebug.h"

#include <QToolTip>
#include <QFrame>
#include <QPainterPath>
#include <QSizePolicy>

#include <cmath>

#include "ktbrushesparser.h"

ShapeConfigurator::ShapeConfigurator(QWidget *parent) : QWidget( parent ), m_currentShapeIndex(0)
{
	m_layout = new QGridLayout(this);
	
	setupDisplay();
	setupBrushManager();
	setupButtons();
	
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
}


ShapeConfigurator::~ShapeConfigurator()
{
	DEND;
	
	QDomDocument doc;
	QDomElement root = doc.createElement("Brushes");
	doc.appendChild(root);
	
	for ( int i = 0; i < m_customShapes.count(); i++)
	{
		root.appendChild(path2xml(m_customShapes[i], doc ));
	}
	
	QFile custom(CONFIG_DIR+"/brushes/customBrushes.ktbr");
	
	QDir brushesDir(CONFIG_DIR+"/brushes");
	
	if ( ! brushesDir.exists() )
	{
		brushesDir.mkdir(brushesDir.path() );
	}
	
	if ( custom.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&custom);
		
		out << doc.toString();
		
		custom.close();
	}
}


void ShapeConfigurator::setupDisplay()
{
	m_brushEditor = new KTBrushEditor();

	m_layout->addWidget(m_brushEditor, 0,0);
}

void ShapeConfigurator::setupBrushManager()
{
	QFrame *container = new QFrame;
	QBoxLayout *layoutContainer = new QBoxLayout(QBoxLayout::TopToBottom, container );
	
	m_editShapeButton = new QPushButton(tr("Edit shape"),container );
	m_editShapeButton->setCheckable(true);
	connect(m_editShapeButton, SIGNAL(clicked()), this, SLOT(editBrush()));
	
	createDefaultBrushes();
	setupCustomBrushes();
	
	layoutContainer->addWidget(m_editShapeButton);
	layoutContainer->addWidget(m_defaultBrushesList);
	m_layout->addWidget(container, 2,0);
}

void ShapeConfigurator::setupCustomBrushes()
{
	QDir customBrushesDir(CONFIG_DIR+"/brushes");
	
	if ( customBrushesDir.exists() )
	{
		// Parse!
		
		KTBrushesParser parser;
		
		QXmlSimpleReader reader;
		reader.setContentHandler(&parser);
		reader.setErrorHandler(&parser);
		
		QFile custom(CONFIG_DIR+"/brushes/customBrushes.ktbr");
		QXmlInputSource xmlsource(&custom);
		
		if ( reader.parse(&xmlsource) )
		{
			foreach(QPainterPath path, parser.brushes() )
			{
				m_defaultBrushesList->addBrush( path );
				m_customShapes << path;
			}
		}
		else
		{
			dError() << "Error while parse file: " << custom.fileName();
		}
	}
}

void ShapeConfigurator::setupButtons()
{
	QGroupBox *containerButtons = new QGroupBox();
	QHBoxLayout *layout = new QHBoxLayout(containerButtons);
	layout->setMargin(0);
	m_addBrush = new DImageButton( QPixmap( HOME+"/themes/default/icons/plussign.png" ), 22, containerButtons);
	connect( m_addBrush, SIGNAL( clicked() ), SLOT( addBrush() ) );
	m_addBrush->setToolTip(tr( "Add Brush" ) );
	layout->addWidget(m_addBrush, Qt::AlignCenter);
	
	m_removeBrush = new DImageButton( QPixmap( HOME+"/themes/default/icons/minussign.png" ), 22, containerButtons);
	connect( m_removeBrush, SIGNAL( clicked() ), SLOT( removeBrush() ) );
	m_removeBrush->setToolTip( tr( "Remove Brush" ) );
	layout->addWidget(m_removeBrush, Qt::AlignCenter);
	
	
	m_layout->addWidget(containerButtons, 1,0);
}

void ShapeConfigurator::selectBrush(DCellViewItem *item)
{
	if ( item )
	{
		KTBrushesList *brushesList = 0;
		
		if ( m_defaultBrushesList->isVisible() )
		{
			brushesList = m_defaultBrushesList;
		}
		else
		{
			brushesList = m_defaultBrushesList;
		}
		
		int currentRow = brushesList->row(item);
		int currentColumn = brushesList->column(item);

		if ( currentRow >= 0 && currentColumn >= 0 )
		{
			int pos = (brushesList->MAX_COLUMNS * (currentRow)) + currentColumn;
			if ( pos < brushesList->count() )
			{
				m_currentShapeIndex = pos;
				m_brushEditor->setPath( brushesList->path( m_currentShapeIndex  ));
				
				m_currentShape = brushesList->path( m_currentShapeIndex);
			}
		}
	}
}

void ShapeConfigurator::createDefaultBrushes()
{
	m_defaultBrushesList = new KTBrushesList;
	QPainterPath form;
	connect(m_defaultBrushesList, SIGNAL(itemClicked( DCellViewItem * )), this,SLOT(selectBrush( DCellViewItem * )));
	
	int thickness = 40; // FIXME
	QRect boundingRect = QRect( 0, 0, thickness , thickness);
	
	form.moveTo( boundingRect.center() );
	form.addEllipse(boundingRect);
	m_defaultBrushesList->addBrush( form);
	
	form = QPainterPath();
	
	form.addRect(boundingRect);
	m_defaultBrushesList->addBrush( form);
	boundingRect = QRect( 0, 0, thickness , thickness);
	form.moveTo(boundingRect.center());
	
	form = QPainterPath();
	form.moveTo(0,0);
	form.lineTo(thickness , thickness);
	m_defaultBrushesList->addBrush( form);

	form = QPainterPath();
	boundingRect = QRect( 0, 0, thickness , thickness);
	form.moveTo(boundingRect.center());
	form.arcTo(boundingRect,0,180);
	form.closeSubpath();
	
	m_defaultBrushesList->addBrush( form );
	
	form = QPainterPath();
	boundingRect = QRect( 0, 0, thickness , thickness);
	form.moveTo(0,0);
	form.addText(0, 0,QFont("Times", 70),"KTooN");
	
	m_defaultBrushesList->addBrush( form );
	
	form = QPainterPath();

	form.moveTo(0,0);
	form.cubicTo(thickness*2, 0, thickness+10, thickness+10, thickness*2, thickness*2);
	
	m_defaultBrushesList->addBrush(form);
	
	form = QPainterPath();
	form.moveTo(thickness/2, 0);
	for (int i = 1; i < 5; ++i)
	{
		form.lineTo(thickness/2 * cos(0.8 * i * 3.14159f), thickness/2 * sin(0.8 * i * 3.14159f));
	}
	form.closeSubpath();
	
	m_defaultBrushesList->addBrush( form);
}

void ShapeConfigurator::editBrush()
{
	m_brushEditor->setEdit( m_editShapeButton->isChecked() );
	
	m_currentShape = m_brushEditor->currentPainterPath();
}

void ShapeConfigurator::addBrush()
{
	QPainterPath shape = m_brushEditor->currentPainterPath();
	m_defaultBrushesList->addBrush(shape);
	
	m_customShapes << shape;
}

void ShapeConfigurator::removeBrush()
{
	dWarning() << "Not implemented yet!";
}


QPainterPath ShapeConfigurator::shape() const
{
	return m_currentShape;
}

QDomElement ShapeConfigurator::path2xml(const QPainterPath &path, QDomDocument &doc)
{
	QDomElement item = doc.createElement("Item");
	
	QList<QPolygonF> polygons = path.toSubpathPolygons ();
		
	QList<QPolygonF>::ConstIterator polygonIt = polygons.begin();
		
	while ( polygonIt != polygons.end() )
	{
		QDomElement polygonElement = doc.createElement("Polygon");
			
		QPolygonF::ConstIterator pointIt = (*polygonIt).begin();
			
		QString attribute = "";
		while (pointIt != (*polygonIt).end() )
		{
			attribute += QString("%1:%2 ").arg((*pointIt).x()).arg((*pointIt).y());
			++pointIt;
		}
		polygonElement.setAttribute("points", attribute.trimmed());
		item.appendChild(polygonElement);
			
		++polygonIt;
	}
	
	return item;
}
