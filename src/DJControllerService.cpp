#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    //Your implementation here 
    bool found = cache.contains(track.get_title());
    if (found){
        cache.get(track.get_title());
        return 1;
    }
    else{
        PointerWrapper<AudioTrack> clone = track.clone();
        if(!clone){
            std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" clone is nullptr" << std::endl;
            return 0; 
        }
        clone->load();
        clone->analyze_beatgrid();
        bool eviction_occurred = cache.put(std::move(clone));
        if (eviction_occurred) {
            return -1;
        } 
        else {
            return 0;
        }
    }
}





void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    AudioTrack* result_ptr = cache.get(track_title);
    if (result_ptr) {
        return result_ptr;
    }
    return nullptr;
}

