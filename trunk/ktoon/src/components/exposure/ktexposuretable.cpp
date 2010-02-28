/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#include "ktexposuretable.h"

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

struct LayerItem
{
    QString title;
    int lastFrame;
    bool isVisible;
    bool isLocked;
};

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 */
class KTExposureHeader: public QHeaderView
{
    Q_OBJECT;
    public:
        KTExposureHeader(QWidget * parent = 0);
        ~KTExposureHeader();
        void paintSection(QPainter *painter, const QRect & rect, int logicalIndex) const;
        void insertLayer(int logicalIndex, const QString &text);
        void setLayerName(int logicalIndex, const QString &text);
        void setLastFrame(int logicalIndex, int num);
        int lastFrame(int logicalIndex);
        void removeLayer(int logicalIndex);
        void moveLayer(int index, int newIndex);
        void setLockLayer(int logicalndex, bool lock);
        bool signalMovedBlocked();
        void setVisibilityChanged(int logicalndex, bool visibility);
        int layersTotal();

    public slots:
        void updateSelection(int col);

    private slots:
        void emitVisiblityChanged(int section);
        void showEditorName(int section);
        void hideEditorName();

    protected:
        virtual void mousePressEvent(QMouseEvent * event);

    private:
        QVector<LayerItem> m_layers;
        QLineEdit *m_editor;
        int m_sectionEdited;
        int m_blockSectionMoved;
        int currentCol;

    signals:
        void changedName(int indexLayer, const QString & name);
        void visiblityChanged(int indexLayer, bool visibility);
};

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

void KTExposureHeader::emitVisiblityChanged(int section)
{
   emit visiblityChanged(visualIndex(section), !m_layers[section].isVisible);
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
    kFatal() << "KTExposureHeader::insertLayer -> logicalIndex: " << logicalIndex;
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
        emitVisiblityChanged(section);
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

    int height = rect.height() - 5;

    QString text = m_layers[logicalIndex].title;
    QFontMetrics fm( painter->font());

    int x = rect.x() + (sectionSize(logicalIndex) - fm.width(text))/2;
    int y = fm.height() + (rect.y() / 2);

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
        QColor border(250, 209, 132, 255);
        painter->setPen(QPen(border, 2, Qt::SolidLine));
        painter->drawRect(rect.normalized().adjusted(0, 1, 0, -1));
    }

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter->drawText(x, y, text);

    height -= 4;
    buttonOption.rect = QRect(rect.x()+3, rect.y() + ((rect.height()-height)/2) + 1, height, height);
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

#include "ktexposuretable.moc"

/////////////////

class KTExposureItemDelegate : public QItemDelegate
{
    public:
        KTExposureItemDelegate(QObject * parent = 0);
        ~KTExposureItemDelegate();
        virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

KTExposureItemDelegate::KTExposureItemDelegate(QObject * parent) :  QItemDelegate(parent)
{
}

KTExposureItemDelegate::~KTExposureItemDelegate()
{
}

void KTExposureItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QItemDelegate::paint(painter, option, index);

    KTExposureTable *table = qobject_cast<KTExposureTable *>(index.model()->parent());

    QTableWidgetItem *item = table->itemFromIndex(index);

    if (item) {

        //if (item->data(KTExposureTable::IsLocked).toBool()) {
        //     QPixmap pixmap(THEME_DIR + "icons/kilit_pic.png");
        //     painter->drawPixmap(1, 5, pixmap);
        //} else 

        if (item->data(KTExposureTable::IsUsed).toBool() && !item->data(KTExposureTable::IsLocked).toBool()) {
            int x = option.rect.topLeft().x() + 2;
            int y = option.rect.topLeft().y() + 2;
            int w = option.rect.bottomRight().x() - x - 2;
            int h = option.rect.bottomRight().y() - y - 2;

            painter->setPen(Qt::gray);
            painter->drawRect(x, y, w, h);
        }
    }
}

////////////////////////

struct KTExposureTable::Private
{
    KTExposureHeader *header;
    QMenu *menu;
    bool removingLayer;
};

KTExposureTable::KTExposureTable(QWidget * parent) : QTableWidget(parent), k(new Private)
{
    setItemDelegate(new KTExposureItemDelegate(this));
    k->removingLayer = false;

    QTableWidgetItem *prototype = new QTableWidgetItem();

    prototype->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable);
    prototype->setTextAlignment(Qt::AlignCenter);
    setItemPrototype(prototype);

    setRowCount(100);

    k->header = new KTExposureHeader(this);

    connect(k->header, SIGNAL(visiblityChanged(int, bool)), this, SIGNAL(requestChangeVisiblityLayer(int, bool)));
    connect(k->header, SIGNAL(changedName(int, const QString &)), this, SIGNAL(requestRenameLayer(int, 
                              const QString & )));
    connect(k->header, SIGNAL(sectionMoved (int, int, int)), this, SLOT(emitRequestMoveLayer(int, int, int)));

    setHorizontalHeader(k->header);

    connect(this, SIGNAL(cellClicked(int, int)), this, SLOT(emitRequestSetUsedFrame(int, int)));

    connect(this, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(emitRequestSelectFrame(int, int, int, int)));

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);

    k->menu = 0;
}

void KTExposureTable::emitRequestRenameFrame(QTableWidgetItem * item)
{
    QModelIndex  index = indexFromItem(item);
    emit requestRenameFrame(index.column(), index.row(), item->text());
}

void KTExposureTable::emitRequestSelectFrame(int currentRow_, int currentColumn_, int previousRow, int previousColumn)
{
    if (!k->removingLayer) {
        if (previousRow != currentRow_ || previousColumn != currentColumn_)
            emit requestSelectFrame(currentLayer(), currentRow());

        if ((previousColumn != currentColumn_) || (columnCount() == 1)) 
             k->header->updateSelection(currentColumn_);
    } else {
        k->removingLayer = false;
        if (previousColumn == 0) {
            selectFrame(1, previousRow);
        } else {
            selectFrame(currentColumn_, currentRow_);
            k->header->updateSelection(currentColumn_);
        }
    }
}

void KTExposureTable::emitRequestMoveLayer(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);

    if (! k->header->signalMovedBlocked()) {
        k->header->moveLayer(newVisualIndex, oldVisualIndex);
        emit requestMoveLayer(oldVisualIndex, newVisualIndex);
    }
}

KTExposureTable::~KTExposureTable()
{
    delete k;
}

QString KTExposureTable::frameName(int indexLayer, int indexFrame)
{
    QTableWidgetItem *frame = item(indexFrame , indexLayer);
    if (frame)
        return frame->text();

    return "";
}

void KTExposureTable::setFrameName(int indexLayer, int indexFrame,const QString & name)
{
    QTableWidgetItem *frame = item(indexFrame , indexLayer);
    if (frame) {
        if (frame->text() != name)
            frame->setText(name);
    }
}

void KTExposureTable::setLayerName(int indexLayer, const QString & name)
{
    k->header->setLayerName(k->header->logicalIndex(indexLayer), name);
}

bool KTExposureTable::frameIsLocked(int indexLayer, int indexFrame)
{
    QTableWidgetItem *frame = item(indexFrame, indexLayer);
    if (frame) {
        return frame->data(IsLocked).toBool();
    } else {
        #ifdef K_DEBUG
               kWarning() << "frame " << indexLayer << "," << indexFrame << " does not exist";
        #endif
    }
    return false;
}

void KTExposureTable::selectFrame(int indexLayer, int indexFrame)
{
    setCurrentCell(indexFrame, k->header->logicalIndex(indexLayer));
}

void KTExposureTable::setMenu(QMenu *menu)
{
    k->menu = menu;
}

int KTExposureTable::currentLayer() const
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    return k->header->visualIndex(currentColumn());
}

int KTExposureTable::currentFrame() const
{
    QTableWidgetItem *frame = currentItem ();

    if (frame) {
        if (frame->data(IsUsed).toBool())
            return currentRow();
        else
            return currentRow()-1; //???
    }

    return currentRow()-1; //???
}

void KTExposureTable::insertLayer(int index, const QString & name)
{
    insertColumn(index);
    k->header->insertLayer(index, name);
}

void KTExposureTable::setUseFrame(int indexLayer, int indexFrame, const QString & name, bool external)
{
    QTableWidgetItem * frame = new QTableWidgetItem;
    frame->setBackgroundColor(Qt::lightGray);
    frame->setText(name);
    frame->setData(IsUsed, true);
    frame->setTextAlignment(Qt::AlignCenter);

    int logicalIndex = k->header->logicalIndex(indexLayer);

    k->header->setLastFrame(logicalIndex, k->header->lastFrame(logicalIndex) + 1);

    setItem(k->header->lastFrame(logicalIndex)-1, logicalIndex, frame);

    for (int i = k->header->lastFrame(logicalIndex)-1; i > indexFrame; i--)
         moveFrame(indexLayer, i , indexLayer, i-1, external);

    if (k->header->lastFrame(logicalIndex) == rowCount())
        setRowCount(k->header->lastFrame(logicalIndex) + 50);
}

void KTExposureTable::setLockFrame(int indexLayer, int indexFrame, bool locked)
{
    int logicalIndex = k->header->logicalIndex (indexLayer);
    QTableWidgetItem * frame = item(indexFrame, logicalIndex);

    if (frame) {
        if (frame->data(IsUsed).toBool()) {
            if (locked)
                frame->setBackgroundColor(Qt::red);
            else
                frame->setBackgroundColor(Qt::lightGray);

            frame->setData(IsLocked, locked);
        }
    }
}

void KTExposureTable::setLockLayer(int indexLayer, bool locked)
{
    k->header->setLockLayer(indexLayer, locked);
}

void KTExposureTable::setVisibilityChanged(int visualIndex, bool visibility)
{
    k->header->setVisibilityChanged(k->header->logicalIndex(visualIndex), visibility);
}

void KTExposureTable::removeLayer(int indexLayer)
{
    setUpdatesEnabled(false);

    k->removingLayer = true;
    int logicalIndex = k->header->logicalIndex(indexLayer);

    kFatal() << "KTExposureTable::removeLayer - logicalIndex: " << logicalIndex;

    k->header->removeLayer(logicalIndex);
    removeColumn(logicalIndex);

    setUpdatesEnabled(true);
}

void KTExposureTable::removeFrame(int indexLayer, int indexFrame)
{
    setUpdatesEnabled(false);
    QTableWidgetItem *i  = takeItem(indexFrame, indexLayer);
    if (i) {
        for (int index = indexFrame+1; index < k->header->lastFrame(indexLayer); index++) {
             QTableWidgetItem * idx  = takeItem(index, indexLayer);
             if (idx)
                 setItem(index-1, indexLayer, idx);
        }
             k->header->setLastFrame(indexLayer, k->header->lastFrame(indexLayer)-1);
    }
    setUpdatesEnabled(true);
}

void KTExposureTable::moveFrame(int oldPosLayer, int oldPosFrame, int newPosLayer, int newPosFrame, bool external)
{
    QTableWidgetItem * oldItem  = takeItem(oldPosFrame, oldPosLayer);
    QTableWidgetItem * newItem  = takeItem(newPosFrame, newPosLayer);

    setItem(newPosFrame, newPosLayer, oldItem);
    setItem(oldPosFrame, oldPosLayer, newItem);
    //FIXME remove this

    if (!external)
        setCurrentItem(oldItem);
}

void KTExposureTable::moveLayer(int oldPosLayer, int newPosLayer)
{
    k->header->moveLayer(oldPosLayer, newPosLayer);
}

void KTExposureTable::emitRequestSetUsedFrame(int indexFrame,  int indexLayer)
{
    int visualIndex = k->header->visualIndex(indexLayer);

    if (indexFrame == k->header->lastFrame(indexLayer)) {
        emit requestSetUsedFrame(visualIndex, indexFrame);
        emit requestSelectFrame(visualIndex, indexFrame);
    }
}

int KTExposureTable::usedFrames(int column) const
{
    return k->header->lastFrame(column);
}

bool KTExposureTable::edit(const QModelIndex & index, EditTrigger trigger, QEvent *event)
{
    QTableWidgetItem *item = itemFromIndex(index);

    if (item) {
        if (item->data(IsUsed).toBool())
            QTableWidget::edit(index, trigger, event);
        else
            return false;
    }

    return false;
}

void KTExposureTable::mousePressEvent(QMouseEvent * event)
{
    QTableWidget::mousePressEvent(event);
    if (event->button() == Qt::RightButton) {
        if (k->menu) 
            k->menu->exec(event->globalPos());
    }
}

void KTExposureTable::commitData(QWidget *editor)
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    QTableWidget::commitData(0); // Don't rename

    if (lineEdit)
        emit requestRenameFrame(currentLayer(),  currentFrame(), lineEdit->text());
}

