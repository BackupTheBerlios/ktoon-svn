/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>

#include "ktpreferences.h"
#include "ktpaintareaconfig.h"

#include <kcore/kglobal.h>
#include <kcore/kdebug.h>
#include <kgui/kapplication.h>
#include <kgui/kformfactory.h>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

/**
 * This class handles the preferences dialog for KToon.
 * Here is where the forms of the KToon preferences dialog are defined.
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTPreferences::GeneralPage : public QWidget
{
    Q_OBJECT
    public:
        GeneralPage();
        ~GeneralPage();
        void saveValues();
        // KToon::RenderType m_renderType;
        
    private:
        QLineEdit *m_home, *m_repository, *m_browser;
        QComboBox *m_renderType;
        QCheckBox *m_openLastProject;
        
    // private slots:
    // void syncRenderType(int index);
};

KTPreferences::GeneralPage::GeneralPage()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    // m_renderType = KToon::RenderType(0);
    KCONFIG->beginGroup("General");
    
    m_home = new QLineEdit;
    
    QString str = KCONFIG->value("Home").toString();
    if (!str.isEmpty())
        m_home->setText(str);
    
    m_repository = new QLineEdit;
    str = KCONFIG->value("Cache").toString();
    if (!str.isEmpty())
        m_repository->setText(str);
    
    m_browser = new QLineEdit;
    str = KCONFIG->value("Browser").toString();
    if (!str.isEmpty())
        m_browser->setText(str);
    
    str = KCONFIG->value("RenderType").toString();

    m_renderType = new QComboBox();
    
    m_renderType->addItem(tr("Image"),  KToon::Image);
    m_renderType->addItem(tr("OpenGL"), KToon::OpenGL);
    m_renderType->addItem(tr("Native"), KToon::Native);
    if (!str.isEmpty())
        m_renderType->setCurrentIndex(str.toInt());
    
    bool openLast = KCONFIG->value("OpenLastProject", true).toBool();
    m_openLastProject = new QCheckBox();
    m_openLastProject->setChecked(openLast);

    QLayout *form = KFormFactory::makeGrid( QStringList() << tr("KToon Home") << tr("Cache") << tr("Browser") 
                    << tr("Render Type") << tr("Open last project"), QWidgetList() << m_home 
                    << m_repository << m_browser << m_renderType << m_openLastProject);
    
    layout->addLayout(form);
    layout->addStretch(3);
}

KTPreferences::GeneralPage::~GeneralPage()
{
}

void KTPreferences::GeneralPage::saveValues()
{
    KCONFIG->beginGroup("General");
    
    QString str = m_home->text();
    if (!str.isEmpty() && m_home->isModified())
        KCONFIG->setValue("Home", str);
    
    str = m_repository->text();
    if (!str.isEmpty() && m_repository->isModified())
        KCONFIG->setValue("Cache", str);
    
    str = m_browser->text();
    if (!str.isEmpty() && m_browser->isModified())
        KCONFIG->setValue("Browser", str);
    
    KCONFIG->setValue("RenderType", QString::number((m_renderType->itemData(m_renderType->currentIndex ()).toInt())));
    KCONFIG->setValue("OpenLastProject", m_openLastProject->isChecked());
    KCONFIG->sync();
}

class KTPreferences::FontPage : public QWidget
{
    Q_OBJECT;
    public:
        FontPage();
        ~FontPage();
        
        QFont currentFont() const;
        
    private:
        KFontChooser *m_fontChooser;
};

KTPreferences::FontPage::FontPage()
{
    m_fontChooser = new KFontChooser(this);
    m_fontChooser->setCurrentFont(font());
}

KTPreferences::FontPage::~FontPage()
{
}

QFont KTPreferences::FontPage::currentFont() const
{
    return m_fontChooser->currentFont();
}

//--------------- CONSTRUCTOR --------------------

KTPreferences::KTPreferences(QWidget *parent) : KConfigurationDialog(parent)
{
    setWindowTitle(tr("Application KTPreferences"));
    
    m_generalPage = new GeneralPage;
    addPage(m_generalPage, tr("General"), QPixmap(THEME_DIR + "icons/general_config.png"));
    
    m_themeSelector = new KTThemeSelector;
    addPage(m_themeSelector, tr("Theme preferences"), QPixmap(THEME_DIR + "icons/theme_config.png"));
    
    m_fontChooser = new FontPage;
    addPage(m_fontChooser, tr("Font"), QPixmap(THEME_DIR + "icons/font_config.png"));
    
    m_drawingAreaProperties = new KTPaintAreaConfig;
    addPage(m_drawingAreaProperties, tr("Paint area"), QIcon(THEME_DIR + "icons/drawing_area.png"));
    
    // resize(400,400);
}

//-------------- DESTRUCTOR -----------------

KTPreferences::~KTPreferences()
{
}

void KTPreferences::ok()
{
    apply();
    KConfigurationDialog::ok();
}

void KTPreferences::apply()
{
    if (static_cast<KTThemeSelector *>(currentPage()) ==  m_themeSelector) {
        if (m_themeSelector->applyColors())
            kApp->applyTheme(m_themeSelector->document());
    }
    else if (static_cast<GeneralPage *>(currentPage()) == m_generalPage) {
             m_generalPage->saveValues();
    }
    else if (qobject_cast<FontPage *>(currentPage()) == m_fontChooser) {
             kApp->setFont(m_fontChooser->currentFont());
    }
    else if (qobject_cast<KTPaintAreaConfig *>(currentPage()) == m_drawingAreaProperties) {
    }
}

#include "ktpreferences.moc"

