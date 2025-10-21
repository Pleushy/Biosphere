#include "core/renderer.hpp"
#include "core/frame.hpp"
#include <cstdio>
#include <iostream>
#include <queue>
#include <sstream>
#include <unistd.h>

void Renderer::clear() {
    // Not compatible with every terminal
    printf("\e[1;1H\e[2J");
}

void Renderer::render_frame(Frame frame) {
    // Make sure the frame is valid
    if (!frame::is_valid(frame)) {
        std::cerr << "Invalid frame" << std::endl;
        return;
    }

    // Clear output
    clear();

    // Create a buffer
    std::stringstream buffer;

    // Add all pixels to buffer
    for (int i = 0; i < frame.size.y; i++) {
        for (int j = 0; j < frame.size.x; j++) {
            Pixel current = frame.pixels.front();
            frame.pixels.pop();
            if (!current.rendered) {
                buffer << " ";
                continue;
            }
            buffer << "\e[38;5;" << current.color << "m" << current.ch;
        }
        buffer << std::endl;
    }

    // Print buffer
    std::cout << buffer.str() << std::flush;
}

void Renderer::add_frame(Frame frame) {
    render_queue.push(frame);
}

void Renderer::add_animation(std::queue<Frame> queue) {
    while (!queue.empty()) {
        add_frame(queue.front());
        queue.pop();
    }
}

void Renderer::update() {
    if (render_queue.empty()) return;
    
    Frame next_frame = render_queue.front();
    render_queue.pop();

    render_frame(next_frame);
}