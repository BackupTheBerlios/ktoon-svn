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

#include "ktlayermanager.h"

#include <qlabel.h>
#include <qlayout.h>

#include <QPixmap>
#include <QHeaderView>
#include <QPainter>
#include <QItemDelegate>

#include <kgui/kseparator.h>
#include <kgui/kapplication.h>
#include <kcore/kdebug.h>

const int LAYER_COLUMN = 0;
const int LOCK_COLUMN = 1;
const int VIEW_COLUMN = 2;

// Header

class KTLayerManagerHeader : public QHeaderView
{
    public:
        KTLayerManagerHeader(QWidget * parent = 0);
        ~KTLayerManagerHeader();
        void paintSection(QPainter *painter, const QRect & rect, int logicalIndex) const;
         
    private:
        QPixmap m_lockIcon;
        QPixmap m_viewIcon;
};

KTLayerManagerHeader::KTLayerManagerHeader(QWidget * parent) : QHeaderView(Qt::Horizontal , parent)
{
    setClickable(true);
    setCascadingSectionResizes(true);
    setMaximumHeight(20);
    
    m_lockIcon = QPixmap(THEME_DIR + "icons/padlock.png");
    m_viewIcon = QPixmap(THEME_DIR + "icons/show_hide_all_layers.png");
}

KTLayerManagerHeader::~KTLayerManagerHeader()
{
}

void KTLayerManagerHeader::paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const
{
    if (!rect.isValid()) 
        return;
    
    QStyleOptionHeader headerOption;
    headerOption.rect = rect;
    headerOption.orientation = Qt::Horizontal;
    headerOption.position = QStyleOptionHeader::Middle;
    
    QStyle::State state = QStyle::State_None;

    if (isEnabled())
        state |= QStyle::State_Enabled;

    if (window()->isActiveWindow())
        state |= QStyle::State_Active;
    
    style()->drawControl(QStyle::CE_HeaderSection, &headerOption, painter);
    
    //painter->drawRect(rect.normalized().adjusted(0, 1, 0, -1));
    
    QString text = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();;
    
    //QFontMetrics fm(painter->font());
    QFont label("Arial", 9, QFont::Bold, false); 
    QFontMetrics fm(label);
    
    int x = rect.x() + (sectionSize(logicalIndex) - fm.width( text ))/2;
    //int y = fm.height() + (rect.y() / 2);
    int y = 14;
    painter->setFont(label); 
    painter->drawText(x, y, text);
    
    QPen originalPen = painter->pen();
    
    // painter->setPen(QPen(palette().text().color(), 2));
    // painter->drawRect(rect.normalized().adjusted(1, 1, -2, -2));
    
    painter->setPen(originalPen);
    
    switch (logicalIndex) {
            case LAYER_COLUMN:
            {
                 // nothing...
            }
            break;
            case LOCK_COLUMN:
            {
                 painter->drawPixmap(QPointF(rect.x() + (rect.width()-10)/2, rect.y() + 3), m_lockIcon, QRectF(0, 0, 10, 13));
            }
            break;
            case VIEW_COLUMN:
            {
                 painter->drawPixmap(QPointF(rect.x() + (rect.width()-16)/2, rect.y() + 2), m_viewIcon, QRectF(0, 0, 16, 16));
            }
            break;
    }
}

// DELEGATE

class KTLayerManagerItemDelegate : public QItemDelegate
{
    public:
        KTLayerManagerItemDelegate(QObject * parent = 0);
        ~KTLayerManagerItemDelegate();
        virtual void drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &, Qt::CheckState state) const;
        virtual void drawFocus(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect) const;
        virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

KTLayerManagerItemDelegate::KTLayerManagerItemDelegate(QObject * parent) :  QItemDelegate(parent)
{
}

KTLayerManagerItemDelegate::~KTLayerManagerItemDelegate()
{
}

void KTLayerManagerItemDelegate::drawCheck(QPainter *painter, const QStyleOptionViewItem &option,
                                           const QRect &, Qt::CheckState state) const
{
    if (option.rect.x() > 0) {
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;

        QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
                                              check(option, option.rect, Qt::Checked).size(),
                                              QRect(option.rect.x() + textMargin, option.rect.y(),
                                              option.rect.width() - (textMargin * 2), option.rect.height()));
        QItemDelegate::drawCheck(painter, option, checkRect, state);
    }
}

void KTLayerManagerItemDelegate::drawFocus(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect) const
{
}

void KTLayerManagerItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QItemDelegate::paint(painter, option, index);

    KTLayerManager *table = qobject_cast<KTLayerManager *>(index.model()->parent());
    QTableWidgetItem *item = table->itemFromIndex(index);

    if (item) {
        switch (index.column()) {
                case LAYER_COLUMN:
                {
                     if (item->isSelected()) {
                         painter->setPen(QPen(QColor(255, 190, 31, 255), 1));
                         painter->drawRect(option.rect.normalized().adjusted(1,1,-2, -2));
                     } 
                }
                break;
                case LOCK_COLUMN:
                case VIEW_COLUMN:
                {
                     /*
                     QStyleOptionButton checkOption;
                     checkOption.state = option.state;

                     if (item->checkState() == Qt::Checked)
                         checkOption.state |= QStyle::State_On;
       
                     checkOption.rect = option.rect.normalized().adjusted(0,2,-2,-2);
                     checkOption.rect = QRect(0, 0, 4, 4); 
                     table->style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkOption, painter);
                     */
                }
                break;
        }
    }
}

////////////////////////////////

struct KTLayerManager::Private
{
    Private() : allSelected(false), allVisible(true), allLock(false), rowHeight(20) {}
    
    bool allSelected, allVisible, allLock;
    int rowHeight;
};

KTLayerManager::KTLayerManager(QWidget *parent) : QTableWidget(0, 3, parent), k(new Private)
{
    #ifdef K_DEBUG
        KINIT;
    #endif

    setSelectionMode(QAbstractItemView::SingleSelection);
    
    QTableWidgetItem *prototype = new QTableWidgetItem;
    prototype->setTextAlignment(Qt::AlignCenter);
    prototype->setBackgroundColor(palette().text().color());
    prototype->setTextColor(palette().background().color());
    
    setItemPrototype(prototype);
    
    setHorizontalHeaderLabels(QStringList() << tr("Layers") << tr("") << tr(""));
    verticalHeader()->hide();
    
    setHorizontalHeader(new KTLayerManagerHeader(this));
    setItemDelegate(new KTLayerManagerItemDelegate(this));

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(emitSelectionSignal()));

    /*
    connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(emitRequestRenameLayer( 
    QTableWidgetItem *)));
    */
}

KTLayerManager::~KTLayerManager()
{
    #ifdef K_DEBUG
        KEND;
    #endif

    delete k;
}

void KTLayerManager::insertLayer(int position, const QString &name)
{
    if (position >= 0 && position <= rowCount()) {

        QTableWidgetItem *newLayer = new QTableWidgetItem(name);
        newLayer->setFont(QFont("Arial", 8, QFont::Normal, false));
        newLayer->setTextAlignment(Qt::AlignCenter);
        
        newLayer->setBackgroundColor(palette().background().color());
        newLayer->setTextColor(palette().foreground().color());
        
        insertRow(position);

        setItem(position, 0, newLayer);
        fixSize();
        
        QTableWidgetItem *lockItem = new QTableWidgetItem;
        lockItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        lockItem->setCheckState(Qt::Unchecked);
        
        setItem(position, 1, lockItem);
        
        QTableWidgetItem *viewItem = new QTableWidgetItem;
        viewItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        viewItem->setCheckState(Qt::Checked);
        
        setItem(position, 2, viewItem);

    }
}

void KTLayerManager::insertSoundLayer(int position, const QString &name)
{
    if (position >= 0 && position <= rowCount()) {
        QTableWidgetItem *newLayer = new QTableWidgetItem(name);
        newLayer->setTextAlignment(Qt::AlignCenter);
        
        newLayer->setBackgroundColor(palette().background().color());
        newLayer->setTextColor(palette().foreground().color());
        
        insertRow(position);
        setItem(position, 0, newLayer);
        fixSize();

        /*
        QTableWidgetItem *lockItem = new QTableWidgetItem;
        lockItem->setFlags(Qt::ItemIsUserCheckable  | Qt::ItemIsEnabled);
        lockItem->setCheckState( Qt::Unchecked);
        setItem(position, 1, lockItem);
        QTableWidgetItem *viewItem = new QTableWidgetItem;
        viewItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        viewItem->setCheckState( Qt::Checked);
        setItem(position, 2, viewItem);
        */
    }
}

void KTLayerManager::removeLayer(int position)
{
    removeRow(verticalHeader()->logicalIndex(position));
}

void KTLayerManager::renameLayer(int position, const QString &name)
{
    QTableWidgetItem *item = this->item(0, verticalHeader()->logicalIndex(position));
    
    if (item)
        item->setText(name);
}

void KTLayerManager::resizeEvent(QResizeEvent *)
{
    fixSize();
}

void KTLayerManager::fixSize()
{
    int offset = 0;

    if (verticalScrollBar()->isVisible())
        offset = verticalScrollBar()->width()-2;
    else
        offset = -2;
    
    int width = this->width() - offset;
        
    horizontalHeader()->resizeSection(0, width-(k->rowHeight*2)-8);
    horizontalHeader()->resizeSection(1, k->rowHeight);
    horizontalHeader()->resizeSection(2, k->rowHeight);
    
    for (int row = 0; row < rowCount(); row++)
         verticalHeader()->resizeSection(row, k->rowHeight);
}

void KTLayerManager::setRowHeight(int rowHeight)
{
    k->rowHeight = rowHeight;
}

void KTLayerManager::commitData(QWidget *editor)
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    
    QTableWidget::commitData(0); // Don't rename
    
    if (lineEdit)
        emit requestRenameEvent(verticalHeader()->visualIndex(currentRow()), lineEdit->text());
}

void KTLayerManager::moveLayer(int position, int newPosition)
{
    if (position < 0 || position >= rowCount() || newPosition < 0 || newPosition >= rowCount()) 
        return;
    
    verticalHeader()->moveSection((position), (newPosition));

    /*
     QTableWidgetItem *item1 = takeItem(position, 0);
         
     bool up = true;
     if (position > newPosition) {
         up = false; // down
     }
         
     if (up) {
         for (int i = position+1; i <= newPosition; i++) {
              setItem(i-1, 0, takeItem(i, 0));
         }
     } else {
         for (int i = position-1; i >= newPosition; i--) {
              setItem(i+1, 0, takeItem(i, 0));
         }
     }
        
     setItem(newPosition, 0, item1);
     setCurrentItem(item1);
    */
}


void KTLayerManager::lockLayer(int position, bool locked)
{
    if (position < 0 || position >= rowCount()) 
        return;
    
    QTableWidgetItem *item = this->item(verticalHeader()->logicalIndex(position), LOCK_COLUMN);
    
    if (item) {
        if (locked)
            item->setCheckState (Qt::Checked);
        else
            item->setCheckState (Qt::Unchecked);
    }
}

void KTLayerManager::emitSelectionSignal()
{
    kFatal() << "KTLayerManager::emitSelectionSignal() : HERE WE GOOO!";
}
