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
        AudioTrack* ptr = clone.get();//ask
        //AudioTrack* ptr = clone.get();//ask
        if(!clone){
            std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" clone is nullptr" << std::endl;
            return -1; //ask: what to return
        }
        // ptr->load();
        // ptr->analyze_beatgrid();
        clone->load();
        clone->analyze_beatgrid();
        //PointerWrapper<AudioTrack> new_ptr(ptr);//ask
        //PointerWrapper<AudioTrack> new_ptr = make_pointer_wrapper<AudioTrack>(*ptr);//ask
        bool eviction_occurred = cache.put(std::move(clone));
        //new_ptr.reset();
        if (eviction_occurred) {
            return -1;
        } 
        else {
            return 0;
        }
        
    }
    //return 0;
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
