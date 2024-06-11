#include "raylib.h"

typedef struct ball {
    Vector2 pos;
    Vector2 speed;
    int rad;
} Ball;

void update(Ball *b);

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight,
               "raylib [core] example - 3d camera first person");

    SetTargetFPS(60);

    Ball a = {.pos = {10.0f, 10.0f}, .speed = {2, 2}, .rad = 10};

    Ball b = {.pos = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
              .speed = {-2, 1},
              .rad = 20};

    float gravity = -9.81f;

    while (!WindowShouldClose()) {

        update(&a);
        update(&b);

        BeginDrawing();

        ClearBackground(BLACK);

        DrawCircleV(a.pos, a.rad, ORANGE);
        DrawCircleV(b.pos, b.rad, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void update(Ball *b) {

    b->pos.y += b->speed.y;
    b->pos.x += b->speed.x;

    if (b->pos.y <= b->rad || b->pos.y >= GetScreenHeight() - b->rad) {
        b->speed.y *= -1;
    }

    if (b->pos.x <= b->rad || b->pos.x >= GetScreenWidth() - b->rad) {
        b->speed.x *= -1;
    }
}
