/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef DFONTCHOOSER_H
#define DFONTCHOOSER_H

#include <QFrame>
#include <dcore/dglobal.h>

class QComboBox;
class QFontComboBox;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class D_GUI_EXPORT DFontChooser : public QFrame
{
	Q_OBJECT
	public:
		DFontChooser(QWidget *parent = 0);
		~DFontChooser();
		void setCurrentFont(const QFont &font);
		QFont currentFont() const;
		
	signals:
		void fontChanged();
		
	private slots:
		void emitFontChanged(int = 0);
		void loadFontInfo(const QFont &newFont);
		
	private:
		QFontComboBox *m_families;
		QComboBox *m_fontStyle;
		QComboBox *m_fontSize;
		
		QFont m_currentFont;
};

#endif
