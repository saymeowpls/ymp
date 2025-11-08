#ifndef YANDEX_API_CLIENT_H
#define YANDEX_API_CLIENT_H

#include "interfaces.h"
#include <memory>

class YandexMusicApiClient : public IMusicApiClient {
private:
    std::string authToken_;
    std::string baseUrl_;
    std::unique_ptr<IHttpClient> httpClient_;

public:
    YandexMusicApiClient(const std::string& authToken, 
                        std::unique_ptr<IHttpClient> httpClient);
    
    std::vector<Track> searchTracks(const std::string& query) override;
    std::vector<Album> getAlbums(const std::vector<std::string>& albumIds) override;
    std::vector<Playlist> getUserPlaylists() override;
    Track getTrack(const std::string& trackId) override;
    
private:
    // Вспомогательные методы...
};

#endif
