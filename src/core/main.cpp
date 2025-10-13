#include "core/renderer.hpp"
#include "utils/globals.hpp"
#include <unistd.h>
#include <queue>

Renderer renderer;
bool running = true;

void heartbeat(int interval) {
    int i = 0;
    while (running) {
        i++;
        std::queue<Pixel> pixels{};
        for (int j = 0; j < 256*64; j++) {
            pixels.push(Pixel('#', (i+j)%256));
        }
        Frame my_frame{256, 64, pixels};
        renderer.render_frame(my_frame);
        usleep(interval);
    }
}

void ready() { heartbeat(constants::FRAME_INTERVAL); }

int main() {
    ready();
    return 0;
}