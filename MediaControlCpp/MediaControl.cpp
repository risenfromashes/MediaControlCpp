#include "MediaControl.h"

bool MediaControl::isSameTrack(win::GlobalSystemMediaTransportControlsSessionMediaProperties one,
                               win::GlobalSystemMediaTransportControlsSessionMediaProperties two)
{
    if (!one || !two)
        return false;
    return (one.Title() == two.Title() && one.Artist() == two.Artist() && one.AlbumTitle() == two.AlbumTitle());
}

MediaControl::MediaControl(HWND handle)
    : handle(handle), sessionManager(win::GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get())
{
    updateSession();
}
win::GlobalSystemMediaTransportControlsSession MediaControl::updateSession()
{
    currentSession = sessionManager.GetCurrentSession();
    return currentSession;
}
void MediaControl::updateStatus(bool shouldUpdateSession, bool waitForNewTrack)
{
    if (shouldUpdateSession) updateSession();
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
}
bool MediaControl::play()
{
    updateSession();
    return currentSession.TryPlayAsync().get();
}
bool MediaControl::pause()
{
    updateSession();
    return currentSession.TryPauseAsync().get();
}
bool MediaControl::prev()
{
    updateSession();
    return currentSession.TrySkipPreviousAsync().get();
}
bool MediaControl::next()
{
    updateSession();
    return currentSession.TrySkipNextAsync().get();
}
win::IRandomAccessStream MediaControl::getThumbnail()
{
    std::lock_guard<std::mutex> lock(status_mutex);
    return status.thumbnail.OpenReadAsync().get().as<win::IRandomAccessStream>();
}
std::string MediaControl::getStatusJson()
{
    std::lock_guard<std::mutex> lock(status_mutex);
    return status.stringify();
}

void MediaControl::mute() { SendMessageW(handle, WM_APPCOMMAND, 0, __APPCOMMAND_VOLUME_MUTE); }

void MediaControl::volDown() { SendMessageW(handle, WM_APPCOMMAND, 0, __APPCOMMAND_VOLUME_DOWN); }

void MediaControl::volUp() { SendMessageW(handle, WM_APPCOMMAND, 0, __APPCOMMAND_VOLUME_UP); }