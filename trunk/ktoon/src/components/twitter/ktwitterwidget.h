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

#ifndef KTWITTERWIDGET_H
#define KTWITTERWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QTextBrowser>
#include <QTextDocument>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTwitterWidget : public QWidget
{
    Q_OBJECT

    public:
        KTwitterWidget(QWidget *parent);
        ~KTwitterWidget();

    public slots:
        void setDocument(const QString &doc);
        void setSource(const QString &filePath);

    private slots: 
        void reload();

    protected:
        void keyPressEvent(QKeyEvent * event); 

    private:
        QSplitter *m_separator;
        QTextBrowser *m_pageArea;
        QTextDocument *m_document;
};

#endif