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

#include "kinaswidget.h"
#include <kcore/kdebug.h>

KinasWidget::KinasWidget(QWidget *parent) : KTModuleWidgetBase(parent)
{
    KINIT;

    setWindowTitle(tr("KToon Script"));
    setWindowIcon(QPixmap(THEME_DIR + "icons/color_palette.png"));

    m_splitter = new QSplitter(this);
    addChild(m_splitter);

    m_functionViewer = new KTSFunctionView(m_splitter);
    m_splitter->addWidget(m_functionViewer);

    m_editors = new QTabWidget(this);
    m_splitter->addWidget(m_editors);

    m_splitter->setSizes(QList<int>()<<160 << 400);

    addEditor("Example.kts");
}


KinasWidget::~KinasWidget()
{
    KEND;
}

void KinasWidget::addEditor(const QString &title)
{
    KTSEditor *m_editor = new KTSEditor;
    m_editors->addTab(m_editor, title);
}
