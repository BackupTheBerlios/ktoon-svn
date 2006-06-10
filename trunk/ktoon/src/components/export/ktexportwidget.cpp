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
#include <QToolButton>
#include <QFileDialog>

#include "dglobal.h"
#include "ddebug.h"


KTExportWidget::KTExportWidget(const KTProjectManager *manager, QWidget *parent) : KTModuleWidgetBase(parent), m_manager(manager), m_currentExporter(0), m_currentFormat(KTExportInterface::NONE)
{
	DINIT;
	setCaption(tr("Export"));
	
	setWindowIcon(QIcon(THEME_DIR+"/icons/export.png"));
	
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
	filePathLayout->addWidget(new QLabel(tr("File: ")));
	
	m_filePath = new QLineEdit();
	filePathLayout->addWidget(m_filePath);
	
	QToolButton *button = new QToolButton;
	
	button->setIcon(QIcon(THEME_DIR+"/icons/open.png"));
	
	connect(button, SIGNAL(clicked()), this, SLOT(chooseFile()));
	
	filePathLayout->addWidget(button);
	
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
			KTExportInterface *exporter = qobject_cast<KTExportInterface *>(plugin);
			
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

void KTExportWidget::addFormats(KTExportInterface::Formats formats)
{
	m_formats.clear();
	m_formatList->clear();
	
	if ( formats & KTExportInterface::SWF )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("Macromedia flash"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::SWF);
	}
	
	if ( formats & KTExportInterface::MPEG )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("MPEG Video"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::MPEG);
	}
	
	if ( formats & KTExportInterface::AVI )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("AVI Video"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::AVI);
	}
	
	if ( formats & KTExportInterface::RM )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("RealMedia Video"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::RM);
	}
	
	if ( formats & KTExportInterface::ASF )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("ASF Video"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::ASF);
	}
	
	if ( formats & KTExportInterface::MOV )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("QuickTime Video"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::MOV);
	}
	
	if ( formats & KTExportInterface::GIF )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("Gif Image"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::GIF);
	}
	
	if ( formats & KTExportInterface::PNG )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("PNG Image Array"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::PNG);
	}
	
	if ( formats & KTExportInterface::JPEG )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("JPEG Image Array"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::JPEG);
	}
	
	if ( formats & KTExportInterface::SMIL )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("SMIL"), m_formatList);
		
		m_formats.insert(format, KTExportInterface::SMIL);
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
	
	if( m_currentExporter && m_currentFormat != KTExportInterface::NONE )
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
		case KTExportInterface::SWF:
		{
			if ( !file.toLower().endsWith(".swf") )
			{
				file+=".swf";
			}
		}
		break;
		case KTExportInterface::MPEG:
		{
			if ( !file.toLower().endsWith(".mpg") && !file.toLower().endsWith(".mpeg"))
			{
				file+=".mpg";
			}
		}
		break;
		case KTExportInterface::AVI:
		{
			if ( !file.toLower().endsWith(".avi"))
			{
				file+=".avi";
			}
		}
		break;
		
		case KTExportInterface::RM:
		{
			if ( !file.toLower().endsWith(".rm"))
			{
				file+=".rm";
			}
		}
		break;
		case KTExportInterface::ASF:
		{
			if ( !file.toLower().endsWith(".asf"))
			{
				file+=".asf";
			}
		}
		break;
		case KTExportInterface::MOV:
		{
			if ( !file.toLower().endsWith(".mov"))
			{
				file+=".mov";
			}
		}
		break;
		
		case KTExportInterface::GIF:
		{
			if ( !file.toLower().endsWith(".gif"))
			{
				file+=".gif";
			}
		}
		break;
		
		case KTExportInterface::SMIL:
		{
			if ( !file.toLower().endsWith(".smil"))
			{
				file+=".smil";
			}
		}
		break;
		
		default: break;
	}
	
	return file;
}

void KTExportWidget::chooseFile()
{
// 	QFileDialog fileDialog;
// 	fileDialog.setFileMode(QFileDialog::Directory);
// 	
// 	if ( fileDialog.exec() != QDialog::Rejected )
// 	{
// 		m_filePath->setText(fileDialog.selectedFiles()[0] );
// 	}
	QString file = QFileDialog::getSaveFileName(this, tr("Choose a filename to save under"), QDir::homePath());
	m_filePath->setText(file);
}
