/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef KTPENWIDGET_H
#define KTPENWIDGET_H

#include <QPen>

#include <ktmodulewidgetbase.h>

#include <kgui/keditspinbox.h>
#include <QComboBox>

class KTPaintAreaEvent;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTPenWidget : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTPenWidget(QWidget *parent = 0);
		~KTPenWidget();
		QPen pen() const;
		
	private:
		void emitPenChanged();
		
	private:
		struct Private;
		Private *const d;
		
	public slots:
		void reset();
		
	private slots:
		void setThickness(int value);
		void setStyle(int s);
		void setJoinStyle(int s);
		void setCapStyle(int s );
		
	signals:
		void penChanged(const QPen &pen);
		void paintAreaEventTriggered(const KTPaintAreaEvent *e);

};

#endif
