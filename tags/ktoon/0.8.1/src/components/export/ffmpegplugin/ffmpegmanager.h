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

#ifndef FFMPEGMANAGER_H
#define FFMPEGMANAGER_H

#include <QtGlobal>

#ifdef Q_OS_WIN32
#warning HARDBIT: crear un configure.bat para windows
#endif

#ifdef Q_OS_UNIX
#include <config.h>
#endif

#include <QString>
#include <QSize>

#ifdef HAVE_FFMPEG
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>
#endif

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class FFMpegManager
{
	public:
		FFMpegManager();
		~FFMpegManager();
		
		void create(const QString &filePath, int formatId, const QStringList &paths, const QSize &size, int fps );
#ifdef HAVE_FFMPEG
		bool openVideo(AVFormatContext *oc, AVStream *st);
		void closeVideo(AVFormatContext *oc, AVStream *st);
		bool writeVideoFrame(const QString &imagePath,AVFormatContext *oc, AVStream *st, int fps);
		AVStream *addVideoStream(AVFormatContext *oc, int codec_id, int width, int height, int fps);
		
		AVFrame *allocPicture(int pix_fmt, int width, int height);
		
	private:
		AVFrame *m_picture, *m_tmpPicture;
		uint8_t *videOutbuf;
		int m_frameCount, videOutbufSize;
		double m_streamDuration;
#endif
};

#endif
