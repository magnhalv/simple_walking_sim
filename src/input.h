#pragma once

#include "types.h"

struct ButtonState {
    i32 half_transition_count; // How many times it flipped between up and down
    bool ended_down;
};

struct MouseInput {
    i32 x;
    i32 y;
    i32 dx;
    i32 dy;
    ButtonState buttons[2];
};

struct Input {
    MouseInput mouse;
    union
    {
        ButtonState buttons[6];
        struct {
            ButtonState move_up;
            ButtonState move_down;
            ButtonState move_left;
            ButtonState move_right;

            ButtonState space;

            //Note: All buttons must be added above this line
            ButtonState terminator;
        };
    };
    f32 dt;

    void frame_clear() {
        mouse.x = 0;
        mouse.y = 0;
        mouse.dx = 0;
        mouse.dy = 0;
        for (auto &button: mouse.buttons) {
            button.half_transition_count = 0;
            button.ended_down = false;
        }

        for (auto &button : buttons) {
            button.half_transition_count = 0;
            button.ended_down = false;
        }
    }

    void process_keyboard_input(int key, int action) {

    }
};



