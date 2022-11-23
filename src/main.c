#include "common.h"
#include "./states/map-generator.c"
#include "game-state.c"

void input_update()
{
    game_state.input.mouse_prev_x = game_state.input.mouse_x;
    game_state.input.mouse_prev_y = game_state.input.mouse_y;
    game_state.input = (input_t){
        .mouse_x = ((double)GetScreenWidth() / (double)APP_WIDTH) * GetMousePosition().x,
        .mouse_y = ((double)GetScreenHeight() / (double)APP_HEIGHT) * GetMousePosition().y,
        .mouse_prev_x = game_state.input.mouse_prev_x,
        .mouse_prev_y = game_state.input.mouse_prev_y,
        .mouse_delta_x = game_state.input.mouse_x - game_state.input.mouse_prev_x,
        .mouse_delta_y = game_state.input.mouse_y - game_state.input.mouse_prev_y,
        .lmb_down = IsMouseButtonDown(MOUSE_LEFT_BUTTON),
        .rmb_down = IsMouseButtonDown(MOUSE_RIGHT_BUTTON),
        .lmb_pressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON),
        .rmb_pressed = IsMouseButtonPressed(MOUSE_RIGHT_BUTTON),
        .key_pressed = GetKeyPressed()};
}

void load_resources()
{
    map_generator_init();
}

void window_init()
{
    InitWindow(0, 0, "Raylib Project Template");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    game_state.fullscreen_width = GetScreenWidth();
    game_state.fullscreen_height = GetScreenHeight();
    game_state.windowed_width = game_state.fullscreen_width / 2;
    game_state.windowed_height = game_state.fullscreen_height / 2;
    game_state.windowed_position_x = (game_state.fullscreen_width / 2) - (game_state.windowed_width / 2);
    game_state.windowed_position_y = (game_state.fullscreen_height / 2) - (game_state.windowed_height / 2);

    SetWindowPosition(game_state.windowed_position_x, game_state.windowed_position_y);
    SetWindowSize(game_state.windowed_width, game_state.windowed_height);
}

void toggle_fullscreen()
{
    game_state.fullscreen = !game_state.fullscreen;
    if (game_state.fullscreen)
    {
        SetWindowSize(game_state.fullscreen_width, game_state.fullscreen_height);
        SetWindowState(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
        SetWindowPosition(0, 0);
    }
    else
    {
        SetWindowSize(game_state.windowed_width, game_state.windowed_height);
        ClearWindowState(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
        SetWindowPosition(game_state.windowed_position_x, game_state.windowed_position_y);
    }
}

int main()
{

    window_init();

    RenderTexture2D canvas = LoadRenderTexture(game_state.fullscreen_width, game_state.fullscreen_height);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);

    load_resources();

    while (!WindowShouldClose())
    {
        game_state.delta_time = GetFrameTime();

        input_update();

        if (IsKeyPressed(KEY_F11))
        {
            toggle_fullscreen();
        }

        if (IsWindowFocused()) // only update logic when focused
        {
            if (game_state.curr_state == GAME_STATE_QUIT)
            {
                goto EXIT_PROCEDURE;
            }
            if (state_update_funcs[game_state.curr_state] != NULL)
            {
                game_state_e next_state = (*state_update_funcs[game_state.curr_state])();
                state_change(next_state);
            }
        }

        // RENDER /////////////////////////////////////////

        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);
        if (state_render_funcs[game_state.curr_state] != NULL)
        {
            (*state_render_funcs[game_state.curr_state])();
        }
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        float scale = MIN((float)GetScreenWidth() / APP_WIDTH, (float)GetScreenHeight() / APP_HEIGHT);
        DrawTexturePro(canvas.texture,
                       (Rectangle){0.0f, 0.0f, (float)canvas.texture.width, (float)-canvas.texture.height},
                       (Rectangle){(GetScreenWidth() - ((float)APP_WIDTH * scale)) * 0.5f,
                                   (GetScreenHeight() - ((float)APP_HEIGHT * scale)) * 0.5f,
                                   (float)APP_WIDTH * scale, (float)APP_HEIGHT * scale},
                       (Vector2){0, 0},
                       0.0f,
                       WHITE);
        EndDrawing();
    }

EXIT_PROCEDURE:

    UnloadTexture(tilemap_texture);

    UnloadRenderTexture(canvas);
    CloseWindow();

    return 0;
}