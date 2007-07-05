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

#ifndef KTPAINTAREASTATUS_H
#define KTPAINTAREASTATUS_H

#include <qstatusbar.h>

class KTViewDocument;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTPaintAreaStatus : public QStatusBar
{
	Q_OBJECT;
	public:
		KTPaintAreaStatus(KTViewDocument *parent);
		~KTPaintAreaStatus();
		
	public slots:
		void setBrush(const QBrush &brush);
		void setPen(const QPen &pen);
		
	private slots:
		void selectAntialiasingHint(bool use);
		void selectRenderer(int id);
		void applyRotationFromItem(const QString & text);
		
	private:
		struct Private;
		Private *const d;
};

#endif
