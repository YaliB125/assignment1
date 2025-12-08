#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include "Playlist.h" // added
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
 : playlist(playlist), library() {} 

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    for (AudioTrack* track : library) delete track;
    library.clear();
    
    for (const SessionConfig::TrackInfo& info : library_tracks) {

        if (info.type == "MP3") {
            library.push_back(new MP3Track(
                info.title,
                info.artists,
                info.duration_seconds,
                info.bpm,
                info.extra_param1,
                info.extra_param2
            ));

        }

        if (info.type == "WAV") {
            library.push_back(new WAVTrack(
                info.title,
                info.artists,
                info.duration_seconds,
                info.bpm,
                info.extra_param1,
                info.extra_param2
            ));
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded\n";

}

         

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    AudioTrack* found_track = playlist.find_track(track_title);
    if (found_track){
        return found_track;
    } 
    return nullptr; // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist= Playlist(playlist_name);
    for (size_t  i = 0; i < track_indices.size(); i++) {
        int inputIndex = track_indices[i]; 
        if (inputIndex < 1 || inputIndex > (int)library.size()){
            std::cout << "[WARNING] Invalid track index:"<< i <<std::endl;
        }
        else{
             AudioTrack* track = library[inputIndex - 1];
             PointerWrapper<AudioTrack> cloned_track = track->clone();
             AudioTrack* raw_ptr = cloned_track.get();
             if(!raw_ptr){
                std::cout << "[ERROR] Cloned Track is nullptr:"<<std::endl;
             }
             else{
                cloned_track->load();
                cloned_track->analyze_beatgrid();
                playlist.add_track(cloned_track.release());
             }
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " ("<<  playlist.get_track_count() << " tracks)"<<std::endl;
}
DJLibraryService::~DJLibraryService() {  
    for (AudioTrack* track : library) {
        delete track;
    } 
library.clear();
}

DJLibraryService::DJLibraryService(const DJLibraryService& other)
    : playlist(other.playlist), library() 
{
    for (const auto* track : other.library) {
        if (track) {
            library.push_back(track->clone().release());
        }
    }
}

DJLibraryService& DJLibraryService::operator=(const DJLibraryService& other) {    
    if (this != &other) {
        for (AudioTrack* track : library) {
            delete track;
        }
        library.clear();
        playlist = other.playlist;
        for (const auto* track : other.library) {
            if (track) {
                library.push_back(track->clone().release());
            }
        }
    }
    return *this;
}
DJLibraryService::DJLibraryService(DJLibraryService&& other) noexcept
    : playlist(std::move(other.playlist)), library(std::move(other.library)) {}

DJLibraryService& DJLibraryService::operator=(DJLibraryService&& other) noexcept {
    if (this != &other) {
        for (AudioTrack* track : library) {
            delete track;
        }
        library.clear();
        playlist = std::move(other.playlist);
        library = std::move(other.library); 
    }
    return *this;
}


/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */

std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles; 
    std::vector<AudioTrack*> track_titles = playlist.getTracks();
    for(AudioTrack* track: track_titles){
        titles.push_back(track->get_title());
    }
    return titles; 
}


