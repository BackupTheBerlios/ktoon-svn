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

#include "ffmpegplugin.h"

#include "ktdebug.h"
#include "ktapplication.h"

#include <QImage>
#include <QPainter>

#include <cstdio>

FFMpegPlugin::FFMpegPlugin() : m_frameCount(0), m_streamDuration(10.0)
{
#ifdef HAVE_FFMPEG
	av_register_all();
#endif
}


FFMpegPlugin::~FFMpegPlugin()
{
}

QString FFMpegPlugin::key() const
{
	return "ffmpeg";
}

ExportInterface::Formats FFMpegPlugin::availableFormats()
{
	return SWF | MPEG | AVI | RM | ASF | MOV;
}

void FFMpegPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format, int fps)
{
#ifdef HAVE_FFMPEG
	AVOutputFormat *fmt = guess_format(0, filePath.toLatin1().data(), 0);
	AVFormatContext *oc = av_alloc_format_context();
	if ( !oc )
	{
		ktError() << "Error while export";
		return;
	}
	
	oc->oformat = fmt;
	snprintf(oc->filename, sizeof(oc->filename), "%s", filePath.toLatin1().data());
	
	AVStream *video_st = addVideoStream(oc, fmt->video_codec, fps);
	
	if ( !video_st )
	{
		ktError() << "Can't add video stream";
		return;
	}
	
	if (av_set_parameters(oc, 0) < 0)
	{
		ktError() << "Invalid output format parameters";
		return ;
	}
	
	dump_format(oc, 0, filePath.toLatin1().data(), 1);
	
	if (!openVideo(oc, video_st) )
	{
		ktError() << "Can't open video";
		return;
	}
	
	if (!(fmt->flags & AVFMT_NOFILE))
	{
		if (url_fopen(&oc->pb, filePath.toLatin1().data(), URL_WRONLY) < 0) 
		{
			ktError() << "Could not open " << filePath.toLatin1().data();
			return;
		}
	}
	
	av_write_header(oc);
	
	double video_pts = 0.0;
	
	QDir temp(KTOON_TEMP_DIR+"/exporting");
	if ( !temp.exists() )
	{
		temp.mkdir(temp.path());
	}
	
	QStringList paths = createImages(scenes, temp);
	
	foreach(QString imagePath, paths)
	{
		if (video_st)
		{
			video_pts = (double)video_st->pts.val * video_st->time_base.num / video_st->time_base.den;
		}
		else
		{
			video_pts = 0.0;
		}
		if (!video_st || video_pts >= m_streamDuration )
		{
			break;
		}
		
		if (! writeVideoFrame(imagePath, oc, video_st, fps) )
		{
			break;
		}
	}
	
	closeVideo(oc, video_st);
	av_write_trailer(oc);
	
	for(int i = 0; i < oc->nb_streams; i++)
	{
		av_freep(&oc->streams[i]);
	}
	
	if (!(fmt->flags & AVFMT_NOFILE)) 
	{
		/* close the output file */
		url_fclose(&oc->pb);
	}
	
	av_free(oc);
	
	foreach(QString path, paths)
	{
		QFile::remove(path);
	}
	
#endif
}

AVStream *FFMpegPlugin::addVideoStream(AVFormatContext *oc, int codec_id, int fps)
{
	AVCodecContext *c;
	AVStream *st;

	st = av_new_stream(oc, 0);
	if (!st) 
	{
		ktError() << "Could not alloc stream";
		return 0;
	}

	c = &st->codec;
	c->codec_id = CodecID(codec_id);
	c->codec_type = CODEC_TYPE_VIDEO;

	/* put sample parameters */
	c->bit_rate = 400000;
	/* resolution must be a multiple of two */
	c->width = 520; // FIXME FIXME FIXME
	c->height = 340; // FIXME FIXME FIXME
	/* frames per second */
	c->frame_rate = fps;
	c->frame_rate_base = 1;
	c->gop_size = 12; /* emit one intra frame every twelve frames at most */
	if (c->codec_id == CODEC_ID_MPEG2VIDEO) 
	{
		/* just for testing, we also add B frames */
		c->max_b_frames = 2;
	}
	if (c->codec_id == CODEC_ID_MPEG1VIDEO)
	{
        /* needed to avoid using macroblocks in which some coeffs overflow
		this doesnt happen with normal video, it just happens here as the
		motion of the chroma plane doesnt match the luma plane */
		c->mb_decision=2;
	}
    // some formats want stream headers to be seperate
	if(!strcmp(oc->oformat->name, "mp4") || !strcmp(oc->oformat->name, "mov") || !strcmp(oc->oformat->name, "3gp"))
	{
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}
	
	return st;
}

AVFrame *FFMpegPlugin::allocPicture(int pix_fmt, int width, int height)
{
	AVFrame *m_picture;
	uint8_t *picture_buf;
	int size;
    
	m_picture = avcodec_alloc_frame();
	if (!m_picture)
	{
		return 0;
	}
	
	size = avpicture_get_size(pix_fmt, width, height);
	picture_buf = (uint8_t *)malloc(size);
	if (!picture_buf)
	{
		av_free(m_picture);
		return 0;
	}
	avpicture_fill((AVPicture *)m_picture, picture_buf, pix_fmt, width, height);
	
	return m_picture;
}

bool FFMpegPlugin::writeVideoFrame(const QString &imagePath, AVFormatContext *oc, AVStream *st, int fps)
{
	AVCodecContext *c = &st->codec;
	
	AVPicture *pict = (AVPicture *) m_picture;
	AVFrame *picturePtr;
	
	
	double nbFrames = ((int)(m_streamDuration * fps));
	if (m_frameCount >= nbFrames)
	{
        	/* no more frame to compress. The codec has a latency of a few
		frames if using B frames, so we get the last frames by
		passing a 0 m_picture */
		picturePtr = 0;
	}
	else
	{
		QImage image(imagePath);
		
		AVPicture pictTmp;
		
		avpicture_alloc(&pictTmp, PIX_FMT_RGBA32,c->width, c->height);

		memcpy(pictTmp.data[0],image.bits(),c->width*c->height*4);

		img_convert((AVPicture *)m_picture,c->pix_fmt,&pictTmp,PIX_FMT_RGBA32,c->width,c->height);
		avpicture_free(&pictTmp);

		picturePtr = m_picture;
	}

	int out_size = -1, ret = -1;
	if (oc->oformat->flags & AVFMT_RAWPICTURE)
	{
        	/* raw video case. The API will change slightly in the near
		futur for that */
		AVPacket pkt;
		av_init_packet(&pkt);
        
		pkt.flags |= PKT_FLAG_KEY;
		pkt.stream_index= st->index;
		pkt.data= (uint8_t *)picturePtr;
		pkt.size= sizeof(AVPicture);
        
		ret = av_write_frame(oc, &pkt);
	} 
	else 
	{
		/* encode the image */
		out_size = avcodec_encode_video(c, videOutbuf, videOutbufSize, picturePtr);
		/* if zero size, it means the image was buffered */

		if (out_size != 0)
		{
			AVPacket pkt;
			av_init_packet(&pkt);
            
			pkt.pts= c->coded_frame->pts;
			if(c->coded_frame->key_frame)
				pkt.flags |= PKT_FLAG_KEY;
			pkt.stream_index= st->index;
			pkt.data= videOutbuf;
			pkt.size= out_size;
            
			/* write the compressed frame in the media file */
			ret = av_write_frame(oc, &pkt);
		} 
		else 
		{
			ret = 0;
		}
	}
	if (ret != 0)
	{
		ktError() << "Error while writing video frame";
		return false;
	}
	m_frameCount++;
	
	return true;
}

bool FFMpegPlugin::openVideo(AVFormatContext *oc, AVStream *st)
{
	AVCodec *codec;
	AVCodecContext *c;

	c = &st->codec;

	/* find the video encoder */
	codec = avcodec_find_encoder(c->codec_id);
	if (!codec)
	{
		ktError() << "codec not found";
		return false;
	}

	/* open the codec */
	if (avcodec_open(c, codec) < 0)
	{
		ktError() << "could not open codec";
		return false;
	}

	videOutbuf = 0;
	if (!(oc->oformat->flags & AVFMT_RAWPICTURE))
	{
		/* allocate output buffer */
		/* XXX: API change will be done */
		videOutbufSize = 200000;
// 		videOutbuf = (uint8_t *) malloc(videOutbufSize);
		videOutbuf = (uint8_t *) av_malloc(videOutbufSize);
	}

	/* allocate the encoded raw m_picture */
	m_picture = allocPicture(c->pix_fmt, c->width, c->height);
	if (!m_picture) 
	{
		ktError() << "Could not allocate m_picture";
		return false;
	}
	
	m_tmpPicture = 0;
	if (c->pix_fmt != PIX_FMT_YUV420P) 
	{
		m_tmpPicture = allocPicture(PIX_FMT_YUV420P, c->width, c->height);
		if (!m_tmpPicture)
		{
			ktError() << "Could not allocate temporary picture";
			return false;
		}
	}
	
	return true;
}

void FFMpegPlugin::closeVideo(AVFormatContext *oc, AVStream *st)
{
	avcodec_close(&st->codec);
	av_free(m_picture->data[0]);
	av_free(m_picture);
	if (m_tmpPicture)
	{
		av_free(m_tmpPicture->data[0]);
		av_free(m_tmpPicture);
	}
	av_free(videOutbuf);
}

QStringList FFMpegPlugin::createImages(const QList<KTScene *> &scenes, const QDir &dir)
{
	QStringList paths;
	
	int nPhotogramsRenderized = 0;
	
	foreach(KTScene *scene, scenes )
	{
		Layers layers = scene->layers();
		
		bool m_isRendered = false;
	
		while ( ! m_isRendered )
		{
			Layers::iterator layerIterator = layers.begin();
			bool ok = true;
			
			QImage renderized = QImage(520, 340, QImage::Format_RGB32);
			renderized.fill(qRgb(255, 255, 255));
			
			QPainter painter(&renderized);
			painter.setRenderHint(QPainter::Antialiasing);
			
			while ( layerIterator != layers.end() )
			{
				ok = ok && (nPhotogramsRenderized > (*layerIterator)->frames().count());
				
				if ( *layerIterator && nPhotogramsRenderized < (*layerIterator)->frames().count() && (*layerIterator)->isVisible() )
				{
					KTKeyFrame *frame = (*layerIterator)->frames()[nPhotogramsRenderized];
					if ( frame )
					{
						QList<AGraphicComponent *> componentList = frame->components();
												
						if ( componentList.count() > 0  )
						{
							QList<AGraphicComponent *>::iterator it = componentList.begin();
													
							while ( it != componentList.end() )
							{
								if ( *it )
								{
									painter.save();
															
									painter.setPen((*it)->pen());
									painter.setBrush((*it)->brush());
															
									// painter.drawPath((*it)->path());
									QList<QPolygonF> poligons =   (*it)->path().toSubpathPolygons();
									QList<QPolygonF>::const_iterator it;
									for(it = poligons.begin(); it != poligons.end(); ++it)
									{
										painter.drawPolygon(*it);
									}
									painter.restore();
								}
								++it;
							}
						}
					}
				}
				++layerIterator;
			}
			
			QString file = "";
			if ( nPhotogramsRenderized < 10 )
			{
				file = QString("000%1.png").arg(nPhotogramsRenderized);
			}
			else if ( nPhotogramsRenderized < 100 )
			{
				file = QString("00%1.png").arg(nPhotogramsRenderized);
			}
			else if( nPhotogramsRenderized < 1000 )
			{
				file = QString("0%1.png").arg(nPhotogramsRenderized);
			}
			else if( nPhotogramsRenderized < 10000 )
			{
				file = QString("%1.png").arg(nPhotogramsRenderized);
			}
			
			renderized.save(dir.path()+"/"+file, "PNG");
// 			emit progressStep( nPhotogramsRenderized, totalPhotograms);
			
			paths << dir.path()+"/"+file;
			
			if (ok )
			{
				m_isRendered = true;
			}
			
			nPhotogramsRenderized++;
		}
	}
	
	return paths;
}


#ifdef HAVE_FFMPEG
Q_EXPORT_PLUGIN( FFMpegPlugin );
#endif

