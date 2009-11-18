/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *    
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
 
#include "genericexportplugin.h"

#include <kcore/kglobal.h>
#include <kcore/kdebug.h>

#include <QImage>
#include <QPainter>

#include "ktlayer.h"
#include "ktanimationrenderer.h"

GenericExportPlugin::GenericExportPlugin()
{
}

GenericExportPlugin::~GenericExportPlugin()
{
}

QString GenericExportPlugin::key() const
{
    return "Image Arrays";
}

KTExportInterface::Formats GenericExportPlugin::availableFormats()
{
    return KTExportInterface::PNG | KTExportInterface::JPEG | KTExportInterface::XPM;
}

void GenericExportPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, KTExportInterface::Format format, const QSize &size, int fps)
{
    QFileInfo fileInfo(filePath);

    QDir dir = fileInfo.dir();
    if (!dir.exists())
        dir.mkdir(dir.path());

    m_baseName = fileInfo.baseName();
    const char *fmt = "PNG";

    switch (format) {
            case KTExportInterface::JPEG:
                 fmt = "JPEG";
                 break;
            case KTExportInterface::XPM:
                 fmt = "XPM";
                 break;
            default:
                 break;
    }

    KTAnimationRenderer renderer;

    foreach (KTScene *scene, scenes) {
             renderer.setScene(scene);

             int photogram = 0;
             while (renderer.nextPhotogram()) {
                    QImage img(size, QImage::Format_RGB32);
                    {
                     QPainter painter(&img);
                     painter.setRenderHint(QPainter::Antialiasing, true);
                     renderer.render(&painter);
                    }
                    img.save(fileInfo.absolutePath() +"/"+ QString(m_baseName+"%1.%2").arg(photogram).arg(QString(fmt).toLower() ), fmt);

                    photogram++;
             }
    }
}

Q_EXPORT_PLUGIN( GenericExportPlugin );

