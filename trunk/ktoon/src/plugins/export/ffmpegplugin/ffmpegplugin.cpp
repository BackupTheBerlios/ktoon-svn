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
    return KTExportInterface::OGV | KTExportInterface::MPEG | KTExportInterface::SWF | KTExportInterface::AVI | KTExportInterface::RM | KTExportInterface::ASF | KTExportInterface::MOV | KTExportInterface::GIF;
}

bool FFMpegPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, KTExportInterface::Format format, const QSize &size, int fps)
{
    KFFMpegMovieGenerator *generator = 0;

    switch (format) {
            case KTExportInterface::OGV:
                 {
                   generator = new KFFMpegMovieGenerator(KFFMpegMovieGenerator::OGV, size, fps);
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

