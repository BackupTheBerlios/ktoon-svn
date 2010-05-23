/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
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

#include "ktipdialog.h"
#include "kseparator.h"
#include "kconfig.h"
#include "kdebug.h"

#include <QVBoxLayout>
#include <QTextBrowser>
#include <QTextFrame>
#include <QTextFrameFormat>
#include <QCheckBox>
#include <QPushButton>
#include <QDomDocument>
#include <QFile>

KTipDialog::KTipDialog(QStringList &labels, const QString &file, QWidget *parent) : QDialog(parent)
{
    tags = labels;
    m_database = new KTipDatabase(file);
    setupGUI();
}

KTipDialog::KTipDialog(QStringList &labels, KTipDatabase *database, QWidget *parent) : QDialog(parent), m_database(database)
{
    tags = labels;
    setupGUI();
}

void KTipDialog::setupGUI()
{
    setWindowTitle(tags.at(0));
    setWindowIcon(QPixmap(THEME_DIR + "icons/today_tip.png"));
    
    int h,s,v;
    QColor baseColor = palette().base().color();
    baseColor.getHsv(&h,&s,&v);
    baseColor.setHsv(h, int(s*(71/76.0)), int(v*(67/93.0)));
    // baseColor.setHsv(h, int(s*(10/6.0)), int(v*(93/99.0)));
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_textArea = new QTextBrowser;
    
    QTextFrameFormat format = m_textArea->document()->rootFrame()->frameFormat();
    format.setMargin(15);
    format.setBorder(5);
    m_textArea->document()->rootFrame()->setFrameFormat(format);
    
    m_textArea->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    m_textArea->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    m_textArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        
    QPalette pal = m_textArea->palette();
    pal.setBrush(QPalette::Base, baseColor);
    
    m_textArea->setPalette(pal);
    
    layout->addWidget(m_textArea);
    layout->addWidget(new KSeparator);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    
    m_showOnStart = new QCheckBox(tags.at(1));
    buttonLayout->addWidget(m_showOnStart);
    connect(m_showOnStart, SIGNAL(clicked()), this, SLOT(setShowOnStart()));
    
    buttonLayout->addStretch(1);
    
    QPushButton *prevTip = new QPushButton(tags.at(2));
    buttonLayout->addWidget(prevTip);
    connect(prevTip, SIGNAL(clicked()), this, SLOT(showPrevTip()));
    
    QPushButton *nextTip = new QPushButton(tags.at(3));
    buttonLayout->addWidget(nextTip);
    connect(nextTip, SIGNAL(clicked()), this, SLOT(showNextTip()));
    
    QPushButton *close = new QPushButton(tags.at(4));
    buttonLayout->addWidget(close);
    connect(close, SIGNAL(clicked()), this, SLOT(accept()));
    
    layout->addLayout(buttonLayout);
    
    setAttribute(Qt::WA_DeleteOnClose, true);
    
    KCONFIG->beginGroup("TipOfDay");
    m_showOnStart->setChecked(qvariant_cast<bool>(KCONFIG->value("ShowOnStart", true)));
    
    showNextTip();
}

KTipDialog::~KTipDialog()
{
}

void KTipDialog::showPrevTip()
{
    m_database->prevTip();
    KTip tip = m_database->tip();
    m_textArea->setHtml(tip.text);
}

void KTipDialog::showNextTip()
{
    m_database->nextTip();
    KTip tip = m_database->tip();
    m_textArea->setHtml(tip.text);
}

void KTipDialog::setShowOnStart()
{
    KCONFIG->beginGroup("TipOfDay");
    KCONFIG->setValue("ShowOnStart", m_showOnStart->isChecked());
}

