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

#ifndef TEXTCONFIGURATOR_H
#define TEXTCONFIGURATOR_H

#include <QWidget>
#include <QLineEdit>

#include <QTextEdit>

class KFontChooser;
class QCheckBox;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class TextConfigurator : public QWidget
{
    Q_OBJECT
    public:
        TextConfigurator(QWidget *parent = 0);
        ~TextConfigurator();
        QString text() const;

        QFont textFont() const;

        void setDocument(QTextDocument *doc);

        bool isHtml() const;

    private slots:
        void changeFont();

    private:
        QTextEdit *m_text;
        KFontChooser *m_fontChooser;
        QCheckBox *m_isHtml;
};

#endif