#include <iostream>
#include <ostream>
#include <queue>
#include <vector>
#include "core/frame.hpp"

bool is_frame_valid(Frame frame) {
    return (frame.width*frame.height) == frame.pixels.size();
}

Frame combine_frames(std::queue<Frame> frames) {
    if (frames.size() < 2) return frames.front();

    std::vector<Pixel> pixel_vector;
    int final_zindex = 0;

    int total_width = 0;
    int total_height = 0;

    std::queue<Frame> tmp = frames;
    while (!tmp.empty()) {
        Frame current_frame = tmp.front();
        tmp.pop();
        if (current_frame.width > total_width) total_width = current_frame.width;
        if (current_frame.height > total_height) total_height = current_frame.height;
    }
    
    for (int i = 0; i < total_width*total_height; i++) {
        pixel_vector.push_back(Pixel());
    }

    while (!frames.empty()) {
        Frame current_frame = frames.front();
        frames.pop();

        if (!is_frame_valid(current_frame)) {
            std::cerr << "Invalid frame" << std::endl;
            continue;
        }
        
        int pixel_count = current_frame.pixels.size();

        for (int i = 0; i < pixel_count; i++) {
            Pixel current_pixel = current_frame.pixels.front();
            current_frame.pixels.pop();

            int new_index = i + (total_width - current_frame.width)*(i/current_frame.width);
            Pixel placed_pixel = pixel_vector.at(new_index);
            if ((!placed_pixel.rendered || current_frame.zindex >= final_zindex) && current_pixel.rendered) {
                pixel_vector[new_index] = current_pixel;
            }
        }

        if (current_frame.zindex > final_zindex) final_zindex = current_frame.zindex;
    }

    std::queue<Pixel> final_pixels;
    for (Pixel pixel : pixel_vector) {
        final_pixels.push(pixel);
    }

    return Frame(total_width, total_height, 0, final_pixels);
}

Frame create_rect(int width, int height, int zindex, char ch, int color) {
    std::queue<Pixel> pixels;
    for (int i = 0; i < width*height; i++) {
        pixels.push(Pixel(true, ch, color));
    }
    return Frame(width,height,zindex,pixels);
}