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

#ifndef KTTIMELINELAYER_H
#define KTTIMELINELAYER_H

#include <qhbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcheckbox.h>

#include "ktelabel.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTTimeLineLayer;

class KTTimeLineLayer : public QHBox
{
	Q_OBJECT
	public:
		KTTimeLineLayer(const QString &name = "Layer", int position = 0, QWidget *parent = 0);
		~KTTimeLineLayer();
		void clearEditFocus();
		int position();
		void setPosition(int pos);
		
	public slots:
		void setSelected( bool selected );
		void setEdited( bool isEdited );
		
		void setOnlyOutlines( bool yes = true);
		void toggleOutlines();
		
		void setLock(bool yes = true);
		void toggleLock();
		
		void setView(bool yes = true);
		void toggleView();
		
		void rename();
		
	protected:
		void mousePressEvent( QMouseEvent *me );
		
	signals:
		void selected(int pos);
		void renamed(const QString &);
		void rightClicked(KTTimeLineLayer *ly, const QPoint &pos);
		
		
	private:
		QCheckBox *m_onlyOutlines;
		int m_position;
		KTELabel *m_layerName;
		QLabel *m_editionImage, *m_visibilityImage, *m_lockImage;
		
		QHBox *m_utils;
		
		bool m_isLocked, m_isVisible, m_onlySeeOutlines, m_isSelected, m_isEdited;
};

#endif
