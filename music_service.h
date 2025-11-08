#ifndef MUSIC_SERVICE_H
#define MUSIC_SERVICE_H

#include "interfaces.h"
#include <memory>

class MusicService {
private:
    std::unique_ptr<IMusicApiClient> apiClient_;
    std::unique_ptr<IAudioPlayer> audioPlayer_;
    std::vector<Track> currentPlaylist_;
    size_t currentTrackIndex_;
    PlayerState currentState_;
    
public:
    MusicService(std::unique_ptr<IMusicApiClient> apiClient, 
                std::unique_ptr<IAudioPlayer> audioPlayer);
    
    void playTrack(const std::string& trackId);
    void playPlaylist(const std::vector<Track>& tracks, size_t startIndex = 0);
    void playNext();
    void playPrevious();
    void togglePause();
    
    std::vector<Track> search(const std::string& query);
    std::vector<Playlist> getPlaylists();
    std::vector<Album> getAlbums(const std::vector<std::string>& albumIds);
    
    Track getCurrentTrack() const;
    PlayerState getPlayerState() const;
};

#endif
