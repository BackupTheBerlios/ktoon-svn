/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
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

#include "splash.h"
#include "images.h"
#include <qpixmap.h>
#include <qbitmap.h>
#include <qimage.h>
#include <QTimer>

#include "ktapplication.h"
#include "kimageeffect.h"

//------------------ CONSTRUCTOR -----------------

Splash::Splash() : QSplashScreen( 0, QPixmap(), Qt::WStyle_StaysOnTop ), m_size(3), m_state(0)
{
	QTimer *timer = new QTimer( this );
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
	timer->start(150);
	
	QPixmap pixmap(KTOON_HOME+"/images/splash.png");
	
	setPixmap(pixmap);
	m_version = tr("Version ")+ktapp->version();
}

Splash::~Splash()
{
}

void Splash::animate()
{
	m_state = ((m_state + 1) % (2*m_size-1));
	repaint();
}

void Splash::setMessage(const QString &msg)
{
	QSplashScreen::showMessage(msg,Qt::AlignTop, palette().text() );
	m_message = msg;
	animate();
}

void Splash::drawContents ( QPainter * painter )
{
	int position;

	// Draw background circles
	painter->setPen(Qt::NoPen);
	painter->setBrush( palette().background() );
	painter->drawEllipse(51,7,9,9);
	painter->drawEllipse(62,7,9,9);
	painter->drawEllipse(73,7,9,9);

	QColor baseColor = palette().base();
	for (int i = 0; i < m_size; i++)
	{
		position = (m_state+i)%(2*m_size-1);

		painter->setBrush(QColor(baseColor.red()-18*i,
				  baseColor.green()-10*i,
				  baseColor.blue()-28*i));

		if (position < 3) 
		{
			painter->drawEllipse(51+position*11,7,9,9);
		}
	}

	painter->setPen( 0x706012);

	// Draw version number
	QRect r = rect();
	r.setRect(r.x() + 5, r.y() + 5, r.width() - 10, r.height() - 10);
	painter->drawText(r, Qt::AlignRight, m_version);

	// Draw message at given position, limited to 43 chars
	// If message is too long, string is truncated
	if (m_message.length() > 40) 
	{
		m_message.truncate(39); m_message += "...";
	}
	painter->drawText (90, 16, m_message, 42);
}

/*
void Splash::drawContents ( QPainter * painter )
{
	painter->setRenderHint(QPainter::TextAntialiasing, true);
	painter->setPen(palette().base());
	painter->setFont(QFont("Times", 12, QFont::Bold));
	QSize sizeText = painter->fontMetrics ().size(Qt::TextSingleLine , m_version);
	painter->drawText( width()-sizeText.width()-10, height()-sizeText.height()-10, sizeText.width(), sizeText.height(), Qt::AlignHCenter, m_version );
	
	QSplashScreen::drawContents(painter);
}*/


