#pragma once

#include <queue>

// Pixel data structure
struct Pixel {
    bool rendered;              // Is rendered
    char ch;                    // Char to display
    int color;                  // ANSI color code
};

// Frame data structure
struct Frame {
    int width;                  // Width of frame
    int height;                 // Height of frame
    int zindex;                 // Z-index of frame (layering)
    std::queue<Pixel> pixels;   // Pixels
};

// Checks whether the frame is a valid frame
bool is_frame_valid(Frame);

// Combines multiple frames into one
Frame combine_frames(std::queue<Frame>);

// Creates a rectangular frame
Frame create_rect(int width, int height, int zindex, char ch, int color);