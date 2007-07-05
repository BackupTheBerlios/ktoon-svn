/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                    *
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


#ifndef KTCELLSCOLOR_H
#define KTCELLSCOLOR_H

#include <kgui/kcellview.h>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTCellsColor : public DCellView
{
Q_OBJECT
	public:
		
		enum Type{Color = 0, Gradient};
		KTCellsColor(QWidget *parent = 0, Type type= Color);
		virtual ~KTCellsColor();
		
		void setReadOnly(bool enable);
		void setType(Type type);
		bool isReadOnly();
		int type();
		QString name() const;
		void setName(const QString& name);
		void save( const QString& path);
		
	private:
		struct Private;
		Private *const d;
		
	protected:
		void mousePressEvent(QMouseEvent* e);
		void mouseMoveEvent(QMouseEvent* e);
		void dragEnterEvent(QDragEnterEvent* e);
		void dropEvent(QDropEvent* e);
};

#endif
