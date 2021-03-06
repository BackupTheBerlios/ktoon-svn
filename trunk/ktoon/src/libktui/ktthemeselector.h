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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef KTTHEMESELECTOR_H
#define KTTHEMESELECTOR_H

#include <QMap>
#include <QTreeWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QColorDialog>
#include <QLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QGridLayout>
#include <QButtonGroup>

#include <kgui/kthemedocument.h>
#include <kgui/kvhbox.h>

class QCheckBox;

/**
 * @author David Cuadrado
*/
class KTThemeSelector : public KVHBox
{
    Q_OBJECT
    public:
        KTThemeSelector(QWidget *parent = 0);
        ~KTThemeSelector();
        
        KThemeDocument document() const;
        QString lastFile() const;
        bool applyColors() const;
        void setPalette(const QPalette &) {};
        
    private slots:
        void chooseGeneralColor(QAbstractButton *);
        void chooseEffectsColor(QAbstractButton *);
        void chooseSelectionsColor(QAbstractButton *);
        void chooseTextEffectsColor(QAbstractButton *);
        void saveSchema();
        void loadSchemaFromListView(QTreeWidgetItem *, int column);
        
    private:
        void setupChooseColor();
        void loadSchemes();
        
    private:
        QGroupBox *m_general;
        ThemeKey m_generalSection;
        QButtonGroup m_generalButtonGroup;
        
        QGroupBox *m_effects;
        ThemeKey m_effectsSection;
        QButtonGroup m_effectsButtonGroup;
        
        QGroupBox *m_selections;
        ThemeKey m_selectionsSection;
        QButtonGroup m_selectionsButtonGroup;
        
        QGroupBox *m_textEffects;
        ThemeKey m_textEffectsSection;
        QButtonGroup m_textEffectsButtonGroup;
        
        QTreeWidget *m_allSchemes;
        QCheckBox *m_useColors;
        
        QString m_lastFile;
};

#endif
