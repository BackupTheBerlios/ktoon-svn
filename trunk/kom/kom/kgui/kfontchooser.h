/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
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


#ifndef KFONTCHOOSER_H
#define KFONTCHOOSER_H

#include <QFrame>
#include <kcore/kglobal.h>

class QComboBox;
class QFontComboBox;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class K_GUI_EXPORT KFontChooser : public QFrame
{
    Q_OBJECT
    public:
        KFontChooser(QWidget *parent = 0);
        ~KFontChooser();
        void setCurrentFont(const QFont &font);
        void initFont();
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
