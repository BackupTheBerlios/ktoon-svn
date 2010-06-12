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

#include "ktanimationspace.h"

#include <QMouseEvent>
#include <QDropEvent>
#include <QLinearGradient>
#include <QBoxLayout>

#include <kcore/kdebug.h>

/**
 * This class defines the space which contains the Animation Mode interface.
 * Here is where the visual space for Animation Mode is defined.
 * @author David Cuadrado <krawek@toonka.com>
*/

KTAnimationspace::KTAnimationspace(QWidget *internal, QWidget *parent) : QMainWindow(parent)
{
    // TODO: Try a nice dark color for this window
    // setStyleSheet("QMainWindow { background-color: #d0d0d0; }");
    QWidget *widget = new QWidget();
    QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, widget);
    layout->addWidget(internal, 0, Qt::AlignCenter);
    widget->setLayout(layout);
    setCentralWidget(widget);
}

KTAnimationspace::~KTAnimationspace()
{
}

void KTAnimationspace::mousePressEvent(QMouseEvent *e)
{
    if (e->button () == Qt::RightButton)
        emit contextMenu(mapToGlobal(e->pos()));
}

void KTAnimationspace::mouseMoveEvent(QMouseEvent *e)
{
}

void KTAnimationspace::mouseReleaseEvent(QMouseEvent *e)
{
}

/*
void KTAnimationspace::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasColor()) {
        QColor color = qvariant_cast<QColor>(event->mimeData()->colorData());
        QPalette pal = palette();
        pal.setColor(QPalette::Dark, color);
 
        kDebug() << "Co: " << color;
        setPalette(pal);
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}
 
void KTAnimationspace::dragEnterEvent(QDragEnterEvent *event)
{
    setFocus();

    if (event->mimeData()->hasColor()) 
        event->acceptProposedAction();
    else 
        event->ignore();
 }
*/
