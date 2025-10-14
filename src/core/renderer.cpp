#include "core/renderer.hpp"
#include "core/frame.hpp"
#include <cstdio>
#include <iostream>
#include <queue>
#include <sstream>
#include <unistd.h>

void Renderer::clear_frame() {
    // Not compatible with every terminal
    printf("\e[1;1H\e[2J");
}

void Renderer::render_frame(Frame frame) {
    // Make sure the frame is valid
    if (!is_frame_valid(frame)) {
        std::cerr << "Invalid frame" << std::endl;
    }

    // Clear the last frame
    clear_frame();

    // Create a buffer
    std::stringstream buffer;

    // Add all pixels to buffer
    for (int i = 0; i < frame.height; i++) {
        for (int j = 0; j < frame.width; j++) {
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