#ifdef __cplusplus
extern "C"
{
#endif

#include "libavformat/avformat.h"

#ifdef __cplusplus
}
#endif

#include <iostream>
#include <string>

using namespace std;

/**
 * return 0 if muxing succeed.
*/
int FFmuxing(const string &vf, const string &af, const string &outputfile)
{
    AVFormatContext *video_fmtctx = NULL, *audio_fmtctx = NULL;
    string video_filename = vf, audio_filename = af;
    int ret;

    // 读取视频和音频文件
    if ((ret = avformat_open_input(&video_fmtctx,
                                   video_filename.c_str(),
                                   NULL, NULL)) < 0)
    {
        cerr << "[FFmpeg_muxing] Could not open video file " << video_filename << endl;
        return 1;
    }
    if ((ret = avformat_open_input(&audio_fmtctx,
                                   audio_filename.c_str(),
                                   NULL, NULL)) < 0)
    {
        cerr << "[FFmpeg_muxing] Could not open audio file " << audio_filename << endl;
        return 1;
    }

    // 创建输出的AVFormatContext
    AVFormatContext *output_fmtctx = NULL;
    string output_filename = outputfile;
    // 分配内存
    avformat_alloc_output_context2(&output_fmtctx, NULL, NULL, output_filename.c_str());
    if (!output_fmtctx)
    {
        cerr << "[FFmpeg_muxing] Could not create output context" << endl;
        return 1;
    }

    // 获取流信息
    if ((ret = avformat_find_stream_info(video_fmtctx, NULL)) < 0)
    {
        cerr << "[FFmpeg_muxing] Failed to retrieve input video stream information" << endl;
        return 1;
    }
    if ((ret = avformat_find_stream_info(audio_fmtctx, NULL)) < 0)
    {
        cerr << "[FFmpeg_muxing] Failed to retrieve input audio stream information" << endl;
        return 1;
    }

    // 每个文件含有的流的个数
    // int video_stream_numbers = video_fmtctx->nb_streams;
    // int audio_stream_numbers = audio_fmtctx->nb_streams;
    // cout << "video has " << video_stream_numbers << " stream(s), "
    //      << "audio has " << audio_stream_numbers << " stream(s)." << endl;

    AVStream *video_stream = video_fmtctx->streams[0],      // 输入视频流
             *audio_stream = audio_fmtctx->streams[0],      // 输入音频流
             *voutput_stream,                               // 输出视频流
             *aoutput_stream;                               // 输出音频流

    // 音视频的编码器参数信息
    AVCodecParameters *video_codecpar = video_stream->codecpar,
                      *audio_codecpar = audio_stream->codecpar;

    // 向输出添加流
    voutput_stream = avformat_new_stream(output_fmtctx, NULL);
    // 将输入流的编码器信心拷贝到新添加的流
    avcodec_parameters_copy(voutput_stream->codecpar, video_codecpar);
    aoutput_stream = avformat_new_stream(output_fmtctx, NULL);
    avcodec_parameters_copy(aoutput_stream->codecpar, audio_codecpar);

    // 输出的流信息
    //av_dump_format(output_fmtctx, 0, output_filename.c_str(), 1);

    // 创建输出文件，根据文件后缀确定格式
    ret = avio_open2(&output_fmtctx->pb, output_filename.c_str(), AVIO_FLAG_WRITE, NULL, NULL);
    if (ret < 0)
    {
        cerr << "[FFmpeg_muxing] Could not open output file " << output_filename << endl;
        return 1;
    }

    // 第一步，写入头部
    ret = avformat_write_header(output_fmtctx, NULL);

    av_dump_format(output_fmtctx, 0, output_filename.c_str(), 1);

    // 第二步，用循环读每个packet，换算时间戳，写入packet
    AVPacket packet;
    while (1)
    {
        ret = av_read_frame(video_fmtctx, &packet);
        if (ret < 0)
            break;
        packet.stream_index = 0;
        packet.pts = av_rescale_q_rnd(packet.pts,
                                      video_stream->time_base,
                                      voutput_stream->time_base,
                                      (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        packet.dts = av_rescale_q_rnd(packet.dts,
                                      video_stream->time_base,
                                      voutput_stream->time_base,
                                      (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        packet.duration = av_rescale_q(packet.duration,
                                       video_stream->time_base,
                                       voutput_stream->time_base);
        packet.pos = -1;
        av_interleaved_write_frame(output_fmtctx, &packet);
        av_packet_unref(&packet);
    }

    // 音频流同样
    while (1)
    {
        ret = av_read_frame(audio_fmtctx, &packet);
        if (ret < 0)
            break;
        packet.stream_index = 1;
        packet.pts = av_rescale_q_rnd(packet.pts,
                                      audio_stream->time_base,
                                      aoutput_stream->time_base,
                                      (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        packet.dts = av_rescale_q_rnd(packet.dts,
                                      audio_stream->time_base,
                                      aoutput_stream->time_base,
                                      (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        packet.duration = av_rescale_q(packet.duration,
                                       audio_stream->time_base,
                                       aoutput_stream->time_base);
        packet.pos = -1;
        av_interleaved_write_frame(output_fmtctx, &packet);
        av_packet_unref(&packet);
    }

    // 最后写入尾部
    av_write_trailer(output_fmtctx);

    // 关闭各种资源
    avformat_close_input(&video_fmtctx);
    avformat_close_input(&audio_fmtctx);
    avio_closep(&output_fmtctx->pb);
    avformat_free_context(output_fmtctx);

    return 0;
}