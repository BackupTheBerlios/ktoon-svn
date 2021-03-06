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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
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

#include "ktlibrarywidget.h"

#include <kcore/kglobal.h>
#include <kcore/kdebug.h>
#include <kgui/koptionaldialog.h>
#include <kcore/kconfig.h>

#include <QApplication>
#include <QGroupBox>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QLabel>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDesktopWidget>
#include <QBuffer>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QComboBox>

#include <ktrequestbuilder.h>

#include <cstdlib>
#include <ctime>
#include <QTreeWidgetItemIterator>

#include "ktlibrary.h"
#include "ktlibraryobject.h"
#include "ktsymboleditor.h"

#include <kgui/kosd.h>
#include <ksound/kaudioplayer.h>

#define RETURN_IF_NOT_LIBRARY if (!k->library) return;

/**
 * This class defines the Library widget.
 * Here is where events around the Library widget are handled.
 * @author David Cuadrado <krawek@toonka.com>
*/

struct KTLibraryWidget::Private
{
    Private() : library(0)
    {
       currentFrame.frame = 0;
       currentFrame.layer = 0;
       currentFrame.scene = 0;
    }

    const KTLibrary *library;
    KTItemPreview *display;
    KTGCTable *libraryTree;
    int childCount;
    QDir libraryDir;
    QComboBox *itemType;

    int currentPlayerId;

    struct Frame
    {
       int scene;
       int layer;
       int frame;
    } currentFrame;
};

KTLibraryWidget::KTLibraryWidget(QWidget *parent) : KTModuleWidgetBase(parent), k(new Private)
{
    #ifdef K_DEBUG
           KINIT;
    #endif
    k->childCount = 0;

    setWindowIcon(QPixmap(THEME_DIR + "icons/library.png"));
    setWindowTitle(tr("Library"));

    k->libraryDir = QDir(CONFIG_DIR + "libraries");
    k->display = new KTItemPreview(this);
    k->libraryTree = new KTGCTable(this);

    connect(k->libraryTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, 
                                   SLOT(previewItem(QTreeWidgetItem *, int)));
    connect(k->libraryTree, SIGNAL(itemRenamed(QTreeWidgetItem*)), this, SLOT(renameObject(QTreeWidgetItem*)));

    QGroupBox *buttons = new QGroupBox(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttons);
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(0);

    k->itemType = new QComboBox();
    k->itemType->setIconSize(QSize(15, 15));
    k->itemType->setMaximumWidth(120);
    
    k->itemType->addItem(QIcon(THEME_DIR + "icons/bitmap.png"), tr("Image"));
    k->itemType->addItem(QIcon(THEME_DIR + "icons/bitmap_array.png"), tr("Image Array"));
    k->itemType->addItem(QIcon(THEME_DIR + "icons/svg.png"), tr("Svg File"));
    k->itemType->addItem(QIcon(THEME_DIR + "icons/svg_array.png"), tr("Svg Array"));

    buttonLayout->addWidget(k->itemType);

    KImageButton *addGC = new KImageButton(QPixmap(THEME_DIR + "icons/plus_sign.png"), 22, buttons);
    connect(addGC, SIGNAL(clicked()), this, SIGNAL(requestCurrentGraphic()));
    // SQA code
    // addGC->setEnabled(false);

    buttonLayout->addWidget(addGC);
    addGC->setToolTip(tr("Add the current graphic to library"));

    /*
    KImageButton *delGC = new KImageButton(QPixmap(THEME_DIR + "icons/minus_sign.png"), 22, buttons);
    connect(delGC, SIGNAL(clicked()), this, SLOT(removeCurrentGraphic()));
    // SQA code
    //delGC->setEnabled(false);

    delGC->setToolTip(tr("Remove the selected symbol from library"));
    buttonLayout->addWidget(delGC);
    */

    KImageButton *gctoDrawingArea = new KImageButton(QPixmap(THEME_DIR + "icons/library_to_ws.png"), 22, buttons);
    connect(gctoDrawingArea, SIGNAL(clicked()), this, SLOT(emitSelectedComponent()));
    gctoDrawingArea->setToolTip(tr("Inserts the selected symbol into the drawing area"));
    buttonLayout->addWidget(gctoDrawingArea);

    KImageButton *addFolderGC = new KImageButton(QPixmap(THEME_DIR + "icons/create_folder.png"), 22, buttons);
    //addFolderGC->setEnabled(false);
    connect(addFolderGC, SIGNAL(clicked()), k->libraryTree, SLOT(createFolder()));
    addFolderGC->setToolTip(tr("Adds a folder to the symbol list"));
    buttonLayout->addWidget(addFolderGC);

    buttons->setLayout(buttonLayout);

    addChild(k->display);
    addChild(buttons);
    addChild(k->libraryTree);
}


KTLibraryWidget::~KTLibraryWidget()
{
    #ifdef K_DEBUG
           KEND;
    #endif
    delete k;
}

void KTLibraryWidget::setLibrary(const KTLibrary *library)
{
    k->library = library;
}

void KTLibraryWidget::addFolder(const QString &name)
{
    k->libraryTree->createFolder(name);
}

void KTLibraryWidget::previewItem(QTreeWidgetItem *item, int)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    RETURN_IF_NOT_LIBRARY;

    if (item) {

        KTLibraryObject *object = k->library->findObject(item->text(1));

        if (!object) {
            #ifdef K_DEBUG
                   kDebug("library") << "Cannot find the object";
                   kFatal() << "KTLibraryWidget::previewItem - Cannot find the object";
            #endif
            return;
        }

        switch (object->type()) {
                case KTLibraryObject::Svg:
                   {
                     QString svgContent = qvariant_cast<QString>(object->data());
                     if (svgContent.length() > 0) {
                         QByteArray stream = svgContent.toLocal8Bit();
                         QGraphicsSvgItem *svg = new QGraphicsSvgItem;
                         svg->renderer()->load(stream);
                         k->display->render(static_cast<QGraphicsItem *>(svg));
                     } 
                   }
                   break;
                case KTLibraryObject::Image:
                case KTLibraryObject::Item:
                   {
                     if (object->data().canConvert<QGraphicsItem *>()) {
                         k->display->render(qvariant_cast<QGraphicsItem *>(object->data()));
                         /* SQA: Just a test
                         KTSymbolEditor *editor = new KTSymbolEditor;
                         editor->setSymbol(object);
                         emit postPage(editor);
                         */    
                     } 
                   }
                   break;
                case KTLibraryObject::Sound:
                   {
                     KAudioPlayer::instance()->setCurrentPlayer(k->currentPlayerId);
                     KAudioPlayer::instance()->stop();

                     k->currentPlayerId = KAudioPlayer::instance()->load(object->dataPath());
                     KAudioPlayer::instance()->play(0);
                   }
                   break;
                default:
                   {
                     #ifdef K_DEBUG
                            kDebug("library") << "Unknown symbol id: " << object->type();
                     #endif
                   }
                   break;
        }
    }
}

void KTLibraryWidget::emitSelectedComponent()
{
    if (!k->libraryTree->currentItem())
        return;

    QString symKey = k->libraryTree->currentItem()->text(1);

    KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::AddSymbolToProject, symKey,
                               KTLibraryObject::Type(k->libraryTree->currentItem()->data(1, 3216).toInt()), 0, 
                               k->currentFrame.scene, k->currentFrame.layer, k->currentFrame.frame);

    emit requestTriggered(&request);
}

void KTLibraryWidget::removeCurrentGraphic()
{
    if (!k->libraryTree->currentItem()) 
        return;

    QString symKey = k->libraryTree->currentItem()->text(1);

    KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Remove, symKey,
                               KTLibraryObject::Type(k->libraryTree->currentItem()->data(0, 3216).toInt()), 0);
    
    emit requestTriggered(&request);
}

void KTLibraryWidget::renameObject(QTreeWidgetItem* item)
{
    Q_UNUSED(item);

    // SQA: Check this code
    /*
    if ( item ) {
         KTGraphicComponent *graphic = k->graphics[item];

         if (graphic) {
             graphic->setComponentName(item->text(1));
         } else { // A Folder
             foreach (QTreeWidgetItem *folder, k->libraryTree->topLevelItems()) {
                      if (folder != item && folder->text(1) == item->text(1)) {
                          // Invalid name
                          item->setFlags(item->flags() | Qt::ItemIsEditable );
                          item->setText(0, item->text(1)+QString::number(() % 999) );
                          k->libraryTree->editItem(item, 0);
                          break;
                      }
             }
         }
    }
    */
}

void KTLibraryWidget::importBitmap()
{
    QString image = QFileDialog::getOpenFileName (this, tr("Import an image..."), QDir::homePath(),  
                                                  tr("Images") + " (*.png *.xpm *.jpg *.gif)");
    if (image.isEmpty()) 
        return;

    QFile f(image);
    QFileInfo fileInfo(f);

    QString symName = fileInfo.fileName();

    if (f.open(QIODevice::ReadOnly)) {
        QByteArray data = f.readAll();
        f.close();

        KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName,
                                                                          KTLibraryObject::Image, data, k->currentFrame.scene, 
                                                                          k->currentFrame.layer, k->currentFrame.frame);
        emit requestTriggered(&request);
    } else {
        KOsd::self()->display(tr("Error"), tr("Cannot open file: %1").arg(image), KOsd::Error);
    }
}

void KTLibraryWidget::importSvg()
{
    QString svg = QFileDialog::getOpenFileName (this, tr("Import a SVG file..."), QDir::homePath(),
                                                  tr("Vectorial") + " (*.svg)");
    if (svg.isEmpty())
        return;

    QFile f(svg);
    QFileInfo fileInfo(f);

    QString symName = fileInfo.fileName();

    if (f.open(QIODevice::ReadOnly)) {
        QByteArray data = f.readAll();
        f.close();

        KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName,
                                                     KTLibraryObject::Svg, data, k->currentFrame.scene, 
                                                     k->currentFrame.layer, k->currentFrame.frame);
        emit requestTriggered(&request);
    } else {
        KOsd::self()->display(tr("Error"), tr("Cannot open file: %1").arg(svg), KOsd::Error);
    }
}

void KTLibraryWidget::importBitmapArray()
{
    QDesktopWidget desktop;
    QString dir = getenv("HOME");
    QString path = QFileDialog::getExistingDirectory(this, tr("Choose the images directory..."), dir,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
        return;

    QDir source(path); 
    QFileInfoList photograms = source.entryInfoList(QDir::Files, QDir::Name);
    int size = photograms.size();

    if (size > 0) {

        QString testFile = photograms.at(0).absoluteFilePath();
        QFile file(testFile);
        int kb = int(file.size()/1024);
        file.close();

        QString text = tr("%1 images will be loaded.").arg(size);
        bool resize = false; 
        if (kb > 200) {
            text = text + "\n" + tr("Files are too big, so they will be resized.") + "\n" 
                   + tr("Note: This task can take a while.");
            resize = true;
        }

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Information"));  
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(text);
        msgBox.setInformativeText(tr("Do you want to continue?"));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.show();
        msgBox.move((int) (desktop.screenGeometry().width() - msgBox.width())/2, 
                    (int) (desktop.screenGeometry().height() - msgBox.height())/2);

        int answer = msgBox.exec();

        if (answer == QMessageBox::Ok) {

            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

            QFont font = this->font();
            font.setPointSize(8);

            QProgressDialog progressDialog(this, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
            progressDialog.setFont(font);
            progressDialog.setLabelText(tr("Loading images..."));
            progressDialog.setCancelButton(0);
            progressDialog.setRange(1, size);
            progressDialog.show();
            int index = 1;

            progressDialog.move((int) (desktop.screenGeometry().width() - progressDialog.width())/2 , (int) (desktop.screenGeometry().height() - progressDialog.height())/2);

            for (int i = 0; i < size; ++i) {
                 QString path = photograms.at(i).absoluteFilePath(); 
                 QString symName = photograms.at(i).fileName();
                 QFile f(path);

                 if (f.open(QIODevice::ReadOnly)) {

                     QByteArray data = f.readAll();
                     f.close();

                     if (resize) {
                         QPixmap *pixmap = new QPixmap();

                         if (pixmap->loadFromData(data, "JPG")) {
                             int width = 300;
                             QPixmap newpix(pixmap->scaledToWidth(width, Qt::SmoothTransformation));
                             QBuffer buffer(&data);
                             buffer.open(QIODevice::WriteOnly);
                             newpix.save(&buffer, "JPG");
                         }
                     }

                     KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName,
                                                                                       KTLibraryObject::Image, data);
                     emit requestTriggered(&request);

                     if (i < photograms.size()-1) {

                         KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->currentFrame.scene, k->currentFrame.layer, 
                                                                                         k->currentFrame.frame + 1, KTProjectRequest::Add, QString());
                         emit requestTriggered(&request);

                         request = KTRequestBuilder::createFrameRequest(k->currentFrame.scene, k->currentFrame.layer, k->currentFrame.frame + 1, 
                                                            KTProjectRequest::Select);
                         emit requestTriggered(&request);
                     }

                     progressDialog.setLabelText(tr("Loading image #%1").arg(index));
                     progressDialog.setValue(index);
                     index++;

                 } else {
                     kFatal() << "ERROR: Can't open file " << symName;
                     QMessageBox::critical(this, tr("ERROR!"), tr("ERROR: Can't open file %1. Please, check file permissions and try again.").arg(symName), QMessageBox::Ok);
                     QApplication::restoreOverrideCursor();
                     return;
                 }
             }

             QApplication::restoreOverrideCursor();

        }
    } else {
        KOsd::self()->display(tr("Error"), tr("No image files were found.<br/>Please, try another directory"), KOsd::Error);
    }
}

void KTLibraryWidget::importSvgArray() 
{
    QDesktopWidget desktop;
    QString dir = getenv("HOME");
    QString path = QFileDialog::getExistingDirectory(this, tr("Choose the SVG files directory..."), dir,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
        return;

    QDir source(path); 
    QFileInfoList photograms = source.entryInfoList(QDir::Files, QDir::Name);
    int size = photograms.size();

    if (size > 0) {

        QString testFile = photograms.at(0).absoluteFilePath();
        QFile file(testFile);
        file.close();

        QString text = tr("%1 SVG files will be loaded.").arg(size);

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Information"));  
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(text);
        msgBox.setInformativeText(tr("Do you want to continue?"));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.show();
        msgBox.move((int) (desktop.screenGeometry().width() - msgBox.width())/2, 
                    (int) (desktop.screenGeometry().height() - msgBox.height())/2);

        int answer = msgBox.exec();

        if (answer == QMessageBox::Ok) {

            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

            QFont font = this->font();
            font.setPointSize(8);

            QProgressDialog progressDialog(this, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
            progressDialog.setFont(font);
            progressDialog.setLabelText(tr("Loading SVG files..."));
            progressDialog.setCancelButton(0);
            progressDialog.setRange(1, size);
            progressDialog.show();
            int index = 1;

            progressDialog.move((int) (desktop.screenGeometry().width() - progressDialog.width())/2 , 
                                (int) (desktop.screenGeometry().height() - progressDialog.height())/2);

            for (int i = 0; i < size; ++i) {
                 QString path = photograms.at(i).absoluteFilePath(); 
                 QString symName = photograms.at(i).fileName();
                 QFile f(path);

                 if (f.open(QIODevice::ReadOnly)) {

                     QByteArray data = f.readAll();
                     f.close();

                     KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName,
                                                                                       KTLibraryObject::Svg, data);
                     emit requestTriggered(&request);

                     if (i < photograms.size()-1) {

                         KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->currentFrame.scene, k->currentFrame.layer, 
                                                                                         k->currentFrame.frame + 1, KTProjectRequest::Add, QString());
                         emit requestTriggered(&request);

                         request = KTRequestBuilder::createFrameRequest(k->currentFrame.scene, k->currentFrame.layer, k->currentFrame.frame + 1, 
                                                            KTProjectRequest::Select);
                         emit requestTriggered(&request);
                     }

                     progressDialog.setLabelText(tr("Loading SVG file #%1").arg(index));
                     progressDialog.setValue(index);
                     index++;

                 } else {
                     kFatal() << "ERROR: Can't open file " << symName;
                     QMessageBox::critical(this, tr("ERROR!"), tr("ERROR: Can't open file %1. Please, check file permissions and try again.").arg(symName), QMessageBox::Ok);
                     QApplication::restoreOverrideCursor();
                     return;
                 }
             }

             QApplication::restoreOverrideCursor();

        }
    } else {
        KOsd::self()->display(tr("Error"), tr("No SVG files were found.<br/>Please, try another directory"), KOsd::Error);
    }
}

void KTLibraryWidget::importSound()
{
    QString sound = QFileDialog::getOpenFileName(this, tr("Import audio file..."), QDir::homePath(),
                                                 tr("Sound file") + " (*.ogg *.wav *.mp3)");

    if (sound.isEmpty()) 
        return;

    QFile f(sound);
    QFileInfo fileInfo(f);
    QString symName = fileInfo.baseName();

    if (f.open(QIODevice::ReadOnly)) {
        QByteArray data = f.readAll();
        f.close();

        KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName,
                                                     KTLibraryObject::Sound, data);
        emit requestTriggered(&request);
    } else {
        KOsd::self()->display(tr("Error"), tr("Cannot open file: %1").arg(sound), KOsd::Error);
    }
}


void KTLibraryWidget::libraryResponse(KTLibraryResponse *response)
{
    RETURN_IF_NOT_LIBRARY;

    switch (response->action()) {
            case KTProjectRequest::Add:
              {
                 QString key = response->arg().toString();
                 KTLibraryObject *obj = k->library->findObject(key);

                 QTreeWidgetItem *item = new QTreeWidgetItem(k->libraryTree);
                 item->setText(1, key);
                 item->setData(1, 3216, obj->type());

                 if (obj) {
                     switch (obj->type()) {
                            case KTLibraryObject::Item:
                               {
                                 item->setIcon(0, QIcon(THEME_DIR + "icons/drawing_object.png"));
                                 k->libraryTree->setCurrentItem(item);
                                 previewItem(item, 1);
                               }
                            break;
                            case KTLibraryObject::Image:
                               {
                                 item->setIcon(0, QIcon(THEME_DIR + "icons/bitmap.png"));
                                 k->libraryTree->setCurrentItem(item);
                                 previewItem(item, 1);
                                 emitSelectedComponent();
                               }
                            break;
                            case KTLibraryObject::Svg:
                               {
                                 item->setIcon(0, QIcon(THEME_DIR + "icons/svg.png"));
                                 k->libraryTree->setCurrentItem(item);
                                 previewItem(item, 1);
                                 emitSelectedComponent();
                               }
                            break;
                            case KTLibraryObject::Sound:
                               {
                                 item->setIcon(0, QIcon(THEME_DIR + "icons/sound_object.png"));
                               }
                            break;
                     }
                 }
              }
            break;

            case KTProjectRequest::Remove: {
                 QString key = response->arg().toString();

                 QTreeWidgetItemIterator it(k->libraryTree);
                 while ((*it)) {
                        if (key == (*it)->text(1)) {
                            delete (*it);
                            break;
                        } else {
                            kFatal() << "KTLibraryWidget::libraryResponse -> items NO equals";
                        }
                        ++it;
                 }

                 QList<QTreeWidgetItem *> selectedGroup = k->libraryTree->selectedItems();
                 if (!selectedGroup.isEmpty()) {
                     //QTreeWidgetItem *pop = selectedGroup[0];
                     if (selectedGroup[0]->text(1).size() > 0) {
                         k->display->render(qvariant_cast<QGraphicsItem *>(k->library->findObject(
                                            selectedGroup[0]->text(1))->data()));
                     } else {
                         kDebug() << "*** KTLibraryWidget::libraryResponse -> Remove action FAILED!";
                     }
                 } else {
                     k->display->render(0);
                     kDebug() << "*** KTLibraryWidget::libraryResponse -> No library item was deleted";
                 }
              }
            break;

            case KTProjectRequest::AddSymbolToProject:
                 #ifdef K_DEBUG
                        kFatal() << "*** KTLibraryWidget::libraryResponse -> AddSymbolToProject : No action taken";
                 #endif
            break;
  
            default:
              {
                 kFatal() << "*** Project Code: " << response->action();
                 qWarning("ktlibrarywidget.cpp IMPLEMENT ME");
              }
            break;
    }
}

void KTLibraryWidget::frameResponse(KTFrameResponse *response)
{
    if (response->action() == KTProjectRequest::Select) {
        k->currentFrame.frame = response->frameIndex();
        k->currentFrame.layer = response->layerIndex();
        k->currentFrame.scene = response->sceneIndex();
    }
}
