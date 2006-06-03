#include <ffmpeg/avformat.h>
#include <ffmpeg/avcodec.h>

#include <iostream>

int main()
{
#if FFMPEG_VERSION_INT != 0x000409
	#error ffmpeg version != 0.4.9
#endif

	return 0;
}

