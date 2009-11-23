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

#include "kffmpegmoviegenerator.h"

#include <QDir>

#include <kcore/kdebug.h>
#include <kcore/kalgorithm.h>

#ifdef __cplusplus
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
// #include "libswscale/swscale.h"
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

    AVStream *video_st;
    AVFormatContext *oc;
    AVOutputFormat *fmt;

    void chooseFileExtension(int format);
    bool openVideo(AVFormatContext *oc, AVStream *st);
    bool writeVideoFrame(const QImage &image);
    void closeVideo(AVStream *st);
};

static AVStream *addVideoStream(AVFormatContext *oc, int codec_id, int width, int height, int fps)
{
    AVCodecContext *c;
    AVStream *st;

    int w = width;
    int h = height;

    st = av_new_stream(oc, 0);
    if (!st) {
        kError() << "Could not alloc stream";
        return 0;
    }

    c = st->codec;

    c->codec_id = CodecID(codec_id);
    c->codec_type = CODEC_TYPE_VIDEO;

    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = w;  // 520
    c->height = h; // 340

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
    if (!strcmp(oc->oformat->name, "mp4") || !strcmp(oc->oformat->name, "mov") || !strcmp(oc->oformat->name, "3gp")) 
        c->flags |= CODEC_FLAG_GLOBAL_HEADER;
	
    return st;
}

static AVFrame *allocPicture(int pix_fmt, int width, int height)
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
        kError() << "codec not found";
        return false;
    }

    /* open the codec */
    if (avcodec_open(c, codec) < 0) {
        kError() << "could not open codec";
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
        kError() << "Could not allocate m_picture";
        return false;
    }

    tmpPicture = 0;

    if (c->pix_fmt != PIX_FMT_YUV420P) {
        tmpPicture = allocPicture(PIX_FMT_YUV420P, c->width, c->height);
        if (!tmpPicture) {
            kError() << "Could not allocate temporary picture";
            return false;
        }
    }
	
    return true;
}

bool KFFMpegMovieGenerator::Private::writeVideoFrame(const QImage &image)
{
    #ifdef K_DEBUG
           kDebug() << "Generating frame #" << frameCount;
    #endif

    AVCodecContext *c = video_st->codec;
    AVFrame *picturePtr;
    double nbFrames = ((int)(streamDuration * fps));

    if (frameCount < nbFrames) {
        int size;
        uint8_t *picture_buf;
        int w = c->width;
        int h = c->height;

        picturePtr = avcodec_alloc_frame();
        size = avpicture_get_size(PIX_FMT_YUV420P, w, h);
        picture_buf = (uint8_t *) av_malloc(size);
        memcpy(picture_buf,image.bits(),size);
        avpicture_fill((AVPicture *)picturePtr, picture_buf,
                        PIX_FMT_YUV420P, w, h);

        //fill_yuv_image(picturePtr, frameCount, w, h);
    }

    int out_size = -1, ret = -1;

    if (oc->oformat->flags & AVFMT_RAWPICTURE) { // Exporting images array
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt.flags |= PKT_FLAG_KEY;
        pkt.stream_index= video_st->index;
        pkt.data= (uint8_t *)picturePtr;
        pkt.size= sizeof(AVPicture);
        
        ret = av_write_frame(oc, &pkt);

    } else { // Exporting movies
        out_size = avcodec_encode_video(c, videOutbuf, videOutbufSize, picturePtr);

        if (out_size > 0) {
            AVPacket pkt;
            av_init_packet(&pkt);
            pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, video_st->time_base);

            if (c->coded_frame->key_frame)
                pkt.flags |= PKT_FLAG_KEY;
                pkt.stream_index = video_st->index;
                pkt.data= videOutbuf;
                pkt.size= out_size;
                ret = av_write_frame(oc, &pkt);
            } else {
                ret = 0;
            }
        }

        if (ret != 0) {
            kError() << "Error while writing video frame";
            return false;
        }

        frameCount++;

        return true;
}

void KFFMpegMovieGenerator::Private::closeVideo(AVStream *st)
{
    AVCodecContext *c;
    c = st->codec;
    avcodec_close(c);
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
    k->movieFile = QDir::tempPath()+"/ffmpeg_video"+KAlgorithm::randomString(12);
    k->chooseFileExtension(format);
    k->fps = fps;
    begin();
}

KFFMpegMovieGenerator::KFFMpegMovieGenerator(KMovieGeneratorInterface::Format format, const QSize &size, int fps) : KMovieGenerator(size.width(), size.height()), k(new Private)
{
    k->movieFile = QDir::tempPath()+"/ffmpeg_video"+KAlgorithm::randomString(12);
    k->chooseFileExtension(format);
    k->fps = fps;
    begin();
}

KFFMpegMovieGenerator::~KFFMpegMovieGenerator()
{
    if ( QFile::exists( k->movieFile ) ) {
         QFile::remove(k->movieFile);
    }
    delete k;
}

bool KFFMpegMovieGenerator::begin()
{
    av_register_all();
    k->fmt = guess_format(0, k->movieFile.toLocal8Bit().data(), 0);

    if ( !k->fmt ) 
         k->fmt = guess_format("mpeg", NULL, NULL);

    if ( ! k->fmt ) {
         kError() << "Cannot find a valid format for " << k->movieFile;
         return false;
    }

    //k->oc = av_alloc_format_context();
    k->oc = avformat_alloc_context();

    if ( !k->oc ) {
         kError() << "Error while export";
         return false;
    }
	
    k->oc->oformat = k->fmt;
    snprintf(k->oc->filename, sizeof(k->oc->filename), "%s", k->movieFile.toLocal8Bit().data());
    k->video_st = addVideoStream(k->oc, k->fmt->video_codec, width(), height(), k->fps);
	
    if ( !k->video_st ) {
         kError() << "Can't add video stream";
         return false;
    }

    if (av_set_parameters(k->oc, 0) < 0) {
        kError() << "Invalid output format parameters";
        return false;
    }

    dump_format(k->oc, 0, k->movieFile.toLocal8Bit().data(), 1);

    if (!k->openVideo(k->oc, k->video_st) ) {
        kError() << "Can't open video";
        return false;
    }

    if (!(k->fmt->flags & AVFMT_NOFILE)) {
        if (url_fopen(&k->oc->pb, k->movieFile.toLocal8Bit().data(), URL_WRONLY) < 0) {
            kError() << "Could not open " << k->movieFile;
            return false;
        }
    }

    av_write_header(k->oc);

    k->video_pts = 0.0;
    k->frameCount = 0;
    k->streamDuration = 10;

    return true;
}

void KFFMpegMovieGenerator::handle(const QImage& image)
{
    if (k->video_st) 
        k->video_pts = (double)k->video_st->pts.val * k->video_st->time_base.num / k->video_st->time_base.den;
    else 
        k->video_pts = 0.0;


    if (!k->video_st || k->video_pts >= k->streamDuration )
        return;

    k->writeVideoFrame(image);
}


void KFFMpegMovieGenerator::end()
{
    k->closeVideo(k->video_st);
    av_write_trailer(k->oc);

    int streams_total = k->oc->nb_streams;
    for (int i = 0; i < streams_total; i++) {
         av_freep(&k->oc->streams[i]);
    }

    if (!(k->fmt->flags & AVFMT_NOFILE)) {
        //url_fclose(&k->oc->pb);
        url_fclose(k->oc->pb);
    }

    av_free(k->oc);
}


void KFFMpegMovieGenerator::__saveMovie(const QString &fileName)
{
    if ( QFile::exists( fileName ) ) 
         QFile::remove(fileName);

    QFile::copy(k->movieFile, fileName);
}
