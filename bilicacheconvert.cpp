#include <iostream>
#include <string>
#include "ffmpeg_mux.h"

using namespace std;


#ifdef __cplusplus
extern "C"
{
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>

#ifdef __cplusplus
}
#endif

int main(int argc, char **argv)
{
    if(FFmuxing("../data/video.m4s", "../data/audio.m4s", "../data/output.mp4") == 0)
        return 0;

    return 1;
}