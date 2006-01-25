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

#ifndef FFMPEGPLUGIN_H
#define FFMPEGPLUGIN_H

#include <config.h>
#include <exportinterface.h>

#ifdef HAVE_FFMPEG
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>
#endif

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class FFMpegPlugin : public KTExportPluginObject, public ExportInterface
{
	Q_OBJECT;
	Q_INTERFACES(ExportInterface);
	public:
		FFMpegPlugin();
		virtual ~FFMpegPlugin();
		virtual QString key() const;
		ExportInterface::Formats availableFormats();
		
		virtual void exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format, int fps);
		
	private:
		AVFrame *m_picture, *m_tmpPicture;
		uint8_t *videOutbuf;
		int m_frameCount, videOutbufSize;
		double m_streamDuration;
		
	private:
		bool openVideo(AVFormatContext *oc, AVStream *st);
		void closeVideo(AVFormatContext *oc, AVStream *st);
		bool writeVideoFrame(AVFormatContext *oc, AVStream *st, int fps);
		AVStream *addVideoStream(AVFormatContext *oc, int codec_id, int fps);
		void fillYuvImage(AVFrame *pict, int frame_index, int width, int height);
		AVFrame *allocPicture(int pix_fmt, int width, int height);
		
	private:
		QStringList createImages(const QList<KTScene *> &scenes, const QDir &dir);
		
};

#endif
