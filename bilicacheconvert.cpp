#include <iostream>
#include <string>
//#include "ffmpeg_mux.h"
//#include "header.h"

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
    // if(FFmuxing("../data/video.m4s", "../data/audio.m4s", "../data/output.mp4") == 0)
    //     return 0;

    av_log_set_level(AV_LOG_INFO);

    AVIODirContext *dir_ctx = NULL;
    AVIODirEntry *dir_entry = NULL;
    int ret;

    if ((ret = avio_open_dir(&dir_ctx, "E:\\code", NULL)) < 0) 
    {
        av_log(NULL, AV_LOG_ERROR, "Cannot open directory: %s.\n", av_err2str(ret));
        return 1;
    }
    cout << ret << endl;
    while (1)
    {
        ret = avio_read_dir(dir_ctx, &dir_entry);
        cout << ret << endl;

        if (!dir_entry)
            break;
        
        cout << dir_entry->size << " " <<dir_entry->name << endl;

        avio_free_directory_entry(&dir_entry);
    }

    cout << "ok" <<endl;
    return 0;
}