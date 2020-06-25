#include "MediaControlStatus.h"

std::string MediaControlStatus::stringify()
{
    using namespace rapidjson;
    StringBuffer                  sb;
    Writer<StringBuffer, UTF16<>> writer(sb);
    writer.StartObject();
    writer.String(L"title");
    writer.String(title.c_str(), title.size());
    writer.String(L"artist");
    writer.String(artist.c_str(), artist.size());
    writer.String(L"album");
    writer.String(album.c_str(), album.size());
    writer.String(L"playing");
    writer.Bool(isPlaying);
    writer.String(L"duration");
    writer.Int(duration);
    writer.String(L"position");
    writer.Int(position);
    writer.EndObject();
    return sb.GetString();
}

void MediaControlStatus::update(win::GlobalSystemMediaTransportControlsSessionMediaProperties prop,
                                win::GlobalSystemMediaTransportControlsSessionPlaybackInfo    playbackInfo)
{
    title     = prop.Title();
    artist    = prop.Artist();
    album     = prop.AlbumTitle();
    thumbnail = prop.Thumbnail();
    isPlaying = playbackInfo.PlaybackStatus() == win::GlobalSystemMediaTransportControlsSessionPlaybackStatus::Playing;
}