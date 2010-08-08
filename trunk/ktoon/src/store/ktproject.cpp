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

#include "ktproject.h"
#include "ktscene.h"
#include "ktlayer.h"
#include "ktsoundlayer.h"
#include "ktframe.h"
#include "ktlibrary.h"
#include "ktlibraryobject.h"
#include "ktgraphiclibraryitem.h"
#include "ktitemfactory.h"
#include "ktprojectresponse.h"
#include "ktprojectloader.h"

#include <kcore/kdebug.h>

#include <QDir>
#include <QGraphicsView>

struct KTProject::Private
{
    QString name;
    QString author;
    QString description;

    QSize dimension;
    int fps;
    QString dataDir;
    Scenes scenes;
    int sceneCounter;
    KTLibrary *library;
    bool isOpen;
};

/**
 * Constructor
 */
KTProject::KTProject(QObject *parent) : QObject(parent), k(new Private)
{
    #ifdef K_DEBUG
           KINIT;
    #endif

    k->sceneCounter = 0;
    k->isOpen = false;
    k->library = new KTLibrary("library", this);
}

/**
 * Destructor
 */
KTProject::~KTProject()
{
    #ifdef K_DEBUG
           KEND;
    #endif

    deleteDataDir();
    k->scenes.clear(true);
    delete k;
}

/**
 * This function loads a local library
 */
void KTProject::loadLibrary(const QString &filename)
{
    QFile lfile(filename);

    if (lfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        k->library->fromXml(QString::fromLocal8Bit(lfile.readAll()));
        lfile.close();
    } else {
        #ifdef K_DEBUG
               kFatal("library") << "Cannot open library from: " << filename;
        #endif
    }
}

/**
 * This function closes project 
 */
void KTProject::clear()
{
    k->scenes.clear(true);
    k->sceneCounter = 0;

    deleteDataDir();
}

/**
 * This function sets project name 
 */
void KTProject::setProjectName(const QString &name)
{
    k->name = name;
}

/**
 * This function sets author name 
 */
void KTProject::setAuthor(const QString &author)
{
    k->author = author;
}

/**
 * This function sets project description
 */
void KTProject::setDescription(const QString& description)
{
    k->description = description;
}

/**
 * This function sets project dimension
 */
void KTProject::setDimension(const QSize dimension)
{
    k->dimension = dimension;
}

/**
 * This function sets FPS dimension
 */
void KTProject::setFPS(const int fps)
{
    k->fps = fps;
}

/**
 * Returns project name
 */
QString KTProject::projectName() const
{
    return k->name;
}

/**
 * Returns project name
 */
QString KTProject::author() const
{
    return k->author;
}

/**
 * Returns description project
 */
QString KTProject::description() const
{
    return k->description;
}

/**
 * Returns dimension project
 */
QSize KTProject::dimension() const
{
    return k->dimension;
}

/**
 * Returns fps project
 */
int KTProject::fps() const
{
    return k->fps;
}

KTScene *KTProject::createScene(int position, bool loaded)
{
    if (position < 0 || position > k->scenes.count())
        return 0;

    KTScene *scene = new KTScene(this);
    k->scenes.insert(position, scene);
    k->sceneCounter++;

    scene->setSceneName(tr("Scene %1").arg(k->sceneCounter));
    
    if (loaded)
        KTProjectLoader::createScene(position, scene->sceneName(), this);

    return scene;
}

void KTProject::updateScene(int position, KTScene *scene)
{
    k->scenes.insert(position, scene);
}

bool KTProject::removeScene(int position)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif
    KTScene *toRemove = scene(position);

    if (toRemove) {
        k->scenes.removeObject(position);
        delete toRemove;
        toRemove = 0;
        k->sceneCounter--;

        return true;
    }

    return false;
}


bool KTProject::moveScene(int position, int newPosition)
{
    if (position < 0 || position >= k->scenes.count() || newPosition < 0 || newPosition >= k->scenes.count()) {
        #ifdef K_DEBUG
               kWarning() << "Failed moving scene!";
        #endif
        return false;
    }

    k->scenes.copyObject(position, newPosition);

    return true;
}

KTScene *KTProject::scene(int position) const
{
    #ifdef K_DEBUG
           K_FUNCINFOX("project")<< position;
    #endif

    if (position < 0 || position >= k->scenes.count()) {
        #ifdef K_DEBUG
               K_FUNCINFO << " FATAL ERROR: index out of bound " << position;
        #endif
        return 0;
    }

    return k->scenes.value(position);
}

int KTProject::visualIndexOf(KTScene *scene) const
{
    return k->scenes.objectIndex(scene);
}

/*
int KTProject::logicalIndexOf(KTScene *scene) const
{
    return k->scenes.logicalIndex(scene);
}
*/

void KTProject::fromXml(const QString &xml)
{
    QDomDocument document;

    if (!document.setContent(xml))
        return;

    QDomElement root = document.documentElement();
    QDomNode n = root.firstChild();

    int i = 0;
    while (!n.isNull()) {
           QDomElement e = n.toElement();

           if (!e.isNull()) {
               if (e.tagName() == "project") {
                   setProjectName(e.attribute("name", projectName()));
                   QDomNode n1 = e.firstChild();
                   e = n1.toElement();

                   if (e.tagName() == "meta") {

                       QDomNode n1 = e.firstChild();

                       while (!n1.isNull()) {

                              QDomElement e1 = n1.toElement();

                              if (e1.tagName() == "author") {

                                  if (e1.firstChild().isText()) 
                                      setAuthor(e1.text());

                              } else if (e1.tagName() == "description") {

                                         if (e1.firstChild().isText())
                                             setDescription(e1.text());

                                } else if (e1.tagName() == "dimension") {

                                           if (e1.firstChild().isText()) {
                                               QStringList list = e1.text().split(",");
                                               int x = list.at(0).toInt();
                                               int y = list.at(1).toInt();
                                               QSize size(x,y);
                                               setDimension(size);
                                           }

                                } else if (e1.tagName() == "fps") {
                                           if (e1.firstChild().isText())
                                               setFPS(e1.text().toInt());

                                }
                                n1 = n1.nextSibling();
                          }
                   }
               }
           } 
           n = n.nextSibling();
           i++;
    }
}

QDomElement KTProject::toXml(QDomDocument &doc) const
{
    QDomElement ktoon = doc.createElement("KToon");
    ktoon.setAttribute("version", "1");

    QDomElement project = doc.createElement("project");
    project.setAttribute("name", k->name);

    QDomElement meta = doc.createElement("meta");

    QDomElement author = doc.createElement("author");
    author.appendChild(doc.createTextNode(k->author));

    QDomElement description = doc.createElement("description");
    description.appendChild(doc.createTextNode(k->description));

    QDomElement size = doc.createElement("dimension");
    QString xy = QString::number(k->dimension.width()) + "," + QString::number(k->dimension.height());
    size.appendChild(doc.createTextNode(xy));

    QDomElement fps = doc.createElement("fps");
    QString frames = QString::number(k->fps);
    fps.appendChild(doc.createTextNode(frames));

    meta.appendChild(author);
    meta.appendChild(description);
    meta.appendChild(size);
    meta.appendChild(fps);

    project.appendChild(meta);
    ktoon.appendChild(project);
    
    return ktoon;
}

Scenes KTProject::scenes() const
{
    return k->scenes;
}

bool KTProject::createSymbol(int type, const QString &name, const QByteArray &data)
{
    if (!k->isOpen) 
        return false;

    return k->library->createSymbol(KTLibraryObject::Type(type), name, data) != 0;
}

bool KTProject::removeSymbol(const QString &name)
{
    return k->library->removeObject(name);
}

bool KTProject::addSymbolToProject(const QString &name, int sceneIndex, int layerIndex, int frameIndex)
{
    KTLibraryObject *object = k->library->findObject(name);
    KTFrame *frame = 0;

    KTScene *scene = this->scene(sceneIndex);

    if (scene) {
        KTLayer *layer = scene->layer(layerIndex);
        if (layer)
            frame = layer->frame(frameIndex);
    }

    if (object && frame) {
        switch (object->type()) {
                case KTLibraryObject::Image:
                     {
                       KTGraphicLibraryItem *libraryItem = new KTGraphicLibraryItem(object);
                       libraryItem->moveBy(100, 100);
                       QString id(libraryItem->symbolName());
                       frame->addItem(id, libraryItem);
                     }
                break;
                case KTLibraryObject::Text:
                     {
                       KTGraphicLibraryItem *libraryItem = new KTGraphicLibraryItem(object);
                       QString id(libraryItem->symbolName());
                       frame->addItem(id, libraryItem);
                     }
                break;
                case KTLibraryObject::Svg:
                     {
                       KTGraphicLibraryItem *libraryItem = new KTGraphicLibraryItem(object);
                       QString id(libraryItem->symbolName());
                       frame->addItem(id, libraryItem);
                     }
                break;
                case KTLibraryObject::Item:
                     {
                       KTGraphicLibraryItem *libraryItem = new KTGraphicLibraryItem(object);
                       QString id(libraryItem->symbolName());
                       frame->addItem(id, libraryItem);
                     }
                break;
                case KTLibraryObject::Sound:
                     {
                       KTSoundLayer *sound = scene->createSoundLayer(scene->soundLayers().count());
                       sound->fromSymbol(object->symbolName());
                     }
                break;
                default:
                     #ifdef K_DEBUG
                       kFatal() << "KTProject::addSymbolToProject() -> Unknown Object Type"; 
                     #endif
                break;
        }

        return true;
    } 

    return false;
}

bool KTProject::removeSymbolFromProject(const QString &name, int scene, int layer, int frame)
{
     #ifdef K_DEBUG
            kFatal("project") << "removeSymbolFromProject::Find me in ktproject.cpp";
            kDebug() << "Name: " << name;
            kDebug() << "Scene: " << scene; 
            kDebug() << "Layer: " << layer;
            kDebug() << "Frame: " << frame; 
     #endif

     return false;
}

KTLibrary *KTProject::library() const
{
    return k->library;
}

void KTProject::emitResponse(KTProjectResponse *response)
{
    emit responsed(response);
}

void KTProject::setOpen(bool open)
{
    k->isOpen = open;
}

bool KTProject::isOpen()
{
    return k->isOpen;
}

bool KTProject::deleteDataDir()
{
    if (QFile::exists(dataDir() ) && !k->name.isEmpty()) {
        QDir dir(dataDir() );

        if ((dir.exists("audio") && dir.exists("video") && dir.exists("images")) || dir.exists("project.ktp")) {
            #ifdef K_DEBUG
                   kDebug("project") << "Removing " << dir.absolutePath() << "...";
            #endif

            dir.remove("project.ktp");
            dir.remove("library.ktl");

            foreach (QString scene, dir.entryList(QStringList() << "scene*.kts", QDir::Files ))
                     dir.remove(scene);

            foreach (QString subdir, QStringList() << "audio" << "video" << "images") {
                     if (dir.exists(subdir)) {
                         dir.cd(subdir);
                         foreach (QString file, dir.entryList()) {
                                  QString absolute = dir.absolutePath() + "/" + file;

                                  if (!file.startsWith(".")) {
                                      QFileInfo finfo(absolute);
                                      if (finfo.isFile())
                                          QFile::remove(absolute);
                                  }
                          }
                          dir.cdUp();
                          dir.rmdir(subdir);
                     }
            }

            if (! dir.rmdir(dir.absolutePath())) {
                #ifdef K_DEBUG
                       kError("project") << "Cannot remove project data directory!";
                #endif
            }
        }

        return true;
    }

    return false;
}

QString KTProject::dataDir() const
{
    return CACHE_DIR + "/" + k->name;
}

int KTProject::scenesTotal() const
{
    return k->sceneCounter;
}
