#include <iostream>
#include <memory>
#include <vector>

// Include all our classes
#include "AudioTrack.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include "Playlist.h"
// Phase 4 orchestrator
#include "DJSession.h"
#include "DJLibraryService.h"
#include "DJControllerService.h"
#include "MixingEngineService.h"
#include "PointerWrapper.h"
/**
 * DJ Track Session Manager - Test Program
 * 
 * This program demonstrates the assignment and tests student implementations.
 * As students complete each phase, more functionality will work correctly.
 * 
 * ⚠️  EXPECT MEMORY LEAKS AND CRASHES INITIALLY! ⚠️
 * Students must fix the issues to make this program run cleanly.
 */

bool del = false; // hint: what is the purpose of this variable? how it changes the ownership semantics?
void test_phase_1_memory_leaks() {
    
    std::cout << "\n======== PHASE 1: MEMORY LEAK TESTING ========" << std::endl;
    std::cout << "Testing the broken Playlist class..." << std::endl;

    // Create some tracks (these will leak if AudioTrack Rule of 5 isn't implemented)
    std::cout << "Creating tracks..." << std::endl;
    AudioTrack* mp3 = new MP3Track("Sunset Lover", {"Petit Biscuit"}, 180, 120, 320);
    AudioTrack* wav = new WAVTrack("Strobe", {"Deadmau5"}, 645, 128, 44100, 16);

    //Create playlist and add tracks (this will leak nodes if destructor is broken)
    std::cout << "Creating playlist..." << std::endl;
    Playlist* my_playlist = new Playlist("Chill House Mix");

    my_playlist->add_track(mp3);
    my_playlist->add_track(wav);
    my_playlist->display();

    // Remove a track (this will leak if remove_track is broken)
    std::cout << "Removing track..." << std::endl;
    my_playlist->remove_track("Strobe");
    my_playlist->display();

    std::cout << "Cleaning up..." << std::endl;
    delete my_playlist;  // Should clean up playlist nodes
    if(del){
        delete mp3;          // Should clean up AudioTrack data
        delete wav;          // Should clean up AudioTrack data
    }

    std::cout << "Phase 1 test complete. Check for memory leaks!\n" << std::endl;
}

void test_phase_2_rule_of_5() {
    std::cout << "\n======== PHASE 2: RULE OF 5 TESTING ========" << std::endl;
    std::cout << "Testing AudioTrack copy and move operations..." << std::endl;

    try {
        // Test copy constructor
        std::cout << "Testing copy constructor..." << std::endl;
        MP3Track original("Original Track", {"Test Artist"}, 200, 128, 256);
        MP3Track copied = original;  // Should call copy constructor

        std::cout << "Original: " << original.get_title() << std::endl;
        std::cout << "Copied: " << copied.get_title() << std::endl;

        // Test copy assignment
        std::cout << "\nTesting copy assignment..." << std::endl;
        MP3Track assigned("Temporary Track", {"Temp Artist"}, 100, 120, 128);
        assigned = original;  // Should call copy assignment

        std::cout << "Assigned: " << assigned.get_title() << std::endl;

        // Test move constructor
        std::cout << "\nTesting move constructor..." << std::endl;
        MP3Track moved = std::move(original);  // Should call move constructor

        std::cout << "Moved: " << moved.get_title() << std::endl;

        std::cout << "Phase 2 test complete!\n" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        std::cout << "This indicates Rule of 5 implementation issues!\n" << std::endl;
    }
}


    void test_DJControllerService() {
        std::cout << "\n===== TEST: DJControllerService =====\n";

        DJControllerService controller(3);  // cache size 3

        MP3Track track1("Track 1", {"Artist A"}, 180, 120, 320);
        WAVTrack track2("Track 2", {"Artist B"}, 200, 128, 44100, 16);

        // Load track1 into cache
        int result = controller.loadTrackToCache(track1);
        std::cout << "Loading 'Track 1': " << result << std::endl;

        // Load track2 into cache
        result = controller.loadTrackToCache(track2);
        std::cout << "Loading 'Track 2': " << result << std::endl;

        // Display cache status
        controller.displayCacheStatus();

        // Attempt to get track from cache
        AudioTrack* ptr = controller.getTrackFromCache("Track 1");
        if (ptr) std::cout << "Retrieved from cache: " << ptr->get_title() << std::endl;
        else std::cout << "Track not found in cache\n";
    }

    void test_LRUCache_basic()
{
    std::cout << "\n===== TEST: LRUCache basic insert & hit =====\n";

    LRUCache cache(3);

    MP3Track t1("Track 1", {"Artist A"}, 180, 120, 320);
    WAVTrack t2("Track 2", {"Artist B"}, 200, 128, 44100, 16);

    // Insert T1
    bool evicted = cache.put(t1.clone());
    std::cout << "After inserting Track 1: evicted=" << evicted
              << " size=" << cache.size() << " (expect 1)\n";

    // Insert T2
    evicted = cache.put(t2.clone());
    std::cout << "After inserting Track 2: evicted=" << evicted
              << " size=" << cache.size() << " (expect 2)\n";

    cache.displayStatus();

    // HIT: access Track 1 (MRU refresh)
    AudioTrack* p = cache.get("Track 1");
    std::cout << "cache.get(\"Track 1\") returned "
              << (p ? p->get_title() : std::string("nullptr")) << "\n";

    cache.displayStatus();
}



void test_phase_3() {
    std::cout << "\n======== PHASE 3: SMART POINTER TESTING ========" << std::endl;
    std::cout << "Testing DJUniquePtr implementation..." << std::endl;

    try {
        // Test basic construction
        std::cout << "Testing DJUniquePtr construction..." << std::endl;
        PointerWrapper<MP3Track> smart_track(new MP3Track("Smart Track", {"AI Artist"}, 240, 130, 320));

        // Test access operators (will be TODO messages initially)
        std::cout << "\nTesting access operators..." << std::endl;
        if (smart_track) {
            std::cout << "Smart pointer contains: " << smart_track->get_title() << std::endl;
            std::cout << "BPM: " << (*smart_track).get_bpm() << std::endl;
        }

        // Test move operations
        std::cout << "\nTesting move operations..." << std::endl;
        PointerWrapper<MP3Track> moved_track = std::move(smart_track);
        std::cout << "Original pointer after move: " << (smart_track ? "still valid" : "null") << std::endl;
        std::cout << "Moved pointer: " << (moved_track ? "valid" : "null") << std::endl;

        // Test reset
        std::cout << "\nTesting reset..." << std::endl;
        moved_track.reset(new MP3Track("Reset Track", {"Reset Artist"}, 180, 125, 256));

        // Test release
        std::cout << "\nTesting release..." << std::endl;
        MP3Track* raw_ptr = moved_track.release();
        std::cout << "Released pointer: " << (raw_ptr ? raw_ptr->get_title() : "null") << std::endl;

        // Manual cleanup since we released
        delete raw_ptr;

        std::cout << "Phase 3 test complete!\n" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        std::cout << "This indicates DJUniquePtr implementation issues!\n" << std::endl;
    }
}

void demonstrate_polymorphism() {
    std::cout << "\n======== POLYMORPHISM DEMONSTRATION ========" << std::endl;
    std::cout << "Showing virtual function calls with different track types...\n" << std::endl;

    // Create different track types
    std::vector<std::unique_ptr<AudioTrack>> tracks;
    tracks.push_back(std::unique_ptr<AudioTrack>(new MP3Track("MP3 Example", {"Artist 1"}, 200, 128, 320)));
    tracks.push_back(std::unique_ptr<AudioTrack>(new WAVTrack("WAV Example", {"Artist 2"}, 180, 130, 44100, 16)));

    // Call virtual functions polymorphically
    for (auto& track : tracks) {
        std::cout << "Track: " << track->get_title() << std::endl;
        track->load();                    // Virtual function call
        track->analyze_beatgrid();        // Virtual function call
        std::cout << "Quality: " << track->get_quality_score() << std::endl;  // Virtual function call

        // Test cloning
        auto cloned = track->clone();
        if (cloned) {
            std::cout << "Cloned: " << cloned->get_title() << std::endl;
        }
        std::cout << std::endl;
    }
}

void test_LRUCache_eviction()
{
    std::cout << "\n===== TEST: LRUCache – eviction =====\n";

    LRUCache cache(2);

    MP3Track t1("Track 1", {"Artist A"}, 180, 120, 320);
    MP3Track t2("Track 2", {"Artist B"}, 200, 125, 256);
    MP3Track t3("Track 3", {"Artist C"}, 210, 130, 192);

    // Fill cache with T1, T2
    cache.put(t1.clone());
    cache.put(t2.clone());
    std::cout << "After inserting Track 1 & 2:\n";
    cache.displayStatus();

    // Access T1 → it becomes MRU, T2 becomes LRU
    cache.get("Track 1");
    std::cout << "After touching Track 1 (should be MRU now):\n";
    cache.displayStatus();

    // Insert T3 → should evict T2 (the LRU)
    bool evicted = cache.put(t3.clone());
    std::cout << "Inserting Track 3, evicted=" << evicted << " (expect 1 / true)\n";
    cache.displayStatus();

    // Verify T2 is gone, T1 and T3 remain
    std::cout << "get(\"Track 2\") -> "
              << (cache.get("Track 2") ? "FOUND (BUG)" : "nullptr (OK)") << "\n";
}

void test_DJControllerService_eviction()
{
    std::cout << "\n===== TEST: DJControllerService – eviction behavior =====\n";

    DJControllerService controller(2);  // small cache to force eviction

    MP3Track t1("Track 1", {"A"}, 180, 120, 320);
    MP3Track t2("Track 2", {"B"}, 200, 122, 256);
    MP3Track t3("Track 3", {"C"}, 210, 124, 192);

    int result;

    result = controller.loadTrackToCache(t1);  // MISS, no eviction
    std::cout << "Load T1: " << result << " (expect 0)\n";

    result = controller.loadTrackToCache(t2);  // MISS, no eviction
    std::cout << "Load T2: " << result << " (expect 0)\n";

    controller.displayCacheStatus();

    // Access T1 so it becomes MRU, making T2 LRU
    result = controller.loadTrackToCache(t1);  // HIT
    std::cout << "Reload T1 (HIT): " << result << " (expect 1)\n";

    // Now add T3 – cache full → MISS with eviction ⇒ -1
    result = controller.loadTrackToCache(t3);
    std::cout << "Load T3 (eviction should happen): " << result << " (expect -1)\n";

    controller.displayCacheStatus();

    // Check who got evicted
    auto* p1 = controller.getTrackFromCache("Track 1");
    auto* p2 = controller.getTrackFromCache("Track 2");
    auto* p3 = controller.getTrackFromCache("Track 3");

    std::cout << "Cache contains T1: " << (p1 ? "YES" : "NO") << "\n";
    std::cout << "Cache contains T2: " << (p2 ? "YES" : "NO (expected)") << "\n";
    std::cout << "Cache contains T3: " << (p3 ? "YES (expected)" : "NO") << "\n";
}


int main(int argc, char* argv[]) {    
    /**
     * Command-line argument parsing
     * - If "-I" is provided as the first argument, run interactive DJ software
     * - If "-A" is provided as the second argument, enable play_all mode
     */
    bool run_software = false;
    bool play_all = false;
    if (argc > 1 && std::string(argv[1]) == "-I") {
        run_software = true;
    }

    if (argc > 2 && std::string(argv[2]) == "-A") {
        play_all = true;
    }

    if (run_software) {
        std::cout << "\n============= RUNNING INTERACTIVE SOFTWARE =============" << std::endl;
        DJSession live_session("Interactive Session", play_all);
        live_session.simulate_dj_performance();
        std::cout << "============= INTERACTIVE SESSION ENDED =============\n" << std::endl;
    } else {
        std::cout << "==================================================" << std::endl;
        std::cout << "    DJ TRACK SESSION MANAGER - TEST PROGRAM" << std::endl;
        std::cout << "==================================================" << std::endl;
        std::cout << "This program tests all phases of the assignment." << std::endl;
        std::cout << "Initially, many features will be incomplete!" << std::endl;
        std::cout << "==================================================" << std::endl;
        
        // Test each phase individually
        test_phase_1_memory_leaks();
        test_phase_2_rule_of_5();
        test_phase_3();
        // test_DJControllerService();
        // test_LRUCache_basic();
        //test_LRUCache_eviction();
        // demonstrate_polymorphism();
        //test_DJControllerService_eviction();
        std::cout << "\n(Set 'run_software' to true in main.cpp to run the full interactive session.)\n" << std::endl;
    }
    return 0;
}