/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktexportwidget.h"

// Qt
#include <QPluginLoader>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QIntValidator>

#include "dglobal.h"
#include "ddebug.h"


KTExportWidget::KTExportWidget(const KTProjectManager *manager, QWidget *parent) : KTModuleWidgetBase(parent), m_manager(manager), m_currentExporter(0), m_currentFormat(ExportInterface::NONE)
{
	DINIT;
	setCaption(tr("Export"));
	
	m_buttons = new QButtonGroup(this);
	connect(m_buttons, SIGNAL(buttonClicked (int)), this, SLOT(makeAction(int)));
	
	QHBoxLayout *layout1 = new QHBoxLayout;
	layout1->addStretch(1);
	
	m_exporterList = new QListWidget;
	connect(m_exporterList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selectExporter(QListWidgetItem *)));
	layout1->addWidget(m_exporterList);
	
	m_formatList = new QListWidget;
	connect(m_formatList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selectFormat(QListWidgetItem *)));
	layout1->addWidget(m_formatList);
	
	setupToExport( layout1);
	setupExportBox( layout1 );
	
	qobject_cast<QVBoxLayout *>(layout())->addLayout(layout1);
	
	loadPlugins();
}

void KTExportWidget::setupToExport(QBoxLayout *mainLayout)
{
	QGroupBox *toExportGroup = new QGroupBox(tr("Choose scene to export"));
	QVBoxLayout *toExportLayout = new QVBoxLayout(toExportGroup);
	
	QRadioButton *allScenes = new QRadioButton(tr("All scenes"));
	allScenes->setChecked(true);
	m_buttons->addButton(allScenes, AllScenes);
	QRadioButton *currentScene = new QRadioButton(tr("Current scene"));
	m_buttons->addButton(currentScene, CurrentScene);
	
	QHBoxLayout *rangeLayout = new QHBoxLayout();
	QRadioButton *scenesRange = new QRadioButton(tr("Range" ));
	rangeLayout->addWidget(scenesRange);
	m_buttons->addButton(scenesRange, SceneRange);
	
	m_fromScene = new QLineEdit("0");
	m_fromScene->setValidator(new QIntValidator(0, 100, m_fromScene));
	rangeLayout->addWidget(m_fromScene);
	
	rangeLayout->addWidget(new QLabel(" to "));
	
	m_toScene = new QLineEdit("0");
	m_toScene->setValidator(new QIntValidator(0, 100, m_toScene));
	rangeLayout->addWidget(m_toScene);
	
	toExportLayout->addWidget(allScenes);
	toExportLayout->addWidget(currentScene);
	toExportLayout->addLayout(rangeLayout);
	
	mainLayout->addWidget(toExportGroup);
}

void KTExportWidget::setupExportBox(QBoxLayout *mainLayout)
{
	QGroupBox *exportBox = new QGroupBox(tr("Export it"));
	QVBoxLayout *exportBoxLayout = new QVBoxLayout(exportBox);
	
	QHBoxLayout *filePathLayout = new QHBoxLayout;
	filePathLayout->addWidget(new QLabel("File: "));
	m_filePath = new QLineEdit();
	filePathLayout->addWidget(m_filePath);
	
	QPushButton *exportIt = new QPushButton(tr("Export"));
	connect(exportIt, SIGNAL(clicked()), this, SLOT(exportIt()));
	
	exportBoxLayout->addLayout(filePathLayout);
	
	exportBoxLayout->addWidget(exportIt);
	
	mainLayout->addWidget(exportBox);
}


KTExportWidget::~KTExportWidget()
{
	DEND;
}

void KTExportWidget::loadPlugins()
{
	QDir pluginDirectory = QDir(HOME+"/plugins/");

	foreach (QString fileName, pluginDirectory.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginDirectory.absoluteFilePath(fileName));
		KTExportPluginObject *plugin = qobject_cast<KTExportPluginObject*>(loader.instance());
		
		if (plugin)
		{
			ExportInterface *exporter = qobject_cast<ExportInterface *>(plugin);
			
			if (exporter)
			{
				QListWidgetItem *newItem = new QListWidgetItem(exporter->key(), m_exporterList);
				m_plugins.insert(newItem, exporter);
			}
			else
				dError() << "Can't load: " << fileName;
		}
	}
}

void KTExportWidget::selectExporter(QListWidgetItem *item)
{
	if ( item )
	{
		m_currentExporter = m_plugins[item];
		addFormats(m_currentExporter->availableFormats());
	}
}

void KTExportWidget::selectFormat(QListWidgetItem *item)
{
	if ( item )
	{
		m_currentFormat = m_formats[item];
	}
}

void KTExportWidget::addFormats(ExportInterface::Formats formats)
{
	m_formats.clear();
	m_formatList->clear();
	
	if ( formats & ExportInterface::SWF )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("Macromedia flash"), m_formatList);
		
		m_formats.insert(format, ExportInterface::SWF);
	}
	
	if ( formats & ExportInterface::MPEG )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("MPEG Video"), m_formatList);
		
		m_formats.insert(format, ExportInterface::MPEG);
	}
	
	if ( formats & ExportInterface::AVI )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("AVI Video"), m_formatList);
		
		m_formats.insert(format, ExportInterface::AVI);
	}
	
	if ( formats & ExportInterface::RM )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("RealMedia Video"), m_formatList);
		
		m_formats.insert(format, ExportInterface::RM);
	}
	
	if ( formats & ExportInterface::ASF )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("ASF Video"), m_formatList);
		
		m_formats.insert(format, ExportInterface::ASF);
	}
	
	if ( formats & ExportInterface::MOV )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("QuickTime Video"), m_formatList);
		
		m_formats.insert(format, ExportInterface::MOV);
	}
	
	if ( formats & ExportInterface::GIF )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("Gif Image"), m_formatList);
		
		m_formats.insert(format, ExportInterface::GIF);
	}
}

void KTExportWidget::makeAction(int buttonId)
{
	m_fromScene->setReadOnly(true);
	m_toScene->setReadOnly(true);
	switch(buttonId)
	{
		case AllScenes:
		{
			
		}
		break;
		case CurrentScene:
		{
			
		}
		break;
		case SceneRange:
		{
			m_fromScene->setReadOnly(false);
			m_toScene->setReadOnly(false);
		}
		break;
	}
}

void KTExportWidget::exportIt()
{
	D_FUNCINFO;
	
	if( m_currentExporter && m_currentFormat != ExportInterface::NONE )
	{
		QString file = fileToExport();
		
		if ( file.isNull() ) return;
		
		dDebug() << "Exporting to file: " << file;
		
		QList<KTScene *> scenes = scenesToExport();
		
		dDebug() << "Exporting " << scenes.count() << " scenas";
		
		if ( scenes.count() > 0)
		{
			m_currentExporter->exportToFormat( file, scenes, m_currentFormat, m_manager->documentSize() );
		}
	}
	else
	{
		emit sendToOSD( tr("Please choose the format before export"), 2);
	}
}

QList<KTScene *> KTExportWidget::scenesToExport()
{
	KTDocument *currentDocument = m_manager->currentDocument();
	
	if (!currentDocument) return QList<KTScene *>();
	
	QList<KTScene *> scenes = currentDocument->scenes();
	
	if ( scenes.count() == 0 ) return QList<KTScene *>();
	
	switch( m_buttons->checkedId() )
	{
		case AllScenes:
		{
			return scenes;
		}
		break;
		case CurrentScene:
		{
			return QList<KTScene *>() << currentDocument->currentScene();
		}
		break;
		case SceneRange:
		{
			
		}
		break;
		
		default: break;
	}
	
	return scenes;
}

QString KTExportWidget::fileToExport() const
{
	QString file = m_filePath->text();
	
	if ( file.isEmpty() || file.isNull() )
	{
		return QString();
	}
	
	switch(m_currentFormat)
	{
		case ExportInterface::SWF:
		{
			if ( !file.toLower().endsWith(".swf") )
			{
				file+=".swf";
			}
		}
		break;
		case ExportInterface::MPEG:
		{
			if ( !file.toLower().endsWith(".mpg") && !file.toLower().endsWith(".mpeg"))
			{
				file+=".mpg";
			}
		}
		break;
		case ExportInterface::AVI:
		{
			if ( !file.toLower().endsWith(".avi"))
			{
				file+=".avi";
			}
		}
		break;
		
		case ExportInterface::RM:
		{
			if ( !file.toLower().endsWith(".rm"))
			{
				file+=".rm";
			}
		}
		break;
		case ExportInterface::ASF:
		{
			if ( !file.toLower().endsWith(".asf"))
			{
				file+=".asf";
			}
		}
		break;
		case ExportInterface::MOV:
		{
			if ( !file.toLower().endsWith(".mov"))
			{
				file+=".mov";
			}
		}
		break;
		
		case ExportInterface::GIF:
		{
			if ( !file.toLower().endsWith(".gif"))
			{
				file+=".gif";
			}
		}
		break;
		
		default: break;
	}
	
	return file;
}


