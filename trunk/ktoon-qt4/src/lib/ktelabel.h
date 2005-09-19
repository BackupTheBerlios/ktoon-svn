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
#ifndef KTELABEL_H
#define KTELABEL_H

#include <ktsqueezelabel.h>
#include <qlineedit.h>
//Added by qt3to4:
#include <QMouseEvent>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTELabel : public KTSqueezeLabel
{
	Q_OBJECT

	public:
		KTELabel(const QString &text, QWidget *parent);
		~KTELabel();
		
	public slots:
		void rename();
		void clearFocus();
		
	private slots:
		void applyText();
		
	signals:
		void renamed(const QString &);
		
	protected:
		virtual void mouseDoubleClickEvent( QMouseEvent *mouse_event );
		
	private:
		QLineEdit *m_editor;
};

#endif
