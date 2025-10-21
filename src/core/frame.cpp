#include <iostream>
#include <ostream>
#include <queue>
#include <string>
#include <vector>
#include "core/frame.hpp"
#include "utils/vec2.hpp"
#include <optional>

bool frame::is_valid(Frame frame) {
    return (frame.size.x*frame.size.y) == frame.pixels.size();
}

std::optional<Frame> frame::combine(std::queue<Frame> frames) {
    if (!frames.size()) return std::nullopt;
    if (frames.size() < 2) return frames.front();

    std::vector<Pixel> pixel_vector;
    int highest_zindex = 0;
    Vec2 total_size{};

    std::queue<Frame> tmp = frames;
    while (!tmp.empty()) {
        Frame current_frame = tmp.front();
        tmp.pop();
        if (current_frame.size.x > total_size.x) total_size.x = current_frame.size.x;
        if (current_frame.size.y > total_size.y) total_size.y = current_frame.size.y;
    }
    
    for (int i = 0; i < total_size.x*total_size.y; i++) {
        pixel_vector.push_back(Pixel{false});
    }

    while (!frames.empty()) {
        Frame current_frame = frames.front();
        frames.pop();

        if (!is_valid(current_frame) || current_frame.position.x+current_frame.size.x > total_size.x || current_frame.position.y+current_frame.size.y > total_size.y) {
            std::cerr << "Invalid frame" << std::endl;
            continue;
        }
        
        int pixel_count = current_frame.pixels.size();

        for (int i = 0; i < pixel_count; i++) {
            Pixel current_pixel = current_frame.pixels.front();
            current_frame.pixels.pop();

            int new_index = i
            + current_frame.position.x
            + current_frame.position.y * total_size.x
            + (total_size.x - current_frame.size.x) * (i/current_frame.size.x);

            Pixel placed_pixel = pixel_vector.at(new_index);
            if ((!placed_pixel.rendered || current_frame.zindex >= highest_zindex) && current_pixel.rendered) {
                pixel_vector[new_index] = current_pixel;
            }
        }

        if (current_frame.zindex > highest_zindex) highest_zindex = current_frame.zindex;
    }

    std::queue<Pixel> final_pixels;
    for (Pixel pixel : pixel_vector) {
        final_pixels.push(pixel);
    }

    return Frame(total_size, Vec2{}, 0, final_pixels);
}

Frame draw::blank(Vec2 size) {
    std::queue<Pixel> pixels;
    for (int i = 0; i < size.x*size.y; i++) {
        pixels.push(Pixel{false});
    }
    return Frame(size,Vec2{},0,pixels);
}

Frame draw::rect(Vec2 size, Vec2 position, int zindex, char ch, int color) {
    std::queue<Pixel> pixels;
    for (int i = 0; i < size.x*size.y; i++) pixels.push(Pixel{true, ch, color});
    return Frame(size,position,zindex,pixels);
}