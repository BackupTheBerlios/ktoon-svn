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

#include "ktmodulewidgettitle.h"

#include <QLabel>
#include <QFrame>
#include <QPainterPath>
#include <QPainter>
#include <QLinearGradient>

#include "ddebug.h"

KTModuleWidgetTitle::KTModuleWidgetTitle(const QString &title, QWidget *parent) : QFrame(parent)
{
	setFrameStyle( QFrame::Box | QFrame::Raised );
	setText(title);
	
#if QT_VERSION >= 0x040100
	setAutoFillBackground(true);
#endif

	setToolTip(tr("Double clickme for undock"));
	
	setFont(font());
}


KTModuleWidgetTitle::~KTModuleWidgetTitle()
{
}

QSize KTModuleWidgetTitle::sizeHint() const
{
	QFontMetrics fm(m_font);
	
	QString text = m_text;
	
	if ( m_text.isNull() )
	{
		text = "XXXXX";
	}
	
	return QSize(QFrame::sizeHint().width(), fm.size(Qt::TextSingleLine, text).height());
}

void KTModuleWidgetTitle::mouseDoubleClickEvent(QMouseEvent *)
{
	emit doubleClicked();
}

void KTModuleWidgetTitle::setText( const QString &text)
{
	m_text = text;
}

void KTModuleWidgetTitle::setFont(const QFont &font)
{
	m_font = font;
	
	QFontMetrics fm(font);
	setMinimumHeight( fm.size(Qt::TextSingleLine, "X").height() );
}

void KTModuleWidgetTitle::paintEvent(QPaintEvent *e)
{
	QFrame::paintEvent(e);
	
	QRect rect = contentsRect();
	
	QPainter painter(this);
	
	QPoint start = rect.topLeft();
	start.setY(rect.center().y());
	QLinearGradient gradient(start, rect.bottomLeft());
	gradient.setSpread(QGradient::ReflectSpread);
	gradient.setColorAt(0, palette().button().color());
	gradient.setColorAt(1, palette().background().color());
	
	painter.fillRect(rect, QBrush(gradient));
	
	painter.setFont(m_font);
	QFontMetrics fm(m_font);
	QSize tSize = fm.size(Qt::TextSingleLine, m_text);
	painter.drawText( (width()-tSize.width())/2, height()-5, m_text);
	
	
	painter.end();
	
}



