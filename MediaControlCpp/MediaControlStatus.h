#ifndef _MEDIA_CONTROL_STATUS_H
#define _MEDIA_CONTROL_STATUS_H
#include "pch.h"

struct MediaControlStatus {
    std::wstring                      title = L"Some Random Song";
    std::wstring                      artist = L"Some Random Artist";
    std::wstring                      album  = L"Some Random Album";
    win::IRandomAccessStreamReference thumbnail = nullptr;
    bool                              isPlaying = false;
    int                               duration = 0;
    int                               position = 0;
    std::string                       stringify();
    void                              update(win::GlobalSystemMediaTransportControlsSessionMediaProperties    prop,
                                             win::GlobalSystemMediaTransportControlsSessionPlaybackInfo       playbackInfo);
};
#endif