#ifndef _MEDIA_CONTROL_FS_H
#define _MEDIA_CONTROL_FS_H

#include "pch.h"

class FileStreamer {
    int                      buffer_size = 1024 * 1024;
    int                      offset          = 0;
    int                      buffer_offset   = 0;
    int                      buffer_end      = 0;
    int                      file_size       = -1;
    bool                     from_win_stream = false;
    char*                    buffer;
    win::Buffer              win_buffer;
    win::IRandomAccessStream stream;
    std::ifstream            file;

  public:
    FileStreamer(std::string path);
    FileStreamer(win::IRandomAccessStream stream);

  private:
    void read();

  public:
    void write(uWS::HttpResponse<false>* res);
};

#endif