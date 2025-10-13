#pragma once

#include <queue>

// Pixel data structure
struct Pixel {
    char ch;                            // Char to display
    int color;                          // ANSI color code
};

// Frame data structure
struct Frame {
    int width;                          // Width of frame
    int height;                         // Height of frame
    std::queue<Pixel> pixels;           // Pixel queue
};

class Renderer {
    private:
        void clear_frame(void);         // Clears frame
    public:
        void render_frame(Frame);       // Renders next frame
};