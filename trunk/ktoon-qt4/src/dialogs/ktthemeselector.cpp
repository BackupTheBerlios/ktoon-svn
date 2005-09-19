/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   				   *
 *   krawek@toonka.com   *
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

#include "ktthemeselector.h"
#include <Q3VButtonGroup>
#include <qcheckbox.h>
#include <qlabel.h>
#include <q3listview.h>
#include <q3filedialog.h>
//Added by qt3to4:
#include <QTextStream>
#include <QGridLayout>
#include <Q3ValueList>

#include "ktapplication.h"
#include "ktfiledialog.h"

#include <QDateTime>

#include "ktdebug.h"


KTThemeSelector::KTThemeSelector(QWidget *parent, const char *name) : Q3VBox(parent, name)
{
	setupChooseColor();
	loadSchemes();
}


KTThemeSelector::~KTThemeSelector()
{
}

void KTThemeSelector::setupChooseColor()
{
	Q3HBox *hbox = new Q3HBox(this);
	Q3VBox *box1 = new Q3VBox(hbox);
	box1->setMargin(10);
	m_general = new Q3ButtonGroup(tr("General"), box1);
	
	QGridLayout *layout1 = new QGridLayout(m_general, 4, 2, 15, 5);
	
	QStringList labels1 = QStringList() << tr("Text") << tr("Base") << tr("Foreground") << tr("Background") << tr("Button") << tr("Button Text");
	
	QStringList names = QStringList() << "Text" << "Base" << "Foreground" << "Background" << "Button" << "ButtonText";
	
	QColorGroup colorGroup = QApplication::palette().active();
	
	
	Q3ValueList<QColor> colors = Q3ValueList<QColor>() << colorGroup.text () << colorGroup.base() << colorGroup.foreground() << colorGroup.background() << colorGroup.button() << colorGroup.buttonText();
	
	for(uint i = 0; i < labels1.count(); i++)
	{
		layout1->addWidget(new QLabel(labels1[i], m_general), i, 0 );
		QPushButton *button = new QPushButton(m_general, names[i]);
		button->setPaletteBackgroundColor(colors[i]);
		m_general->insert(button, i);
		layout1->addWidget(button, i, 1);
		m_generalSection.insert(names[i], colors[i].name());
	}
	
	m_effects = new Q3ButtonGroup(tr("Effects"), box1);
	
	QGridLayout *layout2 = new QGridLayout(m_effects, 4, 2, 15, 5);
	
	QStringList labels2 = QStringList() << tr("Light") << tr("Midlight") << tr("Dark") << tr("Mid");
	QStringList names2 = QStringList() << "Light" << "Midlight" << "Dark" << "Mid";
	
	colors.clear();
	colors << colorGroup.light() << colorGroup.midlight() << colorGroup.dark() << colorGroup.mid();
	
	for(uint i = 0; i < labels2.count(); i++)
	{
		layout2->addWidget(new QLabel(labels2[i], m_effects), i, 0 );
		QPushButton *button = new QPushButton(m_effects, names2[i]);
		button->setPaletteBackgroundColor(colors[i]);
		m_effects->insert(button, i);
		layout2->addWidget(button, i, 1);
		m_effectsSection.insert(names2[i], colors[i].name());
	}
	////////////
	
	Q3VBox *box2 = new Q3VBox(hbox);
	box2->setMargin(10);
	m_selections = new Q3ButtonGroup(tr("Selections"), box2);
	
	QGridLayout *layout3 = new QGridLayout(m_selections, 4, 2, 15, 5);
	
	QStringList labels3 = QStringList() << tr("Highlight") << tr("Highlighted Text");
	QStringList names3 = QStringList() << "Highlight" << "HighlightedText";
	colors.clear();
	colors << colorGroup.highlight() << colorGroup.highlightedText();
	
	for(uint i = 0; i < labels3.count(); i++)
	{
		layout3->addWidget(new QLabel(labels3[i], m_selections), i, 0 );
		QPushButton *button = new QPushButton(m_selections, names3[i]);
		button->setPaletteBackgroundColor(colors[i]);
		m_selections->insert(button, i);
		layout3->addWidget(button, i, 1);
		m_selectionsSection.insert(names3[i], colors[i].name());
	}
	
	m_textEffects = new Q3ButtonGroup(tr("Text effects"), box2);
	
	QGridLayout *layout4 = new QGridLayout(m_textEffects, 4, 2, 15, 5);
	QStringList labels4 = QStringList() << tr("Bright Text") << tr("Link") << tr("Link Visited");
	QStringList names4 = QStringList() << "BrightText" << "Link" << "LinkVisited";
	
	colors.clear();
	colors << colorGroup.brightText() << colorGroup.link() << colorGroup.linkVisited();
	
	for(uint i = 0; i < labels4.count(); i++)
	{
		layout4->addWidget(new QLabel(labels4[i], m_textEffects), i, 0 );
		QPushButton *button = new QPushButton(m_textEffects, names4[i]);
		button->setPaletteBackgroundColor(colors[i]);
		m_textEffects->insert(button, i);
		layout4->addWidget(button, i, 1);
		m_textEffectsSection.insert(names4[i], colors[i].name());
	}
	
	QGroupBox *schemeWidget = new QGroupBox(tr("Schema"), box2); // FIXME: add vertical layout
	
	m_allSchemes = new Q3ListView(schemeWidget);
	m_allSchemes->addColumn(tr("Schema"));
	m_allSchemes->addColumn(tr("Owner"));
	m_allSchemes->addColumn(tr("Date"));
	
	connect(m_allSchemes, SIGNAL(doubleClicked ( Q3ListViewItem *, const QPoint &, int )), this, SLOT(loadSchemaFromListView( Q3ListViewItem *, const QPoint &, int )));
	
	QPushButton *saveSchemeButton = new QPushButton(tr("Save schema"), schemeWidget);
	connect(saveSchemeButton, SIGNAL(clicked()), SLOT(saveSchema()));
	
	m_useColors = new QCheckBox(tr("Use this colors"), schemeWidget);
	
	connect(m_general, SIGNAL(clicked(int)), SLOT(chooseGeneralColor(int)));
	connect(m_effects, SIGNAL(clicked(int)), SLOT(chooseEffectsColor(int)));
	connect(m_selections, SIGNAL(clicked(int)), SLOT(chooseSelectionsColor(int)));
	connect(m_textEffects, SIGNAL(clicked(int)), SLOT(chooseTextEffectsColor(int)));
}

void KTThemeSelector::chooseGeneralColor(int id)
{
	QPushButton *btmp = static_cast<QPushButton *>(m_general->find(id)) ;

	QColor c = QColorDialog::getColor(btmp->paletteBackgroundColor(), this);
	
	if ( c.isValid() )
	{
		btmp->setPaletteBackgroundColor(c);
		
		m_generalSection.insert(btmp->name(), c.name());
	}
}

void KTThemeSelector::chooseEffectsColor(int id)
{
	QPushButton *btmp = static_cast<QPushButton *>(m_effects->find(id)) ;

	QColor c = QColorDialog::getColor(btmp->paletteBackgroundColor(), this);
	
	if ( c.isValid() )
	{
		btmp->setPaletteBackgroundColor(c);
		
		m_effectsSection.insert(btmp->name(), c.name());
	}
}

void KTThemeSelector::chooseSelectionsColor(int id)
{
	QPushButton *btmp = static_cast<QPushButton *>(m_selections->find(id)) ;

	QColor c = QColorDialog::getColor(btmp->paletteBackgroundColor(), this);
	if ( c.isValid() )
	{
		btmp->setPaletteBackgroundColor(c);
		
		m_selectionsSection.insert(btmp->name(), c.name());
	}
}

void KTThemeSelector::chooseTextEffectsColor(int id)
{
	QPushButton *btmp = static_cast<QPushButton *>(m_textEffects->find(id));
	QColor c = QColorDialog::getColor(btmp->paletteBackgroundColor(), this);
	if ( c.isValid() )
	{
		btmp->setPaletteBackgroundColor(c);
	
		m_textEffectsSection.insert(btmp->name(), c.name());
	}
}

KTThemeDocument KTThemeSelector::document()
{
	KTThemeDocument doc;
	
	doc.addGeneralSection( m_generalSection);
	doc.addEffectsSection(m_effectsSection);
	doc.addSelections(m_selectionsSection);
	doc.addTextEffect(m_textEffectsSection);
	
	return doc;
}

void KTThemeSelector::loadSchemes()
{
	QDir themeDir(ktapp->configDir()+"/themes");
	
	if ( themeDir.exists() )
	{
		m_allSchemes->clear();
		QFileInfoList files = themeDir.entryInfoList("*.ktt");
		
		for(int i = 0; i < files.count(); i++ )
		{
			QFileInfo iterator = files[i];
			Q3ListViewItem *item = new Q3ListViewItem(m_allSchemes);
			item->setText(0, iterator.fileName());
			item->setText(1, iterator.owner());
			item->setText(2, iterator.created().toString());
		}
	}
}

bool KTThemeSelector::iWantApplyColors()
{
	return m_useColors->isChecked();
}

void KTThemeSelector::saveSchema()
{
	QDir themeDir(ktapp->configDir()+"/themes");
	if ( ! themeDir.exists() )
	{
		themeDir.mkdir(themeDir.path());
	}
	
	KTFileDialog saveDialog(KTFileDialog::Themes, this);
	
	if ( saveDialog.exec() == QDialog::Rejected )
	{
		return;
	}
	
	QFile file(themeDir.path()+"/"+saveDialog.fileName());
	
	if (file.open(QIODevice::WriteOnly ) )
	{
		QTextStream stream( &file );
		stream << document().toString() << endl;
		file.close();
		m_lastFile = themeDir.path()+"/"+saveDialog.fileName();
	}
	loadSchemes();
}

QString KTThemeSelector::lastFile()
{
	return m_lastFile;
}

void KTThemeSelector::loadSchemaFromListView(Q3ListViewItem *item, const QPoint &, int )
{
	if ( item )
	{
		ktapp->applyTheme(ktapp->configDir()+"/themes/"+item->text(0));
		
		if ( ! item->text(0).isEmpty() )
		{
			KTCONFIG->configDocument()->setThemePath(ktapp->configDir()+"/themes/"+item->text(0));
			
			m_lastFile = ktapp->configDir()+"/themes/"+item->text(0);
		}
	}
}
