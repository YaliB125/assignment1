#include "MixingEngineService.h"
#include <iostream>
#include <memory>
#include <cmath>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
 //: active_deck(0),auto_sync(false),bpm_tolerance(0) 
  : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0) //change
{
    // Your implementation here
    decks[0]=nullptr;
    decks[1]=nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout << "[MixingEngineService] Cleaning up decks...." << std::endl;
    for (int i= 0; i<2; i++){
        if (decks[i]!= nullptr){
            delete decks[i];
            decks [i]= nullptr;
        }
    }
    //delete[] decks; //ask what should we delete in the end?
}

MixingEngineService::MixingEngineService(const MixingEngineService& other)
    : active_deck(other.active_deck), 
      auto_sync(other.auto_sync), 
      bpm_tolerance(other.bpm_tolerance) 
{
    for (int i = 0; i < 2; ++i) {
        if (other.decks[i]) {
            decks[i] = other.decks[i]->clone().release();
        } else {
            decks[i] = nullptr;
        }
    }
}

MixingEngineService& MixingEngineService::operator=(const MixingEngineService& other) {
    if (this != &other) {
        for (int i = 0; i < 2; ++i) {
            delete decks[i];
        }
        active_deck = other.active_deck;
        auto_sync = other.auto_sync;
        bpm_tolerance = other.bpm_tolerance;
        for (int i = 0; i < 2; ++i) {
            if (other.decks[i]) {
                decks[i] = other.decks[i]->clone().release();
            } else {
                decks[i] = nullptr;
            }
        }
    }
    return *this;
}

MixingEngineService::MixingEngineService(MixingEngineService&& other) noexcept
    : active_deck(other.active_deck), 
      auto_sync(other.auto_sync), 
      bpm_tolerance(other.bpm_tolerance) 
{
    for (int i = 0; i < 2; ++i) {
        decks[i] = other.decks[i];
        other.decks[i] = nullptr;
    }
}

MixingEngineService& MixingEngineService::operator=(MixingEngineService&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < 2; ++i) {
            delete decks[i];
        }
        active_deck = other.active_deck;
        auto_sync = other.auto_sync;
        bpm_tolerance = other.bpm_tolerance;
        for (int i = 0; i < 2; ++i) {
            decks[i] = other.decks[i];
            other.decks[i] = nullptr; 
        }
    }
    return *this;
    
}

/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;
    PointerWrapper<AudioTrack> cloned_wrapper = track.clone();
    if (cloned_wrapper.get() == nullptr) {
        std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return -1;
    }
    int target_deck = 1 -active_deck;
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;
    if (decks[target_deck] != nullptr) {
        delete decks[target_deck]; 
        decks[target_deck] = nullptr;
    }
    cloned_wrapper->load();
    cloned_wrapper->analyze_beatgrid();
    
    if (decks[active_deck] != nullptr && auto_sync){
        int active_bpm = decks[active_deck]->get_bpm();
        int new_bpm = cloned_wrapper->get_bpm();
        int bound = abs(active_bpm - new_bpm);
        if (bpm_tolerance < bound) {
            sync_bpm(cloned_wrapper);
        }
    }
    decks[target_deck] = cloned_wrapper.release();
    std::cout << "[Load Complete] '" << decks[target_deck]->get_title() << "' is now loaded on deck " << target_deck << std::endl;

    // if (decks[active_deck] != nullptr) {
    //     std::cout << "[Unload] Unloading previous deck " << active_deck << " ('" << decks[active_deck]->get_title() << "')" << "')" << std::endl;
    //     delete decks[active_deck];
    //     decks[active_deck] = nullptr;
    // }
    
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << target_deck << std::endl;
    return target_deck;

}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if (decks[active_deck]== nullptr){
        return false;
    }
    if (track.get() == nullptr){
        return false;
    }
    int active_bpm = decks[active_deck]->get_bpm();
    int new_bpm = (*track).get_bpm();
    int diff = abs(active_bpm-new_bpm);
    if (diff <= bpm_tolerance){
        return true;
    }
    else{
        return false;
    }
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if (decks[active_deck]!= nullptr && track.get() != nullptr){
        int new_bpm = (*track).get_bpm();
        int active_bpm = decks[active_deck]->get_bpm();
        int average_bpm = (active_bpm+new_bpm)/2;
        (*track).set_bpm(average_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << active_bpm << " to " << average_bpm << std::endl;
    }
}
