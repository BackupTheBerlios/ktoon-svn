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

#ifndef DSQUEEZELABEL_H
#define DSQUEEZELABEL_H

#include <qlabel.h>
#include <dcore/dglobal.h>

/**
 * @if english
 * Class inspired in KSqueezedTextLabel from kde libraries
 * 
 * from kde documentation:
 * 
 * A label class that squeezes its text into the label
 * If the text is too long to fit into the label it is divided into remaining left and right parts which are separated by three dots.
 * 
 * @elseif spanish
 * Esta clase esta inspirada en KSqueezedTextLabel de las librerias KDE
 * 
 * De la documentacion de kde:
 * 
 * Una clase label que corta su texto en el label
 * Si el texto es muy largo para ser llenado en el label es dividido en el espacio restante que son separadas por tres puntos.
 * 
 * @endif
 * @author David Cuadrado <krawek@gmail.com>
*/

class D_GUI_EXPORT DSqueezeLabel : public QLabel
{
	public:
		DSqueezeLabel(QWidget *parent, const char *name=0);
		DSqueezeLabel(const QString &text, QWidget *parent, const char *name=0);
		~DSqueezeLabel();
		
		QSize sizeHint() const;
		QSize minimumSizeHint() const;
		void setText( const QString &text );
		void setAlignment( Qt::Alignment alignment );
		QString completeText() const;
		
	protected:
		virtual void squeezeText();
		void resizeEvent(QResizeEvent *);
		
	private:
		QString squeezer(const QString &s, const QFontMetrics& fm, uint width);
		QString m_text;
};

#endif
