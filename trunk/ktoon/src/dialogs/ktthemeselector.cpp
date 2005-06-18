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
#include <qvgroupbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qfiledialog.h>
#include <iostream>
#include "ktapplication.h"


KTThemeSelector::KTThemeSelector(QWidget *parent, const char *name) : QVBox(parent, name)
{
	setupChooseColor();
}


KTThemeSelector::~KTThemeSelector()
{
}

void KTThemeSelector::setupChooseColor()
{
	QHBox *hbox = new QHBox(this);
	QVBox *box1 = new QVBox(hbox);
	box1->setMargin(10);
	m_general = new QButtonGroup(tr("General"), box1);
	
	QGridLayout *layout1 = new QGridLayout(m_general, 4, 2, 15, 5);
	
	QStringList labels1 = QStringList() << tr("Text") << tr("Base") << tr("Foreground") << tr("Background") << tr("Button") << tr("Button Text");
	
	QStringList names = QStringList() << "Text" << "Base" << "Foreground" << "Background" << "Button" << "ButtonText";
	
	QColorGroup colorGroup = QApplication::palette().active();
	
	
	QValueList<QColor> colors = QValueList<QColor>() << colorGroup.text () << colorGroup.base() << colorGroup.foreground() << colorGroup.background() << colorGroup.button() << colorGroup.buttonText();
	
	for(uint i = 0; i < labels1.count(); i++)
	{
		layout1->addWidget(new QLabel(labels1[i], m_general), i, 0 );
		QPushButton *button = new QPushButton(m_general, names[i]);
		button->setPaletteBackgroundColor(colors[i]);
		m_general->insert(button, i);
		layout1->addWidget(button, i, 1);
	}
	
	m_effects = new QButtonGroup(tr("Effects"), box1);
	
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
	}
	////////////
	
	QVBox *box2 = new QVBox(hbox);
	box2->setMargin(10);
	m_selections = new QButtonGroup(tr("Selections"), box2);
	
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
	}
	
	m_textEffects = new QButtonGroup(tr("Text effects"), box2);
	
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
	}
	
	QVGroupBox *schemeWidget = new QVGroupBox(tr("Schema"), box2);
	
	m_allSchemes = new QListView(schemeWidget);
	m_allSchemes->addColumn(tr("Schema"), m_allSchemes->width());
	
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
	// Reviso en el directorio por defecto de themes y tomo los nombres
}

bool KTThemeSelector::iWantApplyColors()
{
	return m_useColors->isChecked();
}

void KTThemeSelector::saveSchema()
{
	// por ahora lo hago con QFontDialog, la idea es utilizar un widget propio para darle nombre y version al theme
	
	QString filepath = QFileDialog::getSaveFileName(QDir::homeDirPath(),"KToon Themes (*.ktheme)",this, tr("save file"), tr("Choose a file") );
	
	if ( ! filepath.contains(".ktheme",false) )
	{
		filepath += ".ktheme";
	}
	
	QFile file(filepath);
	
	if (file.open(IO_WriteOnly ) )
	{
		QTextStream stream( &file );
		stream << document().toString() << endl;
		file.close();
		m_lastFile = filepath;
	}

}

QString KTThemeSelector::lastFile()
{
	return m_lastFile;
}

