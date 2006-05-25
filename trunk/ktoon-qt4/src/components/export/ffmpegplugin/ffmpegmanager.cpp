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

#include "ffmpegmanager.h"

#include "ddebug.h"
#include "dglobal.h"

#include <cstdio>

#include <QImage>

#include "exportinterface.h"

FFMpegManager::FFMpegManager()
{
#ifdef HAVE_FFMPEG
	av_register_all();
	
	m_frameCount = 0;
	m_streamDuration = 10.0;
#endif
}


FFMpegManager::~FFMpegManager()
{
}

void FFMpegManager::create(const QString &filePath, int formatId, const QStringList &paths, const QSize &size, int fps)
{
#ifdef HAVE_FFMPEG
	
	AVOutputFormat *fmt = guess_format(0, filePath.toLatin1().data(), 0);
	
	if ( !fmt )
	{
		fmt = guess_format("mpeg", NULL, NULL);
	}
	
// 	AVFormatParameters params, *ap = &params;
	
	switch(formatId)
	{
		case ExportInterface::ASF:
		{
			
		}
		break;
		case ExportInterface::AVI:
		{
			fmt->video_codec = CODEC_ID_MSMPEG4V3;
// 			video_st->codec.codec_tag = 0;
		}
		break;
		case ExportInterface::MOV:
		{
			
		}
		break;
		case ExportInterface::MPEG:
		{
		}
		break;
		case ExportInterface::RM:
		{
			
		}
		break;
		case ExportInterface::SWF:
		{
			
		}
		break;
		case ExportInterface::GIF:
		{
// 			AVImageFormat *imageFormat = guess_image_format(filePath.toLatin1().data());
// 			
// 			memset(ap, 0, sizeof(*ap));
// 			ap->image_format = imageFormat;
		}
		break;
		default: break;
	}
	
	AVFormatContext *oc = av_alloc_format_context();
	if ( !oc )
	{
		dError() << "Error while export";
		return;
	}

	
	oc->oformat = fmt;
	snprintf(oc->filename, sizeof(oc->filename), "%s", filePath.toLatin1().data());
	
	AVStream *video_st = addVideoStream(oc, fmt->video_codec, size.width(), size.height(), fps);
	
	if ( !video_st )
	{
		dError() << "Can't add video stream";
		return;
	}
	
	if (av_set_parameters(oc, 0) < 0)
	{
		dError() << "Invalid output format parameters";
		return ;
	}
	
	dump_format(oc, 0, filePath.toLatin1().data(), 1);
	
	if (!openVideo(oc, video_st) )
	{
		dError() << "Can't open video";
		return;
	}
	
	if (!(fmt->flags & AVFMT_NOFILE))
	{
		if (url_fopen(&oc->pb, filePath.toLatin1().data(), URL_WRONLY) < 0) 
		{
			dError() << "Could not open " << filePath.toLatin1().data();
			return;
		}
	}
	
	av_write_header(oc);
	
	double video_pts = 0.0;
	
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
#endif
}

#ifdef HAVE_FFMPEG

AVStream *FFMpegManager::addVideoStream(AVFormatContext *oc, int codec_id, int width, int height, int fps)
{
	AVCodecContext *c;
	AVStream *st;
	
	int w = width;
	int h = height;

	st = av_new_stream(oc, 0);
	if (!st) 
	{
		dError() << "Could not alloc stream";
		return 0;
	}

#if LIBAVCODEC_BUILD <= 4743
	c = &st->codec;
#else
	c = st->codec;
#endif

	c->codec_id = CodecID(codec_id);
	c->codec_type = CODEC_TYPE_VIDEO;

	/* put sample parameters */
	c->bit_rate = 400000;
	/* resolution must be a multiple of two */
	c->width = w;  // 520
	c->height = h; // 340

#if LIBAVCODEC_BUILD <= 4743
	/* frames per second */
	c->frame_rate = fps;
	c->frame_rate_base = 1;
#else
	c->time_base.den = fps;
	c->time_base.num = 1;
	c->gop_size = 12; /* emit one intra frame every twelve frames at most */
	c->pix_fmt = PIX_FMT_YUV420P;
#endif

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

AVFrame *FFMpegManager::allocPicture(int pix_fmt, int width, int height)
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

bool FFMpegManager::writeVideoFrame(const QString &imagePath, AVFormatContext *oc, AVStream *st, int fps)
{
#if LIBAVCODEC_BUILD <= 4743
	AVCodecContext *c = &st->codec;
#else
	AVCodecContext *c = st->codec;
#endif

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
// 		out_size = av_write_trailer(oc);
		/* if zero size, it means the image was buffered */

		if (out_size > 0)
		{
			AVPacket pkt;
			av_init_packet(&pkt);
            
// 			pkt.pts= c->coded_frame->pts;
			pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);
			
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
		dError() << "Error while writing video frame";
		return false;
	}
	m_frameCount++;
	
	return true;
}

bool FFMpegManager::openVideo(AVFormatContext *oc, AVStream *st)
{
	AVCodec *codec;
	AVCodecContext *c;
#if LIBAVCODEC_BUILD <= 4743
	c = &st->codec;
#else
	c = st->codec;
#endif
	/* find the video encoder */
	
	codec = avcodec_find_encoder(c->codec_id);
	
	if (!codec)
	{
		dError() << "codec not found";
		return false;
	}

	/* open the codec */
	if (avcodec_open(c, codec) < 0)
	{
		dError() << "could not open codec";
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
		dError() << "Could not allocate m_picture";
		return false;
	}
	
	m_tmpPicture = 0;
	if (c->pix_fmt != PIX_FMT_YUV420P) 
	{
		m_tmpPicture = allocPicture(PIX_FMT_YUV420P, c->width, c->height);
		if (!m_tmpPicture)
		{
			dError() << "Could not allocate temporary picture";
			return false;
		}
	}
	
	return true;
}

void FFMpegManager::closeVideo(AVFormatContext *oc, AVStream *st)
{
	AVCodecContext *c;
#if LIBAVCODEC_BUILD <= 4743
	c = &st->codec;
#else
	c = st->codec;
#endif
	avcodec_close(c);
	av_free(m_picture->data[0]);
	av_free(m_picture);
	if (m_tmpPicture)
	{
		av_free(m_tmpPicture->data[0]);
		av_free(m_tmpPicture);
	}
	av_free(videOutbuf);
}

#endif


