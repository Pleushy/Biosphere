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

std::optional<Frame> frame::combine(std::vector<Frame> frames, int zindex) {
    if (!frames.size()) return std::nullopt;
    if (frames.size() == 1) {
        frames[0].zindex = zindex;
        return frames[0];
    }

    std::vector<Pixel> pixel_vector;
    int highest_zindex = 0;
    Vec2 total_size;
    Vec2 min_pos{9999,9999};
    Vec2 total_pos;

    for (int i = 0; i < frames.size(); i++) {
        Frame current_frame = frames[i];

        if (current_frame.position.x < min_pos.x) min_pos.x = current_frame.position.x;
        if (current_frame.position.y < min_pos.y) min_pos.y = current_frame.position.y;

        if (current_frame.position.x > total_pos.x) total_pos.x = current_frame.position.x;
        if (current_frame.position.y > total_pos.y) total_pos.y = current_frame.position.y;

        if (current_frame.size.x > total_size.x) total_size.x = current_frame.size.x;
        if (current_frame.size.y > total_size.y) total_size.y = current_frame.size.y;
    }
    
    if (min_pos.x > 0 || min_pos.y > 0) {
       total_size = total_size+total_pos;
    }

    for (int i = 0; i < total_size.x*total_size.y; i++) {
        pixel_vector.push_back(Pixel{false});
    }

    for (int i = 0; i < frames.size(); i++) {
        Frame current_frame = frames[i];

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
            if ((!placed_pixel.rendered || current_frame.zindex > highest_zindex) && current_pixel.rendered) {
                pixel_vector[new_index] = current_pixel;
            }
        }

        if (current_frame.zindex > highest_zindex) highest_zindex = current_frame.zindex;
    }

    std::queue<Pixel> final_pixels;
    for (Pixel pixel : pixel_vector) {
        final_pixels.push(pixel);
    }

    return Frame{total_size, Vec2{}, zindex, final_pixels};
}

std::optional<Animation> frame::combine_animations(std::vector<Animation> animations, int zindex) {
    if (!animations.size()) return std::nullopt;
    if (animations.size() == 1) {
        while (!animations[0].empty()) {
            animations[0].front().zindex = zindex;
            animations[0].pop();
        }
        return animations[0];
    }

    std::vector<Frame> output_anim_vector;
    
    for (int i = 0; i < animations.size(); i++) {
        Animation anim = animations[i];
        int j = 0;
        while (!anim.empty()) {
            Frame current_frame = anim.front();
            anim.pop();

            if ((j+1) > output_anim_vector.size()) {
                output_anim_vector.push_back(current_frame);
            } else {
                std::optional<Frame> combined = combine({current_frame, output_anim_vector[j]}, zindex);
                if (combined.has_value()) {
                    output_anim_vector[j] = combined.value();
                } else {
                    std::cerr << "Failed to combine frames" << std::endl;
                    return std::nullopt;
                }
            }
            j++;
        }
    }

    Animation output_anim;
    for (Frame frame : output_anim_vector) {
        output_anim.push(frame);
    }
    return output_anim;
}

std::optional<Animation> frame::combine_animations_and_frames(std::vector<Frame> frames, std::vector<Animation> animations, int frame_zindex, int animation_zindex, int final_zindex) {
    if (!animations.size() || !frames.size()) return std::nullopt;

    std::optional<Frame> combined_frames = combine(frames, frame_zindex);
    if (!combined_frames.has_value()) return std::nullopt;

    std::optional<Animation> combined_anims = combine_animations(animations, animation_zindex);
    if (!combined_anims.has_value()) return std::nullopt;

    Animation output_anim;

    while (!combined_anims.value().empty()) {
        std::vector<Frame> to_combine = {combined_anims.value().front(), combined_frames.value()};
        std::optional<Frame> combined = combine(to_combine, final_zindex);
        combined_anims.value().pop();

        if (combined.has_value()) {
            output_anim.push(combined.value());
        }
    }

    return output_anim;
}

Frame draw::blank(Vec2 size) {
    std::queue<Pixel> pixels;
    for (int i = 0; i < size.x*size.y; i++) {
        pixels.push(Pixel{false});
    }
    return Frame{size,Vec2{},0,pixels};
}

Frame draw::rect(Vec2 size, Vec2 position, int zindex, char ch, int color) {
    std::queue<Pixel> pixels;
    for (int i = 0; i < size.x*size.y; i++) pixels.push(Pixel{true, ch, color});
    return Frame{size,position,zindex,pixels};
}