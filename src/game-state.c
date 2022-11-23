#include "common.h"

game_state_t game_state = {
    .curr_state = GAME_STATE_MAP_GENERATOR};

void (*state_enter_funcs[])(game_state_e, i32) = {
    NULL, // developer menu
    map_generator_enter,
    NULL, // quit
};

game_state_e (*state_update_funcs[])() = {
    NULL, // developer menu
    map_generator_update,
    NULL // quit
};

void (*state_render_funcs[])() = {
    NULL, // developer menu
    map_generator_render,
    NULL // quit
};

i32 (*state_exit_funcs[])() = {
    NULL, // developer menu
    map_generator_exit,
    NULL // quit
};

i32 state_exit(game_state_e state)
{
    if ((*state_exit_funcs[state]) != NULL)
    {
        return (*state_exit_funcs[state])();
    }
    return 0;
}

void state_enter(game_state_e to, game_state_e from, i32 code)
{
    if ((*state_enter_funcs[to]) != NULL)
    {
        (*state_enter_funcs[to])(from, code);
    }
}

void state_change(game_state_e state)
{
    if (game_state.curr_state != state)
    {
        i32 state_exit_code = state_exit(game_state.curr_state);
        state_enter(state, game_state.curr_state, state_exit_code);
        game_state.prev_state = game_state.curr_state;
        game_state.curr_state = state;
    }
}