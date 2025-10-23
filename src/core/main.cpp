#include "core/renderer.hpp"
#include "core/frame.hpp"
#include "utils/globals.hpp"
#include "utils/bio.hpp"
#include "utils/vec2.hpp"
#include <unistd.h>
#include <queue>
#include <optional>

Bio bio;
Renderer renderer;
bool running = true;

void heartbeat(const int interval) {
    while (running) {
        Frame blank_frame = draw::rect(Vec2{5,5}, Vec2{0,0}, 0, '.', constants::DEFAULT_COLOR);
        std::optional<Animation> anim = bio.execute("assets/animation.bio");
        std::optional<Animation> anim2 = bio.execute("assets/animation2.bio");

        if (anim.has_value() && anim2.has_value()) {
            std::optional<Animation> new_anim = frame::combine_animations_and_frames(
                {blank_frame},
                 {anim.value(), anim2.value()},
                  1,2,0
            );
            renderer.add_animation(new_anim.value());
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