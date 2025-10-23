#pragma once

#include <queue>
#include <optional>
#include "utils/vec2.hpp"

// Pixel data structure
struct Pixel {
    bool rendered;              // Is rendered
    char ch;                    // Char to display
    int color;                  // ANSI color code
};

// Frame data structure
struct Frame {
    Vec2 size;                  // Absolute size of frame
    Vec2 position;              // Absolute position of frame (from top left)
    int zindex;                 // Layering index of frame
    std::queue<Pixel> pixels;   // Pixels
};

// Animation type definition
typedef std::queue<Frame> Animation;

// Namespace for frame functions
namespace frame {
    // Checks whether the frame is a valid frame
    bool is_valid(Frame frame);

    // Attempts to combine multiple frames into one
    std::optional<Frame> combine(std::vector<Frame> frames, int zindex);

    // Attempts to combine multiple animations into one
    std::optional<Animation> combine_animations(std::vector<Animation> animations, int zindex);

    // Attempts to combine multiple animations and multiple frames into one
    std::optional<Animation> combine_animations_and_frames(std::vector<Frame> frames, std::vector<Animation> animations, int frame_zindex, int animation_zindex, int final_zindex);
}

// Namespace for draw functions
namespace draw {
    // Creates a blank frame
    Frame blank(Vec2 size);

    // Creates a rectangular frame
    Frame rect(Vec2 size, Vec2 position, int zindex, char ch, int color);
}