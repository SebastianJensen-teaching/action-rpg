#ifndef __COMMON_H_
#define __COMMON_H_

#include "../external/raylib.h"
#include "../external/raymath.h"

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef signed long long i64;
typedef signed int i32;
typedef signed short i16;
typedef signed char i8;

typedef float f32;
typedef double f64;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef enum
{
    GAME_STATE_DEVELOPER,
    GAME_STATE_MAP_GENERATOR,
    GAME_STATE_QUIT
} game_state_e;

typedef struct
{
    i32 mouse_x;
    i32 mouse_y;
    i32 mouse_prev_x;
    i32 mouse_prev_y;
    i32 mouse_delta_x;
    i32 mouse_delta_y;
    bool lmb_down;
    bool rmb_down;
    bool lmb_pressed;
    bool rmb_pressed;
    i32 key_pressed;
} input_t;

typedef struct
{
    bool fullscreen;
    i32 windowed_width;
    i32 windowed_height;
    i32 fullscreen_width;
    i32 fullscreen_height;
    i32 windowed_position_x;
    i32 windowed_position_y;
    game_state_e curr_state;
    game_state_e prev_state;
    float time_in_state;
    input_t input;
    float delta_time;
} game_state_t;

#define APP_WIDTH (game_state.fullscreen ? game_state.fullscreen_width : game_state.windowed_width)
#define APP_HEIGHT (game_state.fullscreen ? game_state.fullscreen_height : game_state.windowed_height)

#endif