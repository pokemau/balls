#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define COUNT 200

Color colors[] = {YELLOW, ORANGE, GREEN, RED};

typedef struct {
    Vector2 pos;
    Vector2 vel;
    int rad;
    Color color;
} Ball;

void update(Ball *b);
void generateBalls(Ball *balls);
void drawBalls(Ball *balls);
void handleCollision(Ball *ball1, Ball *ball2);

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "balls");

    SetTargetFPS(60);

    Ball *balls = (Ball *)malloc(sizeof(Ball) * COUNT);

    generateBalls(balls);

    Ball a = {.pos = {40.0f, 40.0f}, .vel = {4, 2}, .rad = 20};

    Ball b = {.pos = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
              .vel = {-2, 1},
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

                    handleCollision(curr, target);
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

        curr->pos.x += curr->vel.x;
        curr->pos.y += curr->vel.y;

        if (curr->pos.y <= curr->rad ||
            curr->pos.y >= GetScreenHeight() - curr->rad) {

            curr->vel.y *= -1;
        }

        if (curr->pos.x <= curr->rad ||
            curr->pos.x >= GetScreenWidth() - curr->rad) {
            curr->vel.x *= -1;
        }
    }
}

void createBall(Ball *b, Vector2 pos, Vector2 vel, int rad) {
    b->pos = pos;
    b->vel = vel;
    b->rad = rad;
    b->color = colors[GetRandomValue(0, 3)];
}

void drawBalls(Ball *balls) {

    for (int i = 0; i < COUNT; i++) {
        DrawCircleV(balls[i].pos, balls[i].rad, balls[i].color);
    }
}

void generateBalls(Ball *balls) {

    for (int i = 0; i < COUNT; i++) {
        Vector2 pos = {GetRandomValue(80, GetScreenWidth() - 30),
                       GetRandomValue(80, GetScreenHeight() - 30)};

        Vector2 vel = {1, 1};

        int rad = GetRandomValue(5, 5);

        createBall(&(balls[i]), pos, vel, rad);
    }
    Vector2 vel = {2, 2};
}

void handleCollision(Ball *ball1, Ball *ball2) {
    float dx = ball1->pos.x - ball2->pos.x;
    float dy = ball1->pos.y - ball2->pos.y;
    float distance = sqrt(dx * dx + dy * dy);

    distance = Vector2Distance(ball1->pos, ball2->pos);

    float nx = dx / distance;
    float ny = dy / distance;

    float vx = ball1->vel.x - ball2->vel.x;
    float vy = ball1->vel.y - ball2->vel.y;

    float dotProduct = vx * nx + vy * ny;

    if (dotProduct > 0)
        return;

    float impulse = (2 * dotProduct) / (1 + 1);

    ball1->vel.x -= impulse * nx;
    ball1->vel.y -= impulse * ny;
    ball2->vel.x += impulse * nx;
    ball2->vel.y += impulse * ny;
}
