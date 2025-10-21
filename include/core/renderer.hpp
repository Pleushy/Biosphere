#pragma once

#include <core/frame.hpp>
#include <queue>

/** Basic renderer class (might get more complicated in the future)
*/
class Renderer {
    private:
        // Queue of frames to render
        std::queue<Frame> render_queue;

        // Clears output
        void clear(void);

        // Renders frame
        void render_frame(Frame frame);
    public:
        // Adds a frame into the render queue
        void add_frame(Frame frame);
        
        // Adds multiple frames into the render queue
        void add_animation(std::queue<Frame> queue);

        // Updates the render queue
        void update();
};