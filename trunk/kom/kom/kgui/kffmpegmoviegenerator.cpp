/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
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

#define INT64_C
#define __STDC_CONSTANT_MACROS
#include <stdint.h>

#include "kffmpegmoviegenerator.h"

#include <QDir>

#include <kcore/kdebug.h>
#include <kcore/kalgorithm.h>

#ifdef __cplusplus
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}
#endif

struct KFFMpegMovieGenerator::Private
{
    AVFrame *picture;
    AVFrame *tmpPicture;

    QString movieFile;
    int fps;
    double video_pts;
    uint8_t *videOutbuf;
    int frameCount;
    int videOutbufSize;
    double streamDuration;
    bool exception;
    const char *errorMsg;

    AVStream *video_st;
    AVFormatContext *oc;
    AVOutputFormat *fmt;

    void chooseFileExtension(int format);
    bool openVideo(AVFormatContext *oc, AVStream *st);
    void RGBtoYUV420P(const uint8_t *bufferRGB, uint8_t *bufferYUV, uint iRGBIncrement, bool bSwapRGB, int width, int height);

    void fill_yuv_image(AVFrame *pict, int frame_index, int width, int height);
    bool writeVideoFrame(const QImage &image, AVFormatContext *oc, AVStream *st);
    void closeVideo(AVFormatContext *oc, AVStream *st);
};

static AVStream *addVideoStream(AVFormatContext *oc, enum CodecID codec_id, int width, int height, int fps, const char *errorMsg)
{
    AVCodecContext *c;
    AVStream *st;

    int w = width;
    int h = height;

    st = av_new_stream(oc, 0);
    if (!st) {
        errorMsg = "ffmpeg error: Could not alloc stream. This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";
        #ifdef K_DEBUG
               kError() << errorMsg;
        #endif
        return 0;
    }

    c = st->codec;
    //c->codec_id = CodecID(codec_id);
    c->codec_id = codec_id;
    c->codec_type = CODEC_TYPE_VIDEO;

    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = w;  
    c->height = h; 

    c->time_base.den = fps;
    c->time_base.num = 1;
    c->gop_size = 12; /* emit one intra frame every twelve frames at most */
    c->pix_fmt = PIX_FMT_YUV420P;

    c->gop_size = 12; /* emit one intra frame every twelve frames at most */
    if (c->codec_id == CODEC_ID_MPEG2VIDEO) {
	/* just for testing, we also add B frames */
	c->max_b_frames = 2;
    }

    if (c->codec_id == CODEC_ID_MPEG1VIDEO) {
        /* needed to avoid using macroblocks in which some coeffs overflow
           this doesnt happen with normal video, it just happens here as the
           motion of the chroma plane doesnt match the luma plane */
           c->mb_decision=2;
    }

    // some formats want stream headers to be seperate

    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= CODEC_FLAG_GLOBAL_HEADER;
	
    return st;
}

static AVFrame *allocPicture(enum PixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    uint8_t *picture_buf;
    int size;
 
    picture = avcodec_alloc_frame();

    if (!picture) 
        return 0;
	
    size = avpicture_get_size(pix_fmt, width, height);
    picture_buf = (uint8_t *)malloc(size);

    if (!picture_buf) {
	av_free(picture);
	return 0;
    }

    avpicture_fill((AVPicture *)picture, picture_buf, pix_fmt, width, height);

    return picture;
}

void KFFMpegMovieGenerator::Private::chooseFileExtension(int format)
{
    switch (format) {
            case ASF:
                 movieFile += ".asf";
                 break;
            case AVI:
                 movieFile += ".avi";
                 break;
            case MOV:
                 movieFile += ".mov";
                 break;
            case OGG:
                 movieFile += ".ogg";
                 break;
            case RM:
                 movieFile += ".rm";
                 break;
            case SWF:
                 movieFile += ".swf";
                 break;
            default:
                 movieFile += ".mpg";
                 break;
    }
}

bool KFFMpegMovieGenerator::Private::openVideo(AVFormatContext *oc, AVStream *st)
{
    AVCodec *codec;
    AVCodecContext *c;

    c = st->codec;

    /* find the video encoder */
    codec = avcodec_find_encoder(c->codec_id);

    if (!codec) {
        errorMsg = "ffmpeg error: Video codec not found. This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. It's very possible your system is missing codecs. More info: http://ffmpeg.org/";
        #ifdef K_DEBUG
               kError() << errorMsg;
        #endif

        return false;
    }

    /* open the codec */
    if (avcodec_open(c, codec) < 0) {
        errorMsg = "ffmpeg error: Could not open video codec. This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";
        #ifdef K_DEBUG
               kError() << errorMsg;
        #endif
        return false;
    }

    videOutbuf = 0;
    if (!(oc->oformat->flags & AVFMT_RAWPICTURE)) {
        videOutbufSize = 200000;
        videOutbuf = (uint8_t *) av_malloc(videOutbufSize);
    }

    /* allocate the encoded raw m_picture */
    picture = allocPicture(c->pix_fmt, c->width, c->height);

    if (!picture) {
        errorMsg = "ffmpeg error: Could not allocate m_picture. This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";
        #ifdef K_DEBUG
               kError() << errorMsg;
        #endif 
        return false;
    }

    tmpPicture = 0;

    if (c->pix_fmt != PIX_FMT_YUV420P) {
        tmpPicture = allocPicture(PIX_FMT_YUV420P, c->width, c->height);
        if (!tmpPicture) {
            errorMsg = "ffmpeg error: Could not allocate temporary picture. This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";
            #ifdef K_DEBUG
                   kError() << errorMsg;
            #endif
            return false;
        }
    }
	
    return true;
}

#define rgbtoyuv(r, g, b, y, u, v) \
  y = (uint8_t)(((int)30*r + (int)59*g + (int)11*b)/100); \
  u = (uint8_t)(((int)-17*r - (int)33*g + (int)50*b + 12800)/100); \
  v = (uint8_t)(((int)50*r - (int)42*g - (int)8*b + 12800)/100);

void KFFMpegMovieGenerator::Private::RGBtoYUV420P(const uint8_t *bufferRGB, uint8_t *bufferYUV, uint iRGBIncrement, bool bSwapRGB, int width, int height)
{
    const unsigned iPlaneSize = width * height;
    const unsigned iHalfWidth = width >> 1;

    // get pointers to the data
    uint8_t *yplane  = bufferYUV;
    uint8_t *uplane  = bufferYUV + iPlaneSize;
    uint8_t *vplane  = bufferYUV + iPlaneSize + (iPlaneSize >> 2);
    const uint8_t *bufferRGBIndex = bufferRGB;

    int iRGBIdx[3];
    iRGBIdx[0] = 0;
    iRGBIdx[1] = 1;
    iRGBIdx[2] = 2;

    if (bSwapRGB)  {
        iRGBIdx[0] = 2;
        iRGBIdx[2] = 0;
    }
 
    for (int y = 0; y < (int) height; y++) {
         uint8_t *yline  = yplane + (y * width);
         uint8_t *uline  = uplane + ((y >> 1) * iHalfWidth);
         uint8_t *vline  = vplane + ((y >> 1) * iHalfWidth);

         for (int x=0; x<width; x+=2) {
              rgbtoyuv ( bufferRGBIndex[iRGBIdx[0]], bufferRGBIndex[iRGBIdx[1]], bufferRGBIndex[iRGBIdx[2]], *yline, *uline, *vline );
              bufferRGBIndex += iRGBIncrement;
              yline++;
              rgbtoyuv ( bufferRGBIndex[iRGBIdx[0]], bufferRGBIndex[iRGBIdx[1]], bufferRGBIndex[iRGBIdx[2]], *yline, *uline, *vline );
              bufferRGBIndex += iRGBIncrement;
              yline++;
              uline++;
              vline++;
         }
    }
}

/* prepare a dummy image */
void KFFMpegMovieGenerator::Private::fill_yuv_image(AVFrame *pict, int frame_index, int width, int height)
{
    int x, y, i;

    i = frame_index;

    /* Y */
    for(y=0;y<height;y++) {
        for(x=0;x<width;x++) {
            pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;
        }
    }

    /* Cb and Cr */
    for(y=0;y<height/2;y++) {
        for(x=0;x<width/2;x++) {
            pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
            pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
        }
    }
}

bool KFFMpegMovieGenerator::Private::writeVideoFrame(const QImage &image, AVFormatContext *oc, AVStream *st)
{
    #ifdef K_DEBUG
           kDebug() << "* Generating frame #" << frameCount;
    #endif

    AVCodecContext *c;
    c = st->codec;

    double nbFrames = ((int)(streamDuration * fps));

    if (frameCount < nbFrames) {
        int w = c->width;
        int h = c->height;

        int size = avpicture_get_size(PIX_FMT_YUV420P, w, h);
        uint8_t *pic_dat = (uint8_t *) av_malloc(size);
        RGBtoYUV420P(image.bits(), pic_dat, image.depth()/8, true, w, h);

        picture->quality = 0;
        //avpicture_fill((AVPicture *)picture, pic_dat,
        //                PIX_FMT_YUV420P, w, h);

        fill_yuv_image(picture, frameCount, w, h);
    }

    int out_size = -1, ret = -1;

    if (oc->oformat->flags & AVFMT_RAWPICTURE) { // Exporting images array
        AVPacket pkt;
        av_init_packet(&pkt);

        pkt.flags |= PKT_FLAG_KEY;
        //pkt.stream_index = video_st->index;
        pkt.stream_index= st->index;
        pkt.data = (uint8_t *)picture;
        pkt.size = sizeof(AVPicture);
        
        ret = av_interleaved_write_frame(oc, &pkt);

    } else { // Exporting movies

        out_size = avcodec_encode_video(c, videOutbuf, videOutbufSize, picture);

        if (out_size > 0) {
            AVPacket pkt;
            av_init_packet(&pkt);

            if (c->coded_frame->pts != (int64_t) AV_NOPTS_VALUE) 
                //pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, video_st->time_base);
                pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);

            if (c->coded_frame->key_frame)
                pkt.flags |= PKT_FLAG_KEY;

            //pkt.stream_index = video_st->index;
            pkt.stream_index = st->index;
            pkt.data = videOutbuf;
            pkt.size = out_size;

            /* write the compressed frame in the media file */
            ret = av_interleaved_write_frame(oc, &pkt);
        } else {
            ret = 0;
        }
   }

   if (ret != 0) {
       errorMsg = "ffmpeg error: Could not write video frame. This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";

       #ifdef K_DEBUG
              kError() << errorMsg;
       #endif
       return false;
   }

   frameCount++;

   return true;
}

void KFFMpegMovieGenerator::Private::closeVideo(AVFormatContext *oc, AVStream *st)
{
    avcodec_close(st->codec);
    av_free(picture->data[0]);
    av_free(picture);

    if (tmpPicture) {
        av_free(tmpPicture->data[0]);
        av_free(tmpPicture);
    }
    av_free(videOutbuf);
}

KFFMpegMovieGenerator::KFFMpegMovieGenerator(KMovieGeneratorInterface::Format format, int width, int height, int fps)
 : KMovieGenerator(width, height), k(new Private)
{
    k->movieFile = QDir::tempPath() + "/ktoon_video" + KAlgorithm::randomString(12);
    k->chooseFileExtension(format);
    k->fps = fps;
    k->exception = begin();
}

// Constructor called from Export dialog

KFFMpegMovieGenerator::KFFMpegMovieGenerator(KMovieGeneratorInterface::Format format, const QSize &size, int fps) : KMovieGenerator(size.width(), size.height()), k(new Private)
{
    k->movieFile = QDir::tempPath() + "/ktoon_video" + KAlgorithm::randomString(12);
    k->chooseFileExtension(format);
    k->fps = fps;
    k->exception = begin();
}

KFFMpegMovieGenerator::~KFFMpegMovieGenerator()
{
    if (QFile::exists(k->movieFile)) 
        QFile::remove(k->movieFile);

    delete k;
}

bool KFFMpegMovieGenerator::begin()
{
    av_register_all();

    k->fmt = guess_format(0, k->movieFile.toLocal8Bit().data(), 0);

    if (!k->fmt) 
         k->fmt = guess_format("mpeg", NULL, NULL);

    if (! k->fmt) {
        k->errorMsg = "ffmpeg error: Cannot find a valid format for " + k->movieFile.toLocal8Bit() + ". This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";
        #ifdef K_DEBUG
               kError() << k->errorMsg;
        #endif
        return false;
    }

    k->oc = avformat_alloc_context();

    if (!k->oc) {
        k->errorMsg = "ffmpeg error: Error while doing export. This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";
        #ifdef K_DEBUG
               kError() << k->errorMsg;
        #endif
        return false;
    }
	
    k->oc->oformat = k->fmt;
    snprintf(k->oc->filename, sizeof(k->oc->filename), "%s", k->movieFile.toLocal8Bit().data());

    k->video_st = 0;

    if (k->fmt->video_codec != CODEC_ID_NONE)
        k->video_st = addVideoStream(k->oc, k->fmt->video_codec, width(), height(), 
                                     k->fps, k->errorMsg);
	
    if (!k->video_st) {
        k->errorMsg = "ffmpeg error: Can't add video stream. This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";
        #ifdef K_DEBUG
               kError() << k->errorMsg;
        #endif
        return false;
    }

    if (av_set_parameters(k->oc, 0) < 0) {
        k->errorMsg = "ffmpeg error: Invalid output format parameters. This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";
        #ifdef K_DEBUG
               kError() << k->errorMsg;
        #endif
        return false;
    }

    dump_format(k->oc, 0, k->movieFile.toLocal8Bit().data(), 1);

    /*
    if (!k->openVideo(k->oc, k->video_st)) {
        #ifdef K_DEBUG
               kError() << "Can't open video";
        #endif
        return false;
    }
    */

   if (k->video_st)
       k->openVideo(k->oc, k->video_st);

    if (!(k->fmt->flags & AVFMT_NOFILE)) {
        if (url_fopen(&k->oc->pb, k->movieFile.toLocal8Bit().data(), URL_WRONLY) < 0) {
            k->errorMsg = "ffmpeg error: Could not open " + k->movieFile.toLocal8Bit() + ". This is not a KToon problem directly. Please, check your ffmpeg installation and codec support. More info: http://ffmpeg.org/";
            #ifdef K_DEBUG
                   kError() << k->errorMsg;
            #endif
            return false;
        }
    }

    av_write_header(k->oc);

    return true;
}

bool KFFMpegMovieGenerator::movieHeaderOk() 
{ 
    return k->exception;
}

const char* KFFMpegMovieGenerator::getErrorMsg() 
{
    return k->errorMsg;
}

void KFFMpegMovieGenerator::handle(const QImage& image)
{
    if (k->video_st) 
        k->video_pts = (double)k->video_st->pts.val * k->video_st->time_base.num / k->video_st->time_base.den;
    else 
        k->video_pts = 0.0;

    k->frameCount = 0;
    k->streamDuration = 10;

    if (!k->video_st || k->video_pts >= k->streamDuration)
        return;

    k->writeVideoFrame(image, k->oc, k->video_st);
}

void KFFMpegMovieGenerator::end()
{
    av_write_trailer(k->oc);

    if (k->video_st)
        k->closeVideo(k->oc, k->video_st);

    int streams_total = k->oc->nb_streams;
    for (int i = 0; i < streams_total; i++) {
         av_freep(&k->oc->streams[i]->codec);  
         av_freep(&k->oc->streams[i]);
    }

    if (!(k->fmt->flags & AVFMT_NOFILE)) {
        url_fclose(k->oc->pb);
    }

    av_free(k->oc);
}


void KFFMpegMovieGenerator::__saveMovie(const QString &fileName)
{
    if (QFile::exists(fileName)) 
        QFile::remove(fileName);

    QFile::copy(k->movieFile, fileName);
}
