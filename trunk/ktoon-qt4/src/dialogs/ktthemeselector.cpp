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

// Qt
#include <QFileDialog>
#include <QTextStream>
#include <QGridLayout>
#include <QList>
#include <QDateTime>
#include <QHeaderView>
#include <QCheckBox>

// KToon
#include "ktapplication.h"
#include "ktfiledialog.h"

#include "ktdebug.h"
#include "kseparator.h"

KTThemeSelector::KTThemeSelector(QWidget *parent) : KTVHBox(parent, Qt::Vertical)
{
	setupChooseColor();
	loadSchemes();
}


KTThemeSelector::~KTThemeSelector()
{
}

void KTThemeSelector::setupChooseColor()
{
	KTVHBox *hbox = new KTVHBox(this, Qt::Horizontal);
	KTVHBox *box1 = new KTVHBox(hbox, Qt::Vertical);
	box1->boxLayout()->setMargin(10);
	m_general = new QGroupBox(tr("General"), box1);
	
	QGridLayout *layout1 = new QGridLayout(m_general, 4, 2, 15, 5);
	
	QStringList labels1 = QStringList() << tr("Text") << tr("Base") << tr("Foreground") << tr("Background") << tr("Button") << tr("Button Text");
	
	QStringList names = QStringList() << "Text" << "Base" << "Foreground" << "Background" << "Button" << "ButtonText";
	
	QColorGroup colorGroup = QApplication::palette().active();
	
	
	QList<QColor> colors = QList<QColor>() << colorGroup.text () << colorGroup.base() << colorGroup.foreground() << colorGroup.background() << colorGroup.button() << colorGroup.buttonText();
	
	for(uint i = 0; i < labels1.count(); i++)
	{
		layout1->addWidget(new QLabel(labels1[i], m_general), i, 0 );
		QPushButton *button = new QPushButton(m_general, names[i]);
		
		QPalette pal = button->palette();
		pal.setColor(QPalette::Button, colors[i]);
		button->setPalette(pal);
		
		m_generalButtonGroup.addButton(button);
		layout1->addWidget(button, i, 1);
		m_generalSection.insert(names[i], colors[i].name());
	}
	
	m_effects = new QGroupBox(tr("Effects"), box1);
	
	QGridLayout *layout2 = new QGridLayout(m_effects, 4, 2, 15, 5);
	
	QStringList labels2 = QStringList() << tr("Light") << tr("Midlight") << tr("Dark") << tr("Mid");
	QStringList names2 = QStringList() << "Light" << "Midlight" << "Dark" << "Mid";
	
	colors.clear();
	colors << colorGroup.light() << colorGroup.midlight() << colorGroup.dark() << colorGroup.mid();
	
	for(uint i = 0; i < labels2.count(); i++)
	{
		layout2->addWidget(new QLabel(labels2[i], m_effects), i, 0 );
		QPushButton *button = new QPushButton(m_effects, names2[i]);
		QPalette pal = button->palette();
		pal.setColor(QPalette::Button, colors[i]);
		button->setPalette(pal);
		m_effectsButtonGroup.addButton(button);
		layout2->addWidget(button, i, 1);
		m_effectsSection.insert(names2[i], colors[i].name());
	}
	////////////
	
	KTVHBox *box2 = new KTVHBox(hbox, Qt::Vertical);
	box2->boxLayout()->setMargin(10);
	m_selections = new QGroupBox(tr("Selections"), box2);
	
	QGridLayout *layout3 = new QGridLayout(m_selections, 4, 2, 15, 5);
	
	QStringList labels3 = QStringList() << tr("Highlight") << tr("Highlighted Text");
	QStringList names3 = QStringList() << "Highlight" << "HighlightedText";
	colors.clear();
	colors << colorGroup.highlight() << colorGroup.highlightedText();
	
	for(uint i = 0; i < labels3.count(); i++)
	{
		layout3->addWidget(new QLabel(labels3[i], m_selections), i, 0 );
		QPushButton *button = new QPushButton(m_selections, names3[i]);
		QPalette pal = button->palette();
		pal.setColor(QPalette::Button, colors[i]);
		button->setPalette(pal);
		m_selectionsButtonGroup.addButton(button);
		layout3->addWidget(button, i, 1);
		m_selectionsSection.insert(names3[i], colors[i].name());
	}
	
	m_textEffects = new QGroupBox(tr("Text effects"), box2);
	
	QGridLayout *layout4 = new QGridLayout(m_textEffects, 4, 2, 15, 5);
	QStringList labels4 = QStringList() << tr("Bright Text") << tr("Link") << tr("Link Visited");
	QStringList names4 = QStringList() << "BrightText" << "Link" << "LinkVisited";
	
	colors.clear();
	colors << colorGroup.brightText() << colorGroup.link() << colorGroup.linkVisited();
	
	for(uint i = 0; i < labels4.count(); i++)
	{
		layout4->addWidget(new QLabel(labels4[i], m_textEffects), i, 0 );
		QPushButton *button = new QPushButton(m_textEffects, names4[i]);
		QPalette pal = button->palette();
		pal.setColor(QPalette::Button, colors[i]);
		button->setPalette(pal);
		m_textEffectsButtonGroup.addButton(button);
		layout4->addWidget(button, i, 1);
		m_textEffectsSection.insert(names4[i], colors[i].name());
	}
	
	QGroupBox *schemeWidget = new QGroupBox(tr("Schema"), box2); // FIXME: add vertical layout
	QVBoxLayout *schemaLayout = new QVBoxLayout;
	
	m_allSchemes = new QTreeWidget;
	m_allSchemes->setHeaderLabels ( QStringList() << tr("Schema") << tr( "Owner" ) << tr( "Date" ) );
	m_allSchemes->header()->setResizeMode(QHeaderView::Stretch);
	
	schemaLayout->addWidget(m_allSchemes);
	
	connect(m_allSchemes, SIGNAL(itemDoubleClicked (QTreeWidgetItem *, int )), this, SLOT(loadSchemaFromListView( QTreeWidgetItem *, int )));
	
	QPushButton *saveSchemeButton = new QPushButton(tr("Save schema"));
	connect(saveSchemeButton, SIGNAL(clicked()), SLOT(saveSchema()));
	
	schemaLayout->addWidget(saveSchemeButton);
	
	schemeWidget->setLayout(schemaLayout);
	
	new KSeparator(this);
	m_useColors = new QCheckBox(tr("Use this colors"), this);
	
	connect(&m_generalButtonGroup, SIGNAL(buttonClicked(QAbstractButton * )), SLOT(chooseGeneralColor(QAbstractButton * )));
	connect(&m_effectsButtonGroup, SIGNAL(buttonClicked(QAbstractButton * )), SLOT(chooseEffectsColor(QAbstractButton * )));
	connect(&m_selectionsButtonGroup, SIGNAL(buttonClicked(QAbstractButton * )), SLOT(chooseSelectionsColor(QAbstractButton * )));
	connect(&m_textEffectsButtonGroup, SIGNAL(buttonClicked(QAbstractButton * )), SLOT(chooseTextEffectsColor(QAbstractButton * )));
}

void KTThemeSelector::chooseGeneralColor(QAbstractButton *  button)
{
	QColor c = QColorDialog::getColor(button->palette().color(QPalette::Button), this);
	
	if ( c.isValid() )
	{
		QPalette pal = button->palette();
		pal.setColor(QPalette::Button, c);
		button->setPalette(pal);
		
		m_generalSection.insert(button->name(), c.name());
	}
}

void KTThemeSelector::chooseEffectsColor(QAbstractButton *  button)
{
	QColor c = QColorDialog::getColor(button->palette().color(QPalette::Button), this);
	
	if ( c.isValid() )
	{
		QPalette pal = button->palette();
		pal.setColor(QPalette::Button, c);
		button->setPalette(pal);
		
		m_effectsSection.insert(button->name(), c.name());
	}
}

void KTThemeSelector::chooseSelectionsColor(QAbstractButton *  button)
{
	QColor c = QColorDialog::getColor(button->palette().color(QPalette::Button), this);
	
	if ( c.isValid() )
	{
		QPalette pal = button->palette();
		pal.setColor(QPalette::Button, c);
		button->setPalette(pal);
		
		m_selectionsSection.insert(button->name(), c.name());
	}
}

void KTThemeSelector::chooseTextEffectsColor(QAbstractButton *  button)
{
	QColor c = QColorDialog::getColor(button->palette().color(QPalette::Button), this);
	if ( c.isValid() )
	{
		QPalette pal = button->palette();
		pal.setColor(QPalette::Button, c);
		button->setPalette(pal);
		
		m_textEffectsSection.insert(button->name(), c.name());
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
			QTreeWidgetItem *item = new QTreeWidgetItem(m_allSchemes);
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

void KTThemeSelector::loadSchemaFromListView(QTreeWidgetItem *item, int /*column*/)
{
	if ( item )
	{
		ktapp->applyTheme(ktapp->configDir()+"/themes/"+item->text(0));
		
		if ( ! item->text(0).isEmpty() )
		{
			KT_FUNCINFO;
			KTCONFIG->beginGroup("General");
			KTCONFIG->setValue( "ThemeFile", ktapp->configDir()+"/themes/"+item->text(0) );
			
			m_lastFile = ktapp->configDir()+"/themes/"+item->text(0);
		}
	}
}
