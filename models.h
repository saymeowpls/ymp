#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

struct Track {
    std::string id;
    std::string title;
    std::vector<std::string> artists;
    int duration;
    std::string albumId;
};

struct Album {
    std::string id;
    std::string title;
    std::vector<std::string> artists;
    int year;
    std::vector<Track> tracks;
};

struct Playlist {
    std::string id;
    std::string title;
    std::string owner;
    std::vector<Track> tracks;
};

enum class PlayerState {
    Playing,
    Paused,
    Stopped
};

#endif
