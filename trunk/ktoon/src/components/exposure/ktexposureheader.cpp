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

#include "ktexposureheader.h"

#include <QHeaderView>
#include <QPainter>
#include <QStyleOptionButton>
#include <QMap>
#include <QVector>
#include <QItemDelegate>
#include <QLineEdit>
#include <QMouseEvent>
#include <QMenu>

#include <kcore/kdebug.h>
#include <ktglobal.h>

/*
struct LayerItem
{
    QString title;
    int lastFrame;
    bool isVisible;
    bool isLocked;
};
*/

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 */

KTExposureHeader::KTExposureHeader(QWidget * parent) : QHeaderView(Qt::Horizontal, parent), m_sectionEdited(-1),
                                                       m_blockSectionMoved(false)
{
    setClickable(true);

    //SQA: Disabled while layers movability is enhanced 
    //setMovable(true);

    connect(this, SIGNAL(sectionDoubleClicked(int)), this, SLOT(showEditorName(int)));

    m_editor = new QLineEdit(this);
    m_editor->setFocusPolicy(Qt::ClickFocus);
    m_editor->setInputMask("");
    connect(m_editor, SIGNAL(editingFinished()), this, SLOT(hideEditorName()));
    m_editor->hide();
}

KTExposureHeader::~KTExposureHeader()
{
}

void KTExposureHeader::emitVisibilityChanged(int section)
{
   emit visibilityChanged(visualIndex(section), !m_layers[section].isVisible);
}

void KTExposureHeader::setVisibilityChanged(int logicalndex, bool visibility)
{
    // Q_UNUSED(visibility);
    // FIXME: in ktexpousertable.cpp visibility or !m_layers[logicalndex].isVisible
    // m_layers[logicalndex].isVisible = !m_layers[logicalndex].isVisible;
    m_layers[logicalndex].isVisible = visibility;

    updateSection(logicalndex);
}

void KTExposureHeader::showEditorName(int section)
{
    QFont font("Arial", 8, QFont::Normal, false);
    m_editor->setFont(font);

    int x = sectionViewportPosition(section);
    m_editor->setGeometry(x, 0, sectionSize(section), height());
    m_sectionEdited = section;
    m_editor->setText(m_layers[section].title);
    m_editor->show();
    m_editor->setFocus();
}

void KTExposureHeader::hideEditorName()
{
    m_editor->hide();

    if (m_sectionEdited != -1 && m_editor->isModified())
        emit changedName(m_sectionEdited, m_editor->text());

    m_sectionEdited = -1;
}

void KTExposureHeader::insertLayer(int logicalIndex, const QString &text)
{
    LayerItem layer;
    layer.title = text;
    layer.lastFrame = 0;
    layer.isVisible = true;
    layer.isLocked = false;
    m_layers.insert(logicalIndex, layer);
}

void KTExposureHeader::setLayerName(int logicalIndex, const QString &text)
{
    m_layers[logicalIndex].title = text;
    updateSection(logicalIndex);
}

bool KTExposureHeader::signalMovedBlocked()
{
    return m_blockSectionMoved;
}

void KTExposureHeader::setLockLayer(int logicalndex, bool lock)
{
    m_layers[logicalndex].isLocked = lock;
    updateSection(logicalndex);
}

void KTExposureHeader::moveLayer(int position, int newPosition)
{
    m_blockSectionMoved = true;
    moveSection(position, newPosition);
    m_blockSectionMoved = false;
}

int KTExposureHeader::lastFrame(int logicalIndex)
{
    return m_layers[logicalIndex].lastFrame;
}

void KTExposureHeader::removeLayer(int logicalIndex)
{
    m_layers.remove(logicalIndex);
}

void KTExposureHeader::setLastFrame(int logicalIndex, int num)
{
    m_layers[logicalIndex].lastFrame = num;
}

void KTExposureHeader::mousePressEvent(QMouseEvent * event)
{
    int section = logicalIndexAt(event->pos());
    int x = sectionViewportPosition(section) + 3;

    QRect rect(x+3, 3, height()-3, height()-3);
    if (rect.contains(event->pos()))
        emitVisibilityChanged(section);
    else
        QHeaderView::mousePressEvent(event);
}

void KTExposureHeader::paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const
{
    if (!rect.isValid()) 
        return;

    QStyleOptionHeader headerOption;
    headerOption.rect = rect;
    headerOption.orientation = Qt::Horizontal;
    headerOption.position = QStyleOptionHeader::Middle;
    headerOption.text = "";

    QStyle::State state = QStyle::State_None;

    if (isEnabled())
        state |= QStyle::State_Enabled;

    if (window()->isActiveWindow())
        state |= QStyle::State_Active;

    style()->drawControl(QStyle::CE_HeaderSection, &headerOption, painter);

    QString text = m_layers[logicalIndex].title;
    QFont font("Arial", 8, QFont::Normal, false);
    QFontMetrics fm(font);

    QStyleOptionButton buttonOption;

    if (m_layers[logicalIndex].isVisible) {
        buttonOption.palette.setBrush(QPalette::Button, Qt::green);
    } else {
        buttonOption.palette.setBrush(QPalette::Button, Qt::red);
        buttonOption.state |= QStyle::State_Sunken;
        QColor color(255, 0, 0, 40);
        painter->fillRect(rect.normalized().adjusted(0, 1, 0, -1), color);
    }

    if ((logicalIndex == currentCol) || (m_layers.size() == 1)) {
        QColor color(250, 209, 132, 80);
        painter->fillRect(rect.normalized().adjusted(0, 1, 0, -1), color);
        if (m_layers[logicalIndex].isVisible) {
            painter->setPen(QPen(QColor(250, 209, 132, 255), 2, Qt::SolidLine));
            painter->drawRect(rect.normalized().adjusted(0, 1, 0, -1));
        } else {
            painter->setPen(QPen(QColor(255, 0, 0, 70), 2, Qt::SolidLine));
            painter->drawRect(rect.normalized().adjusted(0, 1, 0, -1));
        }
    }

    int buttonWidth = 12;
    int width = (rect.normalized().width() - (fm.width(text) + buttonWidth) + 4)/ 2;
    int x = rect.normalized().x() + width + buttonWidth;
    int y = rect.normalized().bottomLeft().y() - (1 + (rect.normalized().height() - fm.height())/2);

    painter->setFont(font);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter->drawText(x, y, text);

    buttonOption.rect = QRect(rect.x() + width - 4, rect.y() + ((rect.normalized().height()-buttonWidth)/2) + 1, buttonWidth, buttonWidth);
  
    style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);
}

void KTExposureHeader::updateSelection(int col)
{
    currentCol = col;
    updateSection(col);
}

int KTExposureHeader::layersTotal()
{
    return m_layers.size();
}

//#include "ktexposuretable.moc"
