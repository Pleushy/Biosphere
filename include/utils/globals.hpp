#pragma once

namespace constants {
    constexpr int SECOND_IN_NANOSECONDS = 1000000;
    constexpr int FRAMES_PER_SECOND = 60;
    constexpr int FRAME_INTERVAL = SECOND_IN_NANOSECONDS/FRAMES_PER_SECOND;
}