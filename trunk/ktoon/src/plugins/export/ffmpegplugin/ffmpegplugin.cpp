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

#include "ffmpegplugin.h"

#include <kcore/kdebug.h>
#include <kcore/kglobal.h>

#include <kgui/kffmpegmoviegenerator.h>

#include <QImage>
#include <QPainter>

#include "ktlayer.h"
#include "ktanimationrenderer.h"

FFMpegPlugin::FFMpegPlugin()
{
}

FFMpegPlugin::~FFMpegPlugin()
{
}

QString FFMpegPlugin::key() const
{
    return "Video Formats";
}

KTExportInterface::Formats FFMpegPlugin::availableFormats()
{
    return KTExportInterface::OGG | KTExportInterface::MPEG | KTExportInterface::SWF | KTExportInterface::AVI | KTExportInterface::RM | KTExportInterface::ASF | KTExportInterface::MOV | KTExportInterface::GIF;
}

bool FFMpegPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, KTExportInterface::Format format, const QSize &size, int fps)
{
    KFFMpegMovieGenerator *generator = 0;

    switch (format) {
            case KTExportInterface::OGG:
                 {
                   generator = new KFFMpegMovieGenerator(KFFMpegMovieGenerator::OGG, size, fps);
                 }
                 break;
            case KTExportInterface::SWF:
                 {
                   generator = new KFFMpegMovieGenerator(KFFMpegMovieGenerator::SWF, size, fps);
                 }
                 break;
            case KTExportInterface::MPEG:
                 {
                   generator = new KFFMpegMovieGenerator(KFFMpegMovieGenerator::MPEG, size, fps);
                 }
                 break;
            case KTExportInterface::AVI:
                 {
                   generator = new KFFMpegMovieGenerator(KFFMpegMovieGenerator::AVI, size, fps);
                 }
                 break;
            case KTExportInterface::RM:
                 {
                   generator = new KFFMpegMovieGenerator(KFFMpegMovieGenerator::RM, size, fps);
                 }
                 break;
            case KTExportInterface::MOV:
                 {
                   generator = new KFFMpegMovieGenerator(KFFMpegMovieGenerator::MOV, size, fps);
                 }
                 break;
            case KTExportInterface::ASF:
                 {
                   generator = new KFFMpegMovieGenerator(KFFMpegMovieGenerator::ASF, size, fps);
                 }
                 break;
            case KTExportInterface::GIF:
                 {
                   generator = new KFFMpegMovieGenerator(KFFMpegMovieGenerator::GIF, size, fps);
                 }
                 break;
            default: 
                 return false;
    }

    KTAnimationRenderer renderer;
    {
         if (!generator->movieHeaderOk()) {
             errorMsg = generator->getErrorMsg();
             #ifdef K_DEBUG
                    kError() << "FATAL : can not create video";
             #endif
             delete generator;
             return false;
         }

         QPainter painter(generator);
         painter.setRenderHint(QPainter::Antialiasing, true);

         foreach (KTScene *scene, scenes) {
                  renderer.setScene(scene);

                  while (renderer.nextPhotogram()) {
                         renderer.render(&painter);
                         generator->nextFrame();
                         generator->reset();
                  }
         }
    }

    generator->saveMovie(filePath);
    delete generator;

    return true;
}

const char* FFMpegPlugin::getExceptionMsg() {
    return errorMsg;
}

#ifdef HAVE_FFMPEG
Q_EXPORT_PLUGIN( FFMpegPlugin );
#endif

