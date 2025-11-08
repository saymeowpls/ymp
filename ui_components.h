#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "music_service.h"
#include <ftxui/component/component.hpp>
#include <memory>

class MusicPlayerUI {
private:
    std::shared_ptr<MusicService> musicService_;
    ftxui::Component searchInput_;
    ftxui::Component trackList_;
    ftxui::Component playerControls_;
    ftxui::Component mainContainer_;
    
    std::vector<Track> searchResults_;
    std::vector<std::string> trackListItems_;
    int selectedTrackIndex_;
    std::string currentStatus_;
    
public:
    MusicPlayerUI(std::shared_ptr<MusicService> musicService);
    void initializeUI();
    void run();
    
private:
    void updateTrackList();
    void onSearch(const std::string& query);
    void onTrackSelected(int index);
    void onPlayPause();
    void onNext();
    void onPrevious();
    
    ftxui::Element renderPlayerControls();
    ftxui::Element renderTrackList();
    ftxui::Element renderStatusBar();
};

#endif
