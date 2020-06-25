#include "MediaControl.h"

bool MediaControl::isSameTrack(win::GlobalSystemMediaTransportControlsSessionMediaProperties one,
                               win::GlobalSystemMediaTransportControlsSessionMediaProperties two)
{
    if (!one || !two)
        return false;
    return (one.Title() == two.Title() && one.Artist() == two.Artist() && one.AlbumTitle() == two.AlbumTitle());
}

MediaControl::MediaControl(HWND handle)
    : handle(handle)
{
    sessionManager = win::GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
    updateSession();
}
win::GlobalSystemMediaTransportControlsSession MediaControl::updateSession()
{
    if (sessionManager.GetSessions().Size() == 0)
        sessionManager = win::GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
    currentSession = sessionManager.GetCurrentSession();
    return currentSession;
}
bool MediaControl::updateStatus(bool waitForNewTrack)
{
    if (!updateSession()) return false;
    win::GlobalSystemMediaTransportControlsSessionMediaProperties track{nullptr};
    do {
        track = currentSession.TryGetMediaPropertiesAsync().get();
        if (!waitForNewTrack) break;
    } while (isSameTrack(track, currentTrack));
    currentTrack = track;
    {
        std::lock_guard<std::mutex> lock(status_mutex);
        status.update(currentTrack, currentSession.GetPlaybackInfo());
    }
    return true;
}
bool MediaControl::play()
{
    if (!updateSession()) return false;
    return currentSession.TryPlayAsync().get();
}
bool MediaControl::pause()
{
    if (!updateSession()) return false;
    return currentSession.TryPauseAsync().get();
}
bool MediaControl::prev()
{
    if (!updateSession()) return false;
    return currentSession.TrySkipPreviousAsync().get();
}
bool MediaControl::next()
{
    if (!updateSession()) return false;
    return currentSession.TrySkipNextAsync().get();
}
win::IRandomAccessStream MediaControl::getThumbnail()
{
    std::lock_guard<std::mutex> lock(status_mutex);
    win::IRandomAccessStream    stream;
    if (!status.thumbnail) return nullptr;
    try {        
        stream = status.thumbnail.OpenReadAsync().get().as<win::IRandomAccessStream>();
    }
    catch (...) {
        stream = nullptr;
    }
    return stream;
}
std::string MediaControl::getStatusJson()
{
    std::lock_guard<std::mutex> lock(status_mutex);
    return status.stringify();
}

void MediaControl::mute() { SendMessageW(handle, WM_APPCOMMAND, 0, __APPCOMMAND_VOLUME_MUTE); }

void MediaControl::volDown() { SendMessageW(handle, WM_APPCOMMAND, 0, __APPCOMMAND_VOLUME_DOWN); }

void MediaControl::volUp() { SendMessageW(handle, WM_APPCOMMAND, 0, __APPCOMMAND_VOLUME_UP); }