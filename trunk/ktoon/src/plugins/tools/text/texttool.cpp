/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "texttool.h"

#include <QPointF>
#include <QFontMetrics>
#include <QKeySequence>

#include <dglobal.h>
#include "ktscene.h"

TextTool::TextTool()
{
	m_configurator = new TextConfigurator;
	setupActions();
}

TextTool::~TextTool()
{
}

QStringList TextTool::keys() const
{
	return QStringList() << tr("Text");
}

void TextTool::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	m_item = new KTTextItem;
	
	m_item->setPos(input->pos());
	
	scene->addGraphic(m_item);
}

void TextTool::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
}

void TextTool::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	if ( m_configurator->isHtml() )
	{
		m_item->setHtml(m_configurator->text());
	}
	else
	{
		m_item->setPlainText(m_configurator->text());
	}
	
	QDomDocument doc;
	doc.appendChild(m_item->toXml( doc ));
	
	KTItemEvent *event = new KTItemEvent(KTProjectEvent::Add, scene->index(), scene->currentLayerIndex(), scene->currentFrameIndex(), -1, doc.toString()); // Adds to end
	
	addProjectEvent(event);
}


QMap<QString, DAction *> TextTool::actions() const
{
	return m_actions;
}

int TextTool::toolType() const
{
	return Brush;
}
		
QWidget *TextTool::configurator()
{
	return m_configurator;
}

void TextTool::aboutToChangeTool()
{
}

void TextTool::setupActions()
{
	DAction *pencil = new DAction( QIcon(THEME_DIR+"/icons/text.png"), tr("Text"), this);
	pencil->setShortcut( QKeySequence(tr("Ctrl+T")) );
	
	m_actions.insert( tr("Text"), pencil );
}

Q_EXPORT_PLUGIN2( kt_textool, TextTool );

