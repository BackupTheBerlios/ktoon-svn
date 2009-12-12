/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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
#include <QGraphicsView>

#include <kcore/kglobal.h>
#include "ktscene.h"

#include "ktinputdeviceinformation.h"
#include "ktgraphicsscene.h"
#include "ktprojectrequest.h"
#include "ktprojectresponse.h"

#include "ktrequestbuilder.h"

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

void TextTool::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(brushManager);

    QList<QGraphicsItem *> items = scene->items(input->pos());

    if (items.count() > 0) {
        QGraphicsItem *itemPress = items[0];
        if  (itemPressed(itemPress))
             return;
    }

    m_item = new KTTextItem;
    m_item->setPos(input->pos());
}

void TextTool::doubleClick(const KTInputDeviceInformation *input, KTGraphicsScene *scene)
{
    Q_UNUSED(input);
    Q_UNUSED(scene);
}

bool TextTool::itemPressed(QGraphicsItem *item)
{
    if (KTTextItem *text = qgraphicsitem_cast<KTTextItem *>(item)) {
        text->setEditable(true);
        text->setFocus();
        return true;
    }

    return false;
}

void TextTool::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(input);
    Q_UNUSED(scene);
    Q_UNUSED(scene);
    Q_UNUSED(brushManager);
}

void TextTool::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(input);
    Q_UNUSED(brushManager);

    if (m_configurator->text().isEmpty()) {
        delete m_item;
        return;
    }

    if (m_configurator->isHtml())
        m_item->setHtml(m_configurator->text());
    else
        m_item->setPlainText(m_configurator->text());

    m_item->setFont( m_configurator->textFont() );

    scene->addItem(m_item);

    QDomDocument doc;
    doc.appendChild(m_item->toXml(doc));

    KTProjectRequest event = KTRequestBuilder::createItemRequest( scene->currentSceneIndex(), scene->currentLayerIndex(), scene->currentFrameIndex(), scene->currentFrame()->graphics().count(), KTProjectRequest::Add, doc.toString()); // Adds to end

    emit requested(&event);
}

QMap<QString, KAction *> TextTool::actions() const
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

void TextTool::aboutToChangeScene(KTGraphicsScene *scene)
{
}

void TextTool::setupActions()
{
    KAction *pencil = new KAction( QIcon(THEME_DIR+"/icons/text.png"), tr("Text"), this);
    pencil->setShortcut( QKeySequence(tr("Ctrl+T")) );

    m_actions.insert(tr("Text"), pencil);
}

Q_EXPORT_PLUGIN2(kt_textool, TextTool);

