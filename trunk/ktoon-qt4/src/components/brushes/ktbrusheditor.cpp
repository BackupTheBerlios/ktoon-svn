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

#include "ktbrusheditor.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

#include "ktdebug.h"

KTBrushEditor::KTBrushEditor(QWidget *parent) : QWidget(parent)
{
}


KTBrushEditor::~KTBrushEditor()
{
}

void KTBrushEditor::paintEvent(QPaintEvent *e)
{
	ktDebug() << e;
}

void KTBrushEditor::mousePressEvent(QMouseEvent *e)
{
	ktDebug() << e;
}
