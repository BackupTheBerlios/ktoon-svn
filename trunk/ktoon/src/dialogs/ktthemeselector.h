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

#ifndef KTTHEMESELECTOR_H
#define KTTHEMESELECTOR_H

#include <qvbox.h>
#include <qcolordialog.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qscrollview.h>

#include <qmap.h>

#include "ktthemedocument.h"

class QListView;
class QListViewItem;
class QCheckBox;

/**
 * @author David Cuadrado
*/
class KTThemeSelector : public QVBox
{
	Q_OBJECT
	public:
		KTThemeSelector(QWidget *parent = 0, const char *name = 0);
		~KTThemeSelector();
		
		KTThemeDocument document();
		QString lastFile();
		
		bool iWantApplyColors();
		
		
	private slots:
		void chooseGeneralColor(int );
		void chooseEffectsColor(int );
		void chooseSelectionsColor(int );
		void chooseTextEffectsColor(int );
		void saveSchema();
		void loadSchemaFromListView(QListViewItem *, const QPoint &, int );
		
	private:
		void setupChooseColor();
		void loadSchemes();
		
	private:
		QButtonGroup *m_general;
		ThemeKey m_generalSection;
		
		QButtonGroup *m_effects;
		ThemeKey m_effectsSection;
		
		QButtonGroup *m_selections;
		ThemeKey m_selectionsSection;
		
		QButtonGroup *m_textEffects;
		ThemeKey m_textEffectsSection;
		
		QListView *m_allSchemes;
		QCheckBox *m_useColors;
		
		QString m_lastFile;
};

#endif
