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

#include "ktexposuretable.h"
#include "ktexposureheader.h"

#include <QPainter>
#include <QItemDelegate>
#include <QLineEdit>
#include <QMouseEvent>
#include <QMenu>
#include <QHeaderView>

#include <kcore/kdebug.h>
#include <ktglobal.h>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 */

/////////////////
class KTExposureVerticalHeader : public QHeaderView
{
    //Q_OBJECT

    public:
        KTExposureVerticalHeader(QWidget * parent = 0);
        ~KTExposureVerticalHeader();
        void paintSection(QPainter *painter, const QRect & rect, int logicalIndex) const;
};

KTExposureVerticalHeader::KTExposureVerticalHeader(QWidget * parent) : QHeaderView(Qt::Vertical, parent)
{
    //setClickable(true);
    setFixedWidth(25);
}

KTExposureVerticalHeader::~KTExposureVerticalHeader()
{
}

void KTExposureVerticalHeader::paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const
{
    Q_UNUSED(logicalIndex);

    if (!rect.isValid())
        return;

    QStyleOptionHeader headerOption;
    headerOption.rect = rect;
    headerOption.orientation = Qt::Vertical;
    headerOption.position = QStyleOptionHeader::Middle;
    headerOption.text = "";

    QStyle::State state = QStyle::State_None;

    if (isEnabled())
        state |= QStyle::State_Enabled;

    if (window()->isActiveWindow())
        state |= QStyle::State_Active;
 
    style()->drawControl(QStyle::CE_HeaderSection, &headerOption, painter);

    QString text;
    text = text.setNum(logicalIndex + 1);
    QFont font("Arial", 7, QFont::Normal, false);
    QFontMetrics fm(font);

    int x = rect.normalized().x() + ((rect.normalized().width() - fm.width(text))/2);
    int y = rect.normalized().bottomLeft().y() - (1 + (rect.normalized().height() - fm.height())/2);

    painter->setFont(font);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter->drawText(x, y, text);
}

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

        /* Useful code
        if (item->data(KTExposureTable::IsLocked).toBool()) {
             QPixmap pixmap(THEME_DIR + "icons/padlock.png");
             painter->drawPixmap(1, 5, pixmap);
        } else 
        */

        if (item->data(KTExposureTable::IsUsed).toBool() && !item->data(KTExposureTable::IsLocked).toBool()) {
            int x = option.rect.topLeft().x() + 2;
            int y = option.rect.topLeft().y() + 2;
            int w = option.rect.bottomRight().x() - x - 2;
            int h = option.rect.bottomRight().y() - y - 2;

            painter->setPen(QColor(100,100,100,50));
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
    KTExposureVerticalHeader *verticalHeader = new KTExposureVerticalHeader(this);
    setVerticalHeader(verticalHeader);

    setItemDelegate(new KTExposureItemDelegate(this));
    k->removingLayer = false;

    QTableWidgetItem *prototype = new QTableWidgetItem();

    prototype->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable);
    prototype->setTextAlignment(Qt::AlignCenter);
    setItemPrototype(prototype);

    setRowCount(100);

    for (int i=0; i < 100; i++)
         setRowHeight(i, 20);

    k->header = new KTExposureHeader(this);

    connect(k->header, SIGNAL(visibilityChanged(int, bool)), this, SIGNAL(requestChangeVisibilityLayer(int, bool)));
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

void KTExposureTable::emitRequestSelectFrame(int currentSelectedRow, int currentColumn, int previousRow, int previousColumn)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (currentSelectedRow >= framesTotal())
        return;

    if (!k->removingLayer) {
        if (previousRow != currentSelectedRow || previousColumn != currentColumn)
            emit requestSelectFrame(currentLayer(), currentRow());

        if ((previousColumn != currentColumn) || (columnCount() == 1)) 
             k->header->updateSelection(currentColumn);
    } else {
        k->removingLayer = false;
        if (previousColumn == 0) {
            selectFrame(1, previousRow);
        } else {
            selectFrame(currentColumn, currentSelectedRow);
            k->header->updateSelection(currentColumn);
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
    frame->setFont(QFont("Arial", 7, QFont::Normal, false));

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
    return k->header->visualIndex(currentColumn());
}

int KTExposureTable::currentFrame() const
{
    QTableWidgetItem *frame = currentItem();

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
    setColumnWidth(index, 70);
    k->header->insertLayer(index, name);
}

void KTExposureTable::insertFrame(int indexLayer, int indexFrame, const QString & name, bool external)
{
    QTableWidgetItem *frame = new QTableWidgetItem;
    frame->setBackgroundColor(QColor(0xe6e6e6));
    frame->setFont(QFont("Arial", 7, QFont::Normal, false));
    frame->setSizeHint(QSize(65, 10));
    frame->setText(name);
    frame->setData(IsUsed, true);
    frame->setTextAlignment(Qt::AlignCenter);

    int logicalIndex = k->header->logicalIndex(indexLayer);

    k->header->setLastFrame(logicalIndex, k->header->lastFrame(logicalIndex) + 1);

    setItem(k->header->lastFrame(logicalIndex)-1, logicalIndex, frame);

    for (int i = k->header->lastFrame(logicalIndex)-1; i > indexFrame; i--)
         exchangeFrame(indexLayer, i , indexLayer, i-1, external);

    if (k->header->lastFrame(logicalIndex) == rowCount())
        setRowCount(k->header->lastFrame(logicalIndex) + 50);
}

void KTExposureTable::setLockFrame(int indexLayer, int indexFrame, bool locked)
{
    int logicalIndex = k->header->logicalIndex(indexLayer);
    QTableWidgetItem * frame = item(indexFrame, logicalIndex);

    if (frame) {
        if (frame->data(IsUsed).toBool()) {
            if (locked)
                frame->setBackgroundColor(Qt::red);
            else
                frame->setBackgroundColor(QColor(0xe6e6e6));
                //frame->setBackgroundColor(Qt::lightGray);

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
    k->header->removeLayer(logicalIndex);
    removeColumn(logicalIndex);

    setUpdatesEnabled(true);
}

void KTExposureTable::removeFrame(int indexLayer, int indexFrame)
{
    setUpdatesEnabled(false);
    QTableWidgetItem *i  = takeItem(indexFrame, indexLayer);
    if (i) {
        for (int index = indexFrame + 1; index < k->header->lastFrame(indexLayer); index++) {
             QTableWidgetItem * idx  = takeItem(index, indexLayer);
             if (idx)
                 setItem(index - 1, indexLayer, idx);
        }
             k->header->setLastFrame(indexLayer, k->header->lastFrame(indexLayer)-1);
    }
    setUpdatesEnabled(true);
}

void KTExposureTable::exchangeFrame(int oldPosLayer, int oldPosFrame, int newPosLayer, int newPosFrame, bool external)
{
    kFatal() << "KTExposureTable::moveFrame <- exchanging 2 frames";

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

void KTExposureTable::emitRequestSetUsedFrame(int indexFrame, int indexLayer)
{
    int layer = k->header->visualIndex(indexLayer);
    int lastFrame = k->header->lastFrame(indexLayer); 

    if (indexFrame >= lastFrame) {
        for (int j=0; j<columnCount(); j++) {
             for (int i=lastFrame; i <= indexFrame; i++)
                  emit requestSetUsedFrame(j, i);
        }

        emit requestSelectFrame(layer, indexFrame);
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

int KTExposureTable::layersTotal()
{
    return k->header->layersTotal();
}

int KTExposureTable::framesTotal()
{
    return k->header->lastFrame(0);
}

