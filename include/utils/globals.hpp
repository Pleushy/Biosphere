#pragma once

namespace constants {
    constexpr int SECOND_IN_NANOSECONDS = 1000000;
    constexpr int FRAMES_PER_SECOND = 10;
    constexpr int FRAME_INTERVAL = SECOND_IN_NANOSECONDS/FRAMES_PER_SECOND;
    constexpr int DEFAULT_COLOR = 15;
}

enum Events {
    INPUT,
    ENVIRONMENT
};