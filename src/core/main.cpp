#include "core/renderer.hpp"
#include "core/frame.hpp"
#include "utils/globals.hpp"
#include "utils/bio.hpp"
#include <unistd.h>
#include <queue>
#include <optional>

Bio bio;
Renderer renderer;
bool running = true;

void heartbeat(const int interval) {
    while (running) {
        std::optional<std::queue<Frame>> final_frames = bio.execute("assets/animation.bio");

        if (final_frames.has_value()) {
            renderer.add_animation(final_frames.value());
        }

        renderer.update();
        usleep(interval);
    }
}

void ready() { heartbeat(constants::FRAME_INTERVAL); }

int main() {
    ready();
    return 0;
}