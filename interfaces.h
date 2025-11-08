#ifndef INTERFACES_H
#define INTERFACES_H

#include "models.h"
// #include <memory>
#include <map>
#include <string>

class IHttpClient {
public:
    virtual ~IHttpClient() = default;
    virtual std::string get(const std::string& url, 
                           const std::map<std::string, std::string>& headers = {}) = 0;
    virtual std::string post(const std::string& url, 
                            const std::string& body,
                            const std::map<std::string, std::string>& headers = {}) = 0;
};

class IMusicApiClient {
public:
    virtual ~IMusicApiClient() = default;
    virtual std::vector<Track> searchTracks(const std::string& query) = 0;
    virtual std::vector<Album> getAlbums(const std::vector<std::string>& albumIds) = 0;
    virtual std::vector<Playlist> getUserPlaylists() = 0;
    virtual Track getTrack(const std::string& trackId) = 0;
};

class IAudioPlayer {
public:
    virtual ~IAudioPlayer() = default;
    virtual void play(const Track& track) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;
    virtual void setVolume(int volume) = 0;
};

#endif
