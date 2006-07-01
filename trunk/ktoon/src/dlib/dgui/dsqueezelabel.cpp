/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dsqueezelabel.h"

#include <qtooltip.h>
//Added by qt3to4:
#include <QLabel>
#include <QResizeEvent>

#include "ddebug.h"

DSqueezeLabel::DSqueezeLabel(QWidget *parent, const char *name) : QLabel(parent)
{
	setObjectName(name);
	setSizePolicy(QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ));
}

DSqueezeLabel::DSqueezeLabel( const QString &text , QWidget *parent, const char *name)
	: QLabel(parent)
{
	setObjectName(name);
	setSizePolicy(QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ));
	m_text = text;
	squeezeText();
}


DSqueezeLabel::~DSqueezeLabel()
{
}

void DSqueezeLabel::squeezeText()
{
	QFontMetrics fm(fontMetrics());
	int labelWidth = size().width();
	int textWidth = fm.width(m_text);
	
	if (textWidth > labelWidth)
	{
		QString squeezedText = squeezer(m_text, fm, labelWidth);

		QLabel::setText(squeezedText);
		setToolTip( m_text );
	} 
	else 
	{
		QLabel::setText(m_text);
	}
}

QString DSqueezeLabel::squeezer(const QString &s, const QFontMetrics& fm, uint width)
{
	if ( s.isEmpty() || uint( fm.width( s ) ) <= width )
	{
		return s;
	}
	
	const unsigned int length = s.length();
	if ( length == 2 ) 
	{
		return s;
	}
	const int maxWidth = width - fm.width( '.' ) * 3;
	if ( maxWidth <= 0 ) 
	{
		return "...";
	}
	
	unsigned int leftIdx = 0, rightIdx = length;
	unsigned int leftWidth = fm.charWidth( s, leftIdx++ );
	unsigned int rightWidth = fm.charWidth( s, --rightIdx );
	
	while ( leftWidth + rightWidth < uint( maxWidth ) )
	{
		while ( leftWidth <= rightWidth && leftWidth + rightWidth < uint( maxWidth ) ) 
		{
			leftWidth += fm.charWidth( s, leftIdx++ );
		}
		while ( rightWidth <= leftWidth && leftWidth + rightWidth < uint( maxWidth ) ) 
		{
			rightWidth += fm.charWidth( s, --rightIdx );
		}
	}
	
	if ( leftWidth > rightWidth ) 
	{
		--leftIdx;
	} else 
	{
		++rightIdx;
	}
	 
	rightIdx = length - rightIdx;
	if ( leftIdx == 0 && rightIdx == 1 || leftIdx == 1 && rightIdx == 0 ) 
	{
		return "...";
	}
	
	return s.left( leftIdx ) + "..." + s.right( rightIdx );
}

void DSqueezeLabel::resizeEvent( QResizeEvent * )
{
	squeezeText();
}

void DSqueezeLabel::setAlignment( Qt::Alignment alignment )
{
	QString tmp(m_text);
	QLabel::setAlignment(alignment);
	m_text = tmp;
}

QSize DSqueezeLabel::sizeHint() const
{
	return QSize(contentsRect().width(), QLabel::sizeHint().height());
}

QSize DSqueezeLabel::minimumSizeHint() const
{
	QSize sh = QLabel::minimumSizeHint();
	sh.setWidth(-1);
	return sh;
}

void DSqueezeLabel::setText( const QString &text ) 
{
	m_text = text;
	squeezeText();
}

QString DSqueezeLabel::completeText() const
{
	return m_text;
}


