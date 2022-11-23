#include "../common.h"

// Fix the png file
// Implement recursive backtracker
// 


typedef enum tile_directions_e
{
    DIR_RIGHT = 0b00000001,
    DIR_DOWN = 0b00000010,
    DIR_LEFT = 0b00000100,
    DIR_UP = 0b00001000
} tile_directions_e;

typedef enum tile_connection_e
{
    TILE_BLANK,
    TILE_RIGHT,
    TILE_DOWN,
    TILE_DOWN_RIGHT,
    TILE_LEFT,
    TILE_LEFT_RIGHT,
    TILE_LEFT_DOWN,
    TILE_LEFT_DOWN_RIGHT,
    TILE_UP,
    TILE_UP_RIGHT,
    TILE_UP_DOWN,
    TILE_UP_DOWN_RIGHT,
    TILE_UP_LEFT,
    TILE_UP_LEFT_RIGHT,
    TILE_UP_LEFT_DOWN,
    TILE_UP_LEFT_DOWN_RIGHT
} tile_connection_e;

i32 tilemap[256];
i32 cursor;
Texture2D tilemap_texture;

void map_generator_enter(game_state_e from, i32 code)
{
}

i32 carve(i32 from, tile_directions_e dir)
{
    tilemap[from] |= dir;
    i32 x = from % 16, y = from / 16;
    i32 dx = 0, dy = 0;
    tile_directions_e rev_dir = 0;
    switch (dir)
    {
    case DIR_DOWN:
        dy = 1;
        rev_dir = DIR_UP;
        break;
    case DIR_UP:
        dy = -1;
        rev_dir = DIR_DOWN;
        break;
    case DIR_LEFT:
        dx = -1;
        rev_dir = DIR_RIGHT;
        break;
    case DIR_RIGHT:
        dx = 1;
        rev_dir = DIR_LEFT;
        break;
    }
    x += dx;
    y += dy;
    tilemap[y * 16 + x] |= rev_dir;
    return y * 16 + x;
}

void map_generator_init()
{
    for (int i = 0; i < 256; i++)
    {
        tilemap[i] = TILE_BLANK;
    }

    tilemap_texture = LoadTexture("../assets/tilemap.png");
}

game_state_e map_generator_update()
{
    if (IsKeyPressed(KEY_W))
    {
        cursor = carve(cursor, DIR_UP);
    }
    if (IsKeyPressed(KEY_S))
    {
        cursor = carve(cursor, DIR_DOWN);
    }
    if (IsKeyPressed(KEY_A))
    {
        cursor = carve(cursor, DIR_LEFT);
    }
    if (IsKeyPressed(KEY_D))
    {
        cursor = carve(cursor, DIR_RIGHT);
    }
    return GAME_STATE_MAP_GENERATOR;
}

void map_generator_render()
{
    ClearBackground(GREEN);

    Rectangle source = {
        .width = 32,
        .height = 32,
        .y = 0};
    Rectangle destination = {
        .width = 32,
        .height = 32,
    };

    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            i32 tileID = tilemap[y * 16 + x];
            source.x = tileID * 32;
            destination.x = x * 32;
            destination.y = y * 32;
            DrawTexturePro(tilemap_texture, source, destination, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
            DrawText(TextFormat("%d", tilemap[y * 16 + x]), x * 32, y * 32, 16, RED);
        }
    }

    i32 cursor_x = ((cursor % 16) * 32) + 16;
    i32 cursor_y = ((cursor / 16) * 32) + 16;
    DrawCircle(cursor_x, cursor_y, 8.0f, GREEN);
}

i32 map_generator_exit()
{
    return 0;
}