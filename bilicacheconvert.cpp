#include <iostream>
#include <string>
#include "ffmpeg_mux.h"

using namespace std;

int main(int argc, char **argv)
{
    if(FFmuxing("../data/video.m4s", "../data/audio.m4s", "../data/output.mp4") == 0)
        return 0;

    return 1;
}