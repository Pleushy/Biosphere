#include "core/renderer.hpp"
#include "core/frame.hpp"
#include "utils/globals.hpp"
#include <unistd.h>
#include <queue>

Renderer renderer;
bool running = true;

void heartbeat(const int interval) {
    while (running) {
        std::queue<Frame> frames;
        
        
        frames.push(create_rect(32, 16, 2, 'O', 4));

        frames.push(create_rect(64, 32, 1, '#', 6));
        
        frames.push(create_rect(128, 64, 0, '.', 7));
        
        Frame final_frame = combine_frames(frames); 

        renderer.render_frame(final_frame);
        usleep(interval);
    }
}

void ready() { heartbeat(constants::FRAME_INTERVAL); }

int main() {
    ready();
    return 0;
}