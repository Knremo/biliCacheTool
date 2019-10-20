#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import ffmpeg
import os
import sys
import json


def bilicachetool(folder):
    cache_folder = folder
    copy_dir = os.getcwd()

    if not os.path.exists(cache_folder):
        print("path error\n")
        sys.exit(1)
    
    # check if parts lost
    pdirs = os.listdir(cache_folder)
    #TODO:
    # pszie = len(pdirs)
    # for each in range(1, pszie+1):
    #     if str(each) in pdirs:
    #         continue
        
    for each in pdirs:
        pdir = os.path.join(cache_folder, each)
        #print(pdir)
        entry_path = os.path.join(pdir, 'entry.json')
        danmaku_path = os.path.join(pdir, 'danmaku.xml')
        if not os.path.isfile(entry_path):
            print('have no entry.json !!!')
        else:
            with open(entry_path, 'r', encoding='utf-8') as entry_content:
                entry_content_load = json.load(entry_content)
            #print(entry_content_load['page_data']['page'])
            title = entry_content_load['title']
            part_name = entry_content_load['page_data']['part']
            #part_page = entry_content_load['page_data']['page']

            #TODO:

        if not os.path.isfile(danmaku_path):
            print('have no danmaku.xml !!!')
            #TODO:

        typedir = os.listdir(pdir)
        if  typedir is not []:
            filepath = os.path.join(pdir, typedir[0])

            videopath = os.path.join(filepath, 'video.m4s')
            audiopath = os.path.join(filepath, 'audio.m4s')

            if os.path.isfile(videopath) and os.path.isfile(audiopath):
                outputdir = os.path.join(copy_dir, title)
                if not os.path.exists(outputdir):
                    os.makedirs(outputdir)
                outputpath = os.path.join(copy_dir, title, part_name+'.mp4')
                print(outputpath)
                iv = ffmpeg.input(videopath)
                ia = ffmpeg.input(audiopath)
                (
                    ffmpeg
                    .output(iv, ia, outputpath, codec='copy')
                    .overwrite_output()
                    .run()
                )
    # out = ffmpeg.output(iv, ia, 'v12.mp4', codec='copy')
    # fout = ffmpeg.overwrite_output(out)
    # ffmpeg.run(fout)
    
if __name__ == '__main__':
    path = r"E:\迅雷下载\download"
    for each in os.listdir(path):
        if each == '71119708':
            continue
        #print(each)
        bilicachetool(os.path.join(path, each))