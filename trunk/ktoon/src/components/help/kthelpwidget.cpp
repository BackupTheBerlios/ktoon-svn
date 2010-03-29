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

#include "kthelpwidget.h"
#include <qlayout.h>
#include <qtabwidget.h>
#include <qdom.h>
#include <qfile.h>
#include <qmap.h>

#include <QLocale>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

// KOM
#include <kcore/kdebug.h>
#include <kcore/kglobal.h>

KTHelpWidget::KTHelpWidget(const QString &path, QWidget *parent) : KTModuleWidgetBase(parent)
{
    setWindowTitle(tr("Help"));
    setWindowIcon(QPixmap(THEME_DIR + "icons/help.png"));

    if (QString(QLocale::system().name()).length() > 1)
        m_helpPath = path + QString(QLocale::system().name()).left(2);
    else
        m_helpPath = path + "en";

    QTreeWidget *contentsListView = new QTreeWidget(this);
    contentsListView->setHeaderLabels(QStringList() << tr(""));
    contentsListView->header()->hide();

    connect(contentsListView, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, 
                              SLOT(tryToLoadPage(QTreeWidgetItem *, int)));

    addChild(contentsListView);

    QDomDocument document;
    QFile file(m_helpPath.path() + "/help.xml");
    QTreeWidgetItem *first = new QTreeWidgetItem;

    if (file.open(QIODevice::ReadOnly)) {
        if (document.setContent(&file)) {
            QDomElement root = document.documentElement();
            QDomNode section = root.firstChild();

            while (!section.isNull()) {
                   QDomElement element = section.toElement();
                   if (!element.isNull()) {
                       if (element.tagName() == "Section") {
                           QTreeWidgetItem *item = new QTreeWidgetItem(contentsListView);
                           item->setText(0, element.attribute("title"));

                           QDomNode subSection = element.firstChild();
                           while (! subSection.isNull()) {
                                  QDomElement element2 = subSection.toElement();
                                  if (!element2.isNull()) {
                                      if (element2.tagName() == "SubSection") {
                                          QTreeWidgetItem *subitem = new QTreeWidgetItem(item);
                                          subitem->setText(0, element2.attribute("title"));
                                          m_files.insert(subitem, element2.attribute("file"));
                                          if (element2.attribute("file").compare("init.html") == 0)
                                              first = subitem;
                                      }
                                  }
                                  subSection = subSection.nextSibling();
                           }
                       }
                   }
                   section = section.nextSibling();
            }
        } else {
                 qDebug("KTHelpWidget::Can't set contents");
        }
        file.close();
    } else {
        qDebug("KTHelpWidget::Can't open");
    }

    contentsListView->show();
    contentsListView->expandAll();
    if (first)
        contentsListView->setCurrentItem(first);
}

KTHelpWidget::~KTHelpWidget()
{
}

void KTHelpWidget::tryToLoadPage(QTreeWidgetItem *item, int)
{
    if (item) {
        QString fileName = m_files[item];
        if (! fileName.isNull())
            loadPage(m_helpPath.path()+"/"+ fileName);
    }
}

void KTHelpWidget::loadPage(const QString &filePath)
{
    emit pageLoaded(filePath);
}

QString KTHelpWidget::helpPath () const
{
    return m_helpPath.path();
}
