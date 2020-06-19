#include "FileStreamer.h"

FileStreamer::FileStreamer(std::string path) : win_buffer(nullptr), from_win_stream(false)
{
    buffer = (char*)malloc(sizeof(char) * 1024 * 1024);
    file.open(path, std::ios::binary);
    file.seekg(0, file.end);
    file_size = file.tellg();
    read();
}
FileStreamer::FileStreamer(win::IRandomAccessStream _stream) : win_buffer(buffer_size), from_win_stream(true), stream(_stream)
{
    file_size = stream.Size();
    read();
}

void FileStreamer::read()
{
    int size = std::min(buffer_size, file_size - offset);
    if (from_win_stream) {
        stream.Seek(offset);
        stream.ReadAsync(win_buffer, size, win::InputStreamOptions::None).get();
        buffer = (char*)win_buffer.data();
    }
    else {
        file.seekg(offset, file.beg);
        file.read(buffer, size);
    }
    buffer_offset = offset;
    buffer_end    = buffer_offset + size;
}

void FileStreamer::write(uWS::HttpResponse<false>* res)
{
    offset = res->getWriteOffset();
    if (offset >= buffer_end) read();
    res->writeHeader("Content-Type", "image/jpeg");
    std::string_view cache(buffer + offset - buffer_offset, (size_t)buffer_end - offset);
    if (res->tryEnd(cache, file_size).first) { 
        std::cout << "Write succeeded" << std::endl;
    }
    else {
        std::cout << "Request didn't end" << std::endl;
        res->onWritable([&](int offset) {
               std::cout << "writing again" << std::endl;
               write(res);
               return false;
           })
            ->onAborted([]() { std::cout << "Request aborted" << std::endl; });
    }
}