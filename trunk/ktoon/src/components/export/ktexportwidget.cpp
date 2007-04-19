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

#include <dcore/dglobal.h>
#include <dcore/ddebug.h>

#include <dgui/ditemselector.h>
#include <dgui/dxyspinbox.h>

class SelectPlugin : public DWizardPage
{
	Q_OBJECT;
	public:
		SelectPlugin();
		~SelectPlugin();
		
		bool isComplete() const;
		void reset();
		
		void addPlugin(const QString &plugin);
		
	public slots:
		void selected(QListWidgetItem *);
		
	signals:
		void selectedPlugin(const QString &plugin);
		
	private:
		QListWidget *m_exporterList;
};

SelectPlugin::SelectPlugin() : DWizardPage(tr("Select plugin"))
{
	m_exporterList = new QListWidget;
	connect(m_exporterList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selected(QListWidgetItem *)));
	setWidget(m_exporterList);
	
	reset();
}

SelectPlugin::~SelectPlugin()
{
}

bool SelectPlugin::isComplete() const
{
	return m_exporterList->selectedItems().count() > 0;
}

void SelectPlugin::reset()
{
	m_exporterList->clearSelection();
}

void SelectPlugin::addPlugin(const QString &plugin)
{
	new QListWidgetItem(plugin, m_exporterList);
}

void SelectPlugin::selected(QListWidgetItem *item)
{
	if( item )
	{
		emit selectedPlugin(item->text());
		emit completed();
	}
}

class SelectFormat : public DWizardPage
{
	Q_OBJECT;
	public:
		SelectFormat();
		~SelectFormat();
		
		bool isComplete() const;
		void reset();
		
		void setFormats(KTExportInterface::Formats formats);
		
	private slots:
		void selected(QListWidgetItem *item);
		
	signals:
		void formatSelected(int format);
		
	private:
		QListWidget *m_formatList;
};

SelectFormat::SelectFormat() : DWizardPage(tr(""))
{
	m_formatList = new QListWidget;
	connect(m_formatList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(selected(QListWidgetItem *)));
	setWidget(m_formatList);
	
	reset();
}

SelectFormat::~SelectFormat()
{
}

bool SelectFormat::isComplete() const
{
	return m_formatList->selectedItems().count() > 0;
}

void SelectFormat::reset()
{
	m_formatList->clearSelection();
}

void SelectFormat::setFormats(KTExportInterface::Formats formats)
{
	m_formatList->clear();
	
	if ( formats & KTExportInterface::SWF )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("Macromedia flash"), m_formatList);
		
		format->setData(3124,KTExportInterface::SWF);
	}
	
	if ( formats & KTExportInterface::MPEG )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("MPEG Video"), m_formatList);
		
		format->setData(3124, KTExportInterface::MPEG);
	}
	
	if ( formats & KTExportInterface::AVI )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("AVI Video"), m_formatList);
		
		format->setData(3124, KTExportInterface::AVI);
	}
	
	if ( formats & KTExportInterface::RM )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("RealMedia Video"), m_formatList);
		
		format->setData(3124, KTExportInterface::RM);
	}
	
	if ( formats & KTExportInterface::ASF )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("ASF Video"), m_formatList);
		
		format->setData(3124, KTExportInterface::ASF);
	}
	
	if ( formats & KTExportInterface::MOV )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("QuickTime Video"), m_formatList);
		
		format->setData(3124, KTExportInterface::MOV);
	}
	
	if ( formats & KTExportInterface::GIF )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("Gif Image"), m_formatList);
		
		format->setData(3124, KTExportInterface::GIF);
	}
	
	if ( formats & KTExportInterface::PNG )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("PNG Image Array"), m_formatList);
		
		format->setData(3124, KTExportInterface::PNG);
	}
	
	if ( formats & KTExportInterface::JPEG )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("JPEG Image Array"), m_formatList);
		
		format->setData(3124, KTExportInterface::JPEG);
	}
	
	if ( formats & KTExportInterface::SMIL )
	{
		QListWidgetItem *format = new QListWidgetItem(tr("SMIL"), m_formatList);
		
		format->setData(3124, KTExportInterface::SMIL);
	}
}

void SelectFormat::selected(QListWidgetItem *item)
{
	if( item )
	{
		emit formatSelected(item->data(3124).toInt());
		emit completed();
	}
}

class SelectScenes : public DWizardPage
{
	Q_OBJECT;
	public:
		SelectScenes();
		~SelectScenes();
		
		bool isComplete() const;
		void reset();
		
		void setScenes(const QList<KTScene *> &scenes);
		
		void aboutToNextPage();
		
	private slots:
		void updateState();
		
	signals:
		void selectedScenes(const QList<int> &scenes);
		
	private:
		DItemSelector *m_selector;
};

SelectScenes::SelectScenes() : DWizardPage(tr(""))
{
	m_selector = new DItemSelector;
	
	connect(m_selector, SIGNAL(changed()), this, SLOT(updateState()));
	
	setWidget(m_selector);
}

SelectScenes::~SelectScenes()
{
}

bool SelectScenes::isComplete() const
{
	return m_selector->selectedItems().count() > 0;
}

void SelectScenes::reset()
{
}

void SelectScenes::setScenes(const QList<KTScene *> &scenes)
{
	m_selector->clear();
	
	int pos = 1;
	foreach(KTScene *scene, scenes)
	{
		m_selector->addItem(QString("%1: ").arg(pos)+scene->sceneName());
		pos++;
	}
}

void SelectScenes::aboutToNextPage()
{
	emit selectedScenes(m_selector->selectedIndexes());
}

void SelectScenes::updateState()
{
	emit completed();
}

class ExportTo : public DWizardPage
{
	Q_OBJECT;
	public:
		ExportTo(const KTProject *project);
		~ExportTo();
		
		bool isComplete() const;
		void reset();
		
		void aboutToFinish();
		
	public slots:
		void exportIt();
		
	private slots:
		void updateState(const QString & text);
		void chooseFile();
		
	private:
		QString fileToExport() const;
		QList<KTScene *> scenesToExport() const;
		
	public slots:
		void setScenesIndexes(const QList<int> &indexes);
		void setCurrentExporter(KTExportInterface *currentExporter);
		void setCurrentFormat(int currentFormat);
		
	private:
		QList<int> m_indexes;
		KTExportInterface *m_currentExporter;
		KTExportInterface::Format m_currentFormat;
		
		const KTProject *m_project;
		
		QLineEdit *m_filePath;
		QSpinBox *m_fps;
		
		DXYSpinBox *m_size;
};

ExportTo::ExportTo(const KTProject *project) : DWizardPage(tr("")), m_currentExporter(0), m_currentFormat(KTExportInterface::NONE), m_project(project)
{
	QWidget *container = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(container);
	
	
	////////////////
	QHBoxLayout *filePathLayout = new QHBoxLayout;
	filePathLayout->addWidget(new QLabel(tr("File: ")));
	
	m_filePath = new QLineEdit;
	connect(m_filePath, SIGNAL(textChanged ( const QString &)), this, SLOT(updateState(const QString &)));
	filePathLayout->addWidget(m_filePath);
	
	QToolButton *button = new QToolButton;
	button->setIcon(QIcon(THEME_DIR+"/icons/open.png"));
	connect(button, SIGNAL(clicked()), this, SLOT(chooseFile()));
	filePathLayout->addWidget(button);
	
	layout->addLayout(filePathLayout);
	/////////////////
	
	QWidget *configure = new QWidget;
	QHBoxLayout *configureLayout = new QHBoxLayout(configure);
	configureLayout->addStretch();
	
	m_size = new DXYSpinBox(tr("Size") );
	m_size->setMaximum( 1024 );
	m_size->setModifyTogether(true);
	
	m_size->setX( 520 );
	m_size->setY( 340);
	
	QGroupBox *configuration = new QGroupBox(tr("Configuration"));
	QHBoxLayout *configurationLayout = new QHBoxLayout(configuration);
	
	configurationLayout->addWidget(new QLabel(tr("FPS")));
	
	m_fps = new QSpinBox;
	m_fps->setMinimum(0);
	m_fps->setMaximum(100);
	m_fps->setValue(24);
	configurationLayout->addWidget(m_fps);
	
	configureLayout->addWidget(m_size);
	configureLayout->addWidget(configuration);
	
	configureLayout->addStretch();
	
	layout->addWidget(configure);
	
	layout->addStretch();
	
	setWidget(container);
}

ExportTo::~ExportTo()
{
}

bool ExportTo::isComplete() const
{
	return !m_filePath->text().isEmpty();
}

void ExportTo::reset()
{
}

void ExportTo::aboutToFinish()
{
	exportIt();
}

void ExportTo::setScenesIndexes(const QList<int> &indexes)
{
	m_indexes = indexes;
}

void ExportTo::setCurrentExporter(KTExportInterface *currentExporter)
{
	m_currentExporter = currentExporter;
}

void ExportTo::setCurrentFormat(int currentFormat)
{
	m_currentFormat = KTExportInterface::Format(currentFormat);
}

QString ExportTo::fileToExport() const
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

void ExportTo::updateState(const QString &)
{
	emit completed();
}

void ExportTo::chooseFile()
{
	QString fileName = QFileDialog::getSaveFileName( this, tr("Choose a file name..."));
	
	m_filePath->setText(fileName);
}

void ExportTo::exportIt()
{
	D_FUNCINFO;
	
	if( m_currentExporter && m_currentFormat != KTExportInterface::NONE )
	{
		QString file = fileToExport();
		
		if ( file.isNull() ) return;
		
		dDebug("export") << "Exporting to file: " << file;
		
		QList<KTScene *> scenes = scenesToExport();
		
		dDebug("export") << "Exporting " << scenes.count() << " scenes";
		
		if ( scenes.count() > 0)
		{
			m_currentExporter->exportToFormat( file, scenes, m_currentFormat, QSize(m_size->x(),m_size->x()), m_fps->value()  );
		}
	}
	else
	{
		DOsd::self()->display( tr("Please choose the format before export"), DOsd::Error );
	}
}

QList<KTScene *> ExportTo::scenesToExport() const
{
	QList<KTScene *> scenes;
	foreach(int index, m_indexes)
	{
		scenes << m_project->scene(index);
	}
	
	return scenes;
}


KTExportWidget::KTExportWidget(const KTProject *project, QWidget *parent) : DWizard(parent), m_project(project)
{
	DINIT;
	setWindowTitle(tr("Export"));
	setWindowIcon(QIcon(THEME_DIR+"/icons/export.png"));
	
	m_pluginSelectionPage = new SelectPlugin();
	addPage(m_pluginSelectionPage);
	
	m_formatSelectionPage = new SelectFormat();
	addPage(m_formatSelectionPage);
	
	m_scenesSelectionPage = new SelectScenes();
	m_scenesSelectionPage->setScenes(project->scenes());
	
	addPage(m_scenesSelectionPage);
	
	m_exportToPage = new ExportTo(project);
	addPage(m_exportToPage);
	
	connect(m_pluginSelectionPage, SIGNAL(selectedPlugin(const QString &)), this, SLOT(setExporter(const QString &)));
	connect(m_formatSelectionPage, SIGNAL(formatSelected(int)), m_exportToPage, SLOT(setCurrentFormat(int)));
	connect(m_scenesSelectionPage, SIGNAL(selectedScenes(const QList<int> &)), m_exportToPage, SLOT(setScenesIndexes(const QList<int> &)));
	
	loadPlugins();
}

KTExportWidget::~KTExportWidget()
{
	DEND;
	qDeleteAll(m_plugins);
}

void KTExportWidget::loadPlugins()
{
	QDir pluginDirectory = QDir(HOME_DIR+"/plugins/");

	foreach (QString fileName, pluginDirectory.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginDirectory.absoluteFilePath(fileName));
		KTExportPluginObject *plugin = qobject_cast<KTExportPluginObject*>(loader.instance());
		
		if (plugin)
		{
			KTExportInterface *exporter = qobject_cast<KTExportInterface *>(plugin);
			
			if (exporter)
			{
				m_pluginSelectionPage->addPlugin(exporter->key());
				m_plugins.insert(exporter->key(), exporter);
			}
			else
				dError() << "Can't load: " << fileName;
		}
	}
}

void KTExportWidget::setExporter(const QString &plugin)
{
	if ( m_plugins.contains(plugin) )
	{
		KTExportInterface* currentExporter = m_plugins[plugin];
		m_formatSelectionPage->setFormats(currentExporter->availableFormats());
		
		m_exportToPage->setCurrentExporter(currentExporter);
	}
}


#include "ktexportwidget.moc"

