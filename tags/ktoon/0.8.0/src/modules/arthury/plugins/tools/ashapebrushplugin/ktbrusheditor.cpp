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

#include "ktbrusheditor.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QVector>

#include "ddebug.h"


class KTBrushEditor::Editor
{
	public:
		Editor() : editing(false), editedPointIndex(-1)
		{
		}
		~Editor()
		{
		}
		
		QPainterPath createPath()
		{
			QPainterPath brushForm;
			
			QVector<QPoint>::const_iterator it;
			
			for (it = nodes.begin(); it != nodes.end(); ++it)
			{
				if ( it == nodes.begin() )
				{
					brushForm.moveTo(*it);
				}
				else
				{
					brushForm.lineTo(*it);
				}
				
				if ( it == nodes.end()-1)
				{
					brushForm.lineTo( *nodes.begin() );
				}
			}
			
			return brushForm;
		}
		
		bool editing;
		QVector<QPoint> nodes;
		int editedPointIndex;
};

KTBrushEditor::KTBrushEditor(QWidget *parent) : DDisplayPath(parent), m_editor(0)
{
	setMouseTracking(true);
	
	m_editor = new Editor();
}

KTBrushEditor::~KTBrushEditor()
{
	if ( m_editor ) delete m_editor;
}

void KTBrushEditor::paintEvent(QPaintEvent *e)
{
	QFrame::paintEvent(e);
	if ( m_editor->editing )
	{
		QImage *editArea = displayDevice();
		QPainter painter( editArea ) ;
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen(Qt::black);
		
		editArea->fill(qRgb(255, 255, 255));
		
		painter.save();
		drawEditor(&painter);
		painter.restore();
		
		painter.end();
	
		painter.begin(this);
		painter.translate(QPoint(width()/2-50, (height() - editArea->height())/2 ));
	
		painter.drawImage(QPoint(0, 0), *editArea);
	
		painter.drawRect(editArea->rect());
	}
	else
	{
		DDisplayPath::paintEvent(e);
	}
}

void KTBrushEditor::drawEditor(QPainter *p)
{
	p->drawPath(m_editor->createPath());
	
	p->setPen( QPen(QBrush(Qt::blue), 5, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin) );
	
	for (int pos = 0; pos < m_editor->nodes.count(); pos++)
	{
		QPoint point = m_editor->nodes[pos];
		
		if ( m_editor->editedPointIndex != pos )
		{
			p->drawPoint( point );
		}
		else
		{
			p->save();
			
			p->setPen( QPen(QBrush(Qt::red), 5, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin) );
			p->drawPoint(point);
			
			p->restore();
		}
	}
}

void KTBrushEditor::mousePressEvent(QMouseEvent *e)
{
	if ( m_editor->editing )
	{
		int pos = findNodeIndex(mapToEditor( e->pos() ), 5 );
		
		if ( (e->buttons() & Qt::LeftButton) )
		{
			if ( pos >= 0 )
			{
				m_editor->editedPointIndex = pos;
			}
			else
			{
				m_editor->nodes << mapToEditor(e->pos());
			}
		}
		else if ( e->buttons() & Qt::RightButton )
		{
			if ( pos >= 0)
			{
				m_editor->nodes.remove(pos);
			}
		}
		
		repaint();
	}
}

void KTBrushEditor::mouseMoveEvent(QMouseEvent *e)
{
	if ( (e->buttons() & Qt::LeftButton) )
	{
		if ( m_editor->editedPointIndex >= 0 )
		{
			m_editor->nodes[m_editor->editedPointIndex] = mapToEditor( e->pos());
			repaint();
		}
	}
}

void KTBrushEditor::mouseReleaseEvent(QMouseEvent *e)
{
// 	if ( e->buttons() & Qt::LeftButton )
// 	{
		if ( m_editor->editedPointIndex >= 0 )
		{
			m_editor->nodes[m_editor->editedPointIndex] = mapToEditor( e->pos());
			m_editor->editedPointIndex = -1;
		}
// 	}
	
	repaint();
}

int KTBrushEditor::findNodeIndex(const QPoint &p, int rate)
{
	QRect rect(p - QPoint(rate/2,rate/2), p + QPoint(rate/2,rate/2) );
	
	QVector<QPoint>::const_iterator it;
	
	int pos = -1;
	
	for (it = m_editor->nodes.begin(); it != m_editor->nodes.end(); ++it)
	{
		if( rect.contains(*it) )
		{
			pos = m_editor->nodes.indexOf(*it);
			break;
		}
	}
	
	return pos;
}

void KTBrushEditor::setEdit(bool e)
{
	m_editor->editing = e;
	
	repaint();
}

QPoint KTBrushEditor::mapToEditor(const QPoint &p)
{
	return p - QPoint( width()/2 - displayDevice()->width()/2, height()/2 - displayDevice()->height()/2);
}

QPainterPath KTBrushEditor::currentPainterPath()
{
	if ( m_editor->editing )
	{
		QPainterPath path = m_editor->createPath();
		QPointF position = path.currentPosition();
	
		QMatrix matrix;
		matrix.translate(-position.x(),-position.y());	
		return matrix.map(path);
	}
	return DDisplayPath::currentPainterPath();
}

