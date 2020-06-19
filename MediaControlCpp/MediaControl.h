#ifndef _MEDIA_CONTROL_H
#define _MEDIA_CONTROL_H

#include "MediaControlStatus.h"
#include "pch.h"
#define __APPCOMMAND_VOLUME_MUTE 0x80000
#define __APPCOMMAND_VOLUME_UP 0xA0000
#define __APPCOMMAND_VOLUME_DOWN 0x90000

class MediaControl {
  private:
    win::GlobalSystemMediaTransportControlsSessionManager         sessionManager;
    win::GlobalSystemMediaTransportControlsSessionMediaProperties currentTrack{nullptr};
    win::GlobalSystemMediaTransportControlsSession                currentSession{nullptr};
    MediaControlStatus                                            status;
    std::mutex                                                    status_mutex;
    HWND                                                          handle;
    bool isSameTrack(win::GlobalSystemMediaTransportControlsSessionMediaProperties one,
                     win::GlobalSystemMediaTransportControlsSessionMediaProperties two);

  public:
    MediaControl(HWND handle);
    win::GlobalSystemMediaTransportControlsSession updateSession();
    void                              updateStatus(bool shouldUpdateSession = false, bool waitForNewTrack = false);
    bool                              play();
    bool                              pause();
    bool                              prev();
    bool                              next();
    win::IRandomAccessStream          getThumbnail();
    std::string                       getStatusJson();
    void                              mute();
    void                              volDown();
    void                              volUp();
};
#endif