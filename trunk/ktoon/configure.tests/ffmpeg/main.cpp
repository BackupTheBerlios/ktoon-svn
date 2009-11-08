#ifdef __cplusplus
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}
#endif

#include <iostream>

int main()
{
	av_register_all();
	AVOutputFormat *fmt = guess_format("mpeg", NULL, NULL);
	
	AVFormatContext *oc = av_alloc_format_context();
	
	oc->oformat = fmt;
	
	AVCodecContext *c;
	AVStream *st;
	
	st = av_new_stream(oc, 0);
	
	c = st->codec;
	
	c->time_base.den = 24;
	c->time_base.num = 1;
	c->gop_size = 12;
	c->pix_fmt = PIX_FMT_YUV420P;
	
	
	av_free(oc);
	
	
	return 0;
}

