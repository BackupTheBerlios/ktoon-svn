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

#include "kthelpbrowser.h"
#include <kcore/kglobal.h>
#include <kcore/kdebug.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QMouseEvent>

// Help Browser

KTHelpBrowser::KTHelpBrowser(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(tr("Help"));
    setWindowIcon(QIcon(QPixmap(THEME_DIR + "icons/help_mode.png")));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(15);
    m_separator = new QSplitter(this);
    layout->addWidget(m_separator);

    m_pageArea = new QTextBrowser(m_separator);
    m_pageArea->setOpenExternalLinks(true);

    m_document = new QTextDocument(m_pageArea);

    m_pageArea->setDocument(m_document);
}

KTHelpBrowser::~KTHelpBrowser()
{
}

void KTHelpBrowser::setDocument(const QString &doc)
{
    m_document->setHtml(doc);
}

void KTHelpBrowser::setSource(const QString &filePath)
{
    m_pageArea->setSource(filePath);
}

void KTHelpBrowser::setDataDirs(const QStringList &dirs)
{
    m_pageArea->setSearchPaths(dirs);
}

// SQA: These methods are just temporary for developing reasons

void KTHelpBrowser::keyPressEvent(QKeyEvent * event) {
    switch (event->key()) {
            case (Qt::Key_R):
                  if (event->modifiers() == Qt::ControlModifier)
                      reload();
            break;
    }
}

void KTHelpBrowser::reload()
{
    m_pageArea->reload();
}
