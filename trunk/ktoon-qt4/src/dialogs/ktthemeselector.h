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

#include <q3vbox.h>
#include <qcolordialog.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <q3buttongroup.h>
#include <q3scrollview.h>

#include <qmap.h>

#include "ktthemedocument.h"

class Q3ListView;
class Q3ListViewItem;
class QCheckBox;

/**
 * @author David Cuadrado
*/
class KTThemeSelector : public Q3VBox
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
		void loadSchemaFromListView(Q3ListViewItem *, const QPoint &, int );
		
	private:
		void setupChooseColor();
		void loadSchemes();
		
	private:
		Q3ButtonGroup *m_general;
		ThemeKey m_generalSection;
		
		Q3ButtonGroup *m_effects;
		ThemeKey m_effectsSection;
		
		Q3ButtonGroup *m_selections;
		ThemeKey m_selectionsSection;
		
		Q3ButtonGroup *m_textEffects;
		ThemeKey m_textEffectsSection;
		
		Q3ListView *m_allSchemes;
		QCheckBox *m_useColors;
		
		QString m_lastFile;
};

#endif
