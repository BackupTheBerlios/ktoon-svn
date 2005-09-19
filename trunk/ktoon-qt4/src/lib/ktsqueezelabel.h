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

#ifndef KTSQUEEZELABEL_H
#define KTSQUEEZELABEL_H

#include <qlabel.h>
//Added by qt3to4:
#include <QResizeEvent>

/**
 * Class based in KSqueezedTextLabel from kde libraries
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTSqueezeLabel : public QLabel
{
	public:
		KTSqueezeLabel(QWidget *parent, const char *name=0);
		KTSqueezeLabel(const QString &text, QWidget *parent, const char *name=0);
		~KTSqueezeLabel();
		
		QSize sizeHint() const;
		QSize minimumSizeHint() const;
		void setText( const QString &text );
		void setAlignment( int alignment );
		QString completeText() const;
		
	protected:
		virtual void squeezeText();
		void resizeEvent(QResizeEvent *);
		
	private:
		QString squeezer(const QString &s, const QFontMetrics& fm, uint width);
		QString m_text;
};

#endif
