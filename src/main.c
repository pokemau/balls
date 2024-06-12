#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>

#define COUNT 2

typedef struct {
    Vector2 pos;
    Vector2 speed;
    int rad;
} Ball;

void update(Ball *b);
void generateBalls(Ball *balls);
void drawBalls(Ball *balls);

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight,
               "raylib [core] example - 3d camera first person");

    SetTargetFPS(60);

    Ball *balls = (Ball *)malloc(sizeof(Ball) * COUNT);

    generateBalls(balls);

    Ball a = {.pos = {40.0f, 40.0f}, .speed = {4, 2}, .rad = 20};

    Ball b = {.pos = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
              .speed = {-2, 1},
              .rad = 40};

    float gravity = -9.81f;

    while (!WindowShouldClose()) {

        update(balls);

        BeginDrawing();

        ClearBackground(BLACK);

        drawBalls(balls);

        // TODO: OPTIMIZE COLLISION DETECTION
        for (int i = 0; i < COUNT; i++) {
            Ball *curr = &balls[i];

            for (int j = 0; j < COUNT; j++) {
                Ball *target = &balls[j];

                if (curr == target) {
                    continue;
                }

                float distance = Vector2Distance(curr->pos, target->pos);

                if (distance < curr->rad + target->rad) {

                    Vector2 un = Vector2Normalize(
                        Vector2Subtract(curr->speed, target->speed));
                    Vector2 ut = {-un.y, un.x};

                    float v1n = Vector2DotProduct(un, curr->speed);
                    float v1t = Vector2DotProduct(ut, curr->speed);

                    float v2n = Vector2DotProduct(un, target->speed);
                    float v2t = Vector2DotProduct(ut, target->speed);

                    Vector2 newV1n = Vector2Scale(un, v1n);
                    Vector2 newV1t = Vector2Scale(ut, v1t);

                    Vector2 newV2n = Vector2Scale(un, v2n);
                    Vector2 newV2t = Vector2Scale(ut, v2t);

                    curr->speed = Vector2Add(newV1n, newV1t);
                    target->speed = Vector2Add(newV2n, newV2t);

                    // curr->speed = normal;

                    /*
                        Vector2 angle = { target->pos.x, target->pos.y };
                        angle = Vector2Normalize(angle);

                        DrawLine(curr->pos.x, curr->pos.y, target->pos.x,
                                 target->pos.y, RED);

                        target->speed.x = angle.x;
                        target->speed.y = angle.y;

                        curr->speed.x = -angle.x;
                        curr->speed.y = -angle.y;
                        */
                }
            }
        }

        DrawFPS(10, 10);

        EndDrawing();
    }

    free(balls);

    CloseWindow();

    return 0;
}

void update(Ball *balls) {

    for (int i = 0; i < COUNT; i++) {

        Ball *curr = &balls[i];

        curr->pos.x += curr->speed.x;
        curr->pos.y += curr->speed.y;

        if (curr->pos.y <= curr->rad ||
            curr->pos.y >= GetScreenHeight() - curr->rad) {

            curr->speed.y *= -1;
        }

        if (curr->pos.x <= curr->rad ||
            curr->pos.x >= GetScreenWidth() - curr->rad) {
            curr->speed.x *= -1;
        }
    }
}

void createBall(Ball *b, Vector2 pos, Vector2 speed, int rad) {
    b->pos = pos;
    b->speed = speed;
    b->rad = rad;
}

void drawBalls(Ball *balls) {

    for (int i = 0; i < COUNT; i++) {
        DrawCircleV(balls[i].pos, balls[i].rad, ORANGE);
    }
}

void generateBalls(Ball *balls) {

    Vector2 pos = {100, 100};
    Vector2 speed = {2, 2};
    int rad = 30;
    createBall(&(balls[0]), pos, speed, rad);

    Vector2 pos2 = {200, 200};
    Vector2 speed2 = {-2, -2};

    createBall(&(balls[1]), pos2, speed2, rad);

    /*
    for (int i = 0; i < COUNT; i++) {
        Vector2 pos = {GetRandomValue(40, GetScreenWidth() - 30),
                       GetRandomValue(40, GetScreenHeight() - 30)};
        // Vector2 speed = {GetRandomValue(-2, 2), GetRandomValue(-2, 2)};
        Vector2 speed = {-2, 2};
        int rad = GetRandomValue(10, 30);

        createBall(&(balls[i]), pos, speed, rad);
    }
    Vector2 speed = {2, 2};
    */
}
