#ifndef _MEDIA_CONTROL_STATUS_H
#define _MEDIA_CONTROL_STATUS_H
#include "pch.h"

struct MediaControlStatus {
    std::wstring                      title;
    std::wstring                      artist;
    std::wstring                      album;
    win::IRandomAccessStreamReference thumbnail;
    bool                              isPlaying;
    std::string                       stringify();
    void                              update(win::GlobalSystemMediaTransportControlsSessionMediaProperties prop,
                                             win::GlobalSystemMediaTransportControlsSessionPlaybackInfo    playbackInfo);
};
#endif