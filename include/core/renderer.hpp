#pragma once

#include <core/frame.hpp>

// Basic renderer class (might get more complicated in the future)
class Renderer {
    public:
        void clear_frame(void);     // Clears current frame
        void render_frame(Frame);   // Renders next frame
};