/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
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

#ifndef KTPREFERENCES_H
#define KTPREFERENCES_H

/**
 * @file ktpreferences.h
 * @brief Include this file if you need the class KTPreferences
 */

#include <qlineedit.h>
#include <qlabel.h>
#include <qradiobutton.h>

#include <kgui/kfontchooser.h>
#include <kgui/ktabdialog.h>
#include <kgui/kconfigurationdialog.h>

#include "ktthemeselector.h"
#include "ktglobal.h"

class ColorSchemePref;
class KTPaintAreaConfig;

class KTPreferences : public KConfigurationDialog
{
    Q_OBJECT;
    
    public:
        KTPreferences(QWidget *parent);
        ~KTPreferences();
        
    private:
        class GeneralPage;
        class FontPage;
        
        KTThemeSelector *m_themeSelector;
        FontPage *m_fontChooser;
        GeneralPage *m_generalPage;
        KTPaintAreaConfig *m_drawingAreaProperties;
        
    public slots:
        void apply();
        void ok();

    signals:
        void timerChanged();
};

#endif

