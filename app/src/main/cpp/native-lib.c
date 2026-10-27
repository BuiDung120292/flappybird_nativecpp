#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 288
#define SCREEN_HEIGHT 512

#define PIPE_COUNT 3
#define PIPE_GAP 150
#define PIPE_SPEED 2

typedef struct Pipe {
    float x;
    float topHeight;
    bool passed;
} Pipe;

int main(void)
{
    InitWindow(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            "Flappy Bird"
    );

    InitAudioDevice();

    SetTargetFPS(60);

    srand(time(NULL));

    //================================
    // TEXTURES
    //================================

    Texture2D background =
            LoadTexture("background.png");

    Texture2D ground =
            LoadTexture("ground.png");

    Texture2D pipeTop =
            LoadTexture("pipe_top.png");

    Texture2D pipeBottom =
            LoadTexture("pipe_bottom.png");

    Texture2D birdTexture =
            LoadTexture("bird.png");

    Texture2D numbers[10];

    for (int i = 0; i < 10; i++)
    {
        char path[64];

        sprintf(path, "%d.png", i);

        numbers[i] =
                LoadTexture(path);
    }

    //================================
    // SOUNDS
    //================================

    Sound dieSound =
            LoadSound("die.wav");

    Sound hitSound =
            LoadSound("hit.wav");

    Sound pointSound =
            LoadSound("point.wav");

    Sound swooshSound =
            LoadSound("swoosh.wav");

    Sound wingSound =
            LoadSound("wing.wav");

    //================================
    // BIRD
    //================================

    Rectangle birdFrame = {
            0,
            0,
            34,
            24
    };

    Vector2 birdPos = {
            80,
            150
    };

    float birdVelocity = 0;

    float birdRotation = 0;

    int currentFrame = 0;

    int frameCounter = 0;

    //================================
    // GROUND
    //================================

    float groundX = 0;

    //================================
    // SCORE
    //================================

    int score = 0;

    //================================
    // GAME STATE
    //================================

    bool gameOver = false;

    //================================
    // PIPES
    //================================

    Pipe pipes[PIPE_COUNT];

    for (int i = 0; i < PIPE_COUNT; i++)
    {
        pipes[i].x =
                SCREEN_WIDTH + i * 180;

        pipes[i].topHeight =
                50 + rand() % 180;

        pipes[i].passed = false;
    }

    //================================
    // GAME LOOP
    //================================

    while (!WindowShouldClose())
    {
        bool touch =
                IsGestureDetected(GESTURE_TAP);

        //================================
        // UPDATE
        //================================

        if (!gameOver)
        {
            // JUMP
            if (IsKeyPressed(KEY_SPACE) || touch)
            {
                birdVelocity = -5.5f;

                PlaySound(wingSound);
            }

            // GRAVITY
            birdVelocity += 0.35f;

            birdPos.y += birdVelocity;

            //================================
            // ROTATION
            //================================

            if (birdVelocity < 0)
            {
                birdRotation = -25;
            }
            else
            {
                birdRotation += 2;

                if (birdRotation > 90)
                {
                    birdRotation = 90;
                }
            }

            //================================
            // BIRD ANIMATION
            //================================

            frameCounter++;

            if (frameCounter >= 8)
            {
                frameCounter = 0;

                currentFrame++;

                if (currentFrame > 2)
                {
                    currentFrame = 0;
                }
            }

            birdFrame.x =
                    currentFrame * 34;

            //================================
            // GROUND MOVE
            //================================

            groundX -= 2;

            if (groundX <= -48)
            {
                groundX = 0;
            }

            //================================
            // PIPE UPDATE
            //================================

            for (int i = 0; i < PIPE_COUNT; i++)
            {
                pipes[i].x -= PIPE_SPEED;

                // RESET PIPE
                if (pipes[i].x < -52)
                {
                    pipes[i].x =
                            SCREEN_WIDTH + 180;

                    pipes[i].topHeight =
                            50 + rand() % 180;

                    pipes[i].passed = false;
                }

                //================================
                // SCORE
                //================================

                if (!pipes[i].passed &&
                        pipes[i].x + 52 < birdPos.x)
                {
                    pipes[i].passed = true;

                    score++;

                    PlaySound(pointSound);
                }

                //================================
                // COLLISION
                //================================

                Rectangle birdRect = {
                        birdPos.x,
                        birdPos.y,
                        34,
                        24
                };

                Rectangle topRect = {
                        pipes[i].x,
                        pipes[i].topHeight - 320,
                        52,
                        320
                };

                Rectangle bottomRect = {
                        pipes[i].x,
                        pipes[i].topHeight + PIPE_GAP,
                        52,
                        320
                };

                if (
                        CheckCollisionRecs(
                                birdRect,
                                topRect
                        ) ||

                                CheckCollisionRecs(
                                        birdRect,
                                        bottomRect
                                )
                        )
                {
                    gameOver = true;

                    PlaySound(hitSound);

                    PlaySound(dieSound);
                }
            }

            //================================
            // GROUND COLLISION
            //================================

            if (birdPos.y >=
                    SCREEN_HEIGHT - 112 - 24)
            {
                gameOver = true;

                PlaySound(hitSound);

                PlaySound(dieSound);
            }

            //================================
            // TOP LIMIT
            //================================

            if (birdPos.y <= 0)
            {
                birdPos.y = 0;
            }
        }
        else
        {
            //================================
            // RESTART
            //================================

            if (IsKeyPressed(KEY_ENTER) || touch)
            {
                birdPos.x = 80;

                birdPos.y = 150;

                birdVelocity = 0;

                birdRotation = 0;

                score = 0;

                groundX = 0;

                gameOver = false;

                PlaySound(swooshSound);

                for (int i = 0; i < PIPE_COUNT; i++)
                {
                    pipes[i].x =
                            SCREEN_WIDTH + i * 180;

                    pipes[i].topHeight =
                            50 + rand() % 180;

                    pipes[i].passed = false;
                }
            }
        }

        //================================
        // DRAW
        //================================

        BeginDrawing();

        ClearBackground(BLACK);

        //================================
        // BACKGROUND
        //================================

        DrawTexture(
                background,
                0,
                0,
                WHITE
        );

        //================================
        // PIPES
        //================================

        for (int i = 0; i < PIPE_COUNT; i++)
        {
            // TOP PIPE
            DrawTexture(
                    pipeTop,
                    pipes[i].x,
                    pipes[i].topHeight - 320,
                    WHITE
            );

            // BOTTOM PIPE
            DrawTexture(
                    pipeBottom,
                    pipes[i].x,
                    pipes[i].topHeight + PIPE_GAP,
                    WHITE
            );
        }

        //================================
        // BIRD
        //================================

        Rectangle source = {
                birdFrame.x,
                birdFrame.y,
                34,
                24
        };

        Rectangle dest = {
                birdPos.x,
                birdPos.y,
                34,
                24
        };

        Vector2 origin = {
                17,
                12
        };

        DrawTexturePro(
                birdTexture,
                source,
                dest,
                origin,
                birdRotation,
                WHITE
        );

        //================================
        // GROUND
        //================================

        DrawTexture(
                ground,
                groundX,
                SCREEN_HEIGHT - 112,
                WHITE
        );

        DrawTexture(
                ground,
                groundX + 336,
                SCREEN_HEIGHT - 112,
                WHITE
        );

        //================================
        // SCORE
        //================================

        char scoreText[16];

        sprintf(scoreText, "%d", score);

        int len = strlen(scoreText);

        for (int i = 0; i < len; i++)
        {
            int digit =
                    scoreText[i] - '0';

            DrawTexture(
                    numbers[digit],

                    SCREEN_WIDTH / 2 -
                            (len * 24) / 2 +
                            i * 24,

                    40,

                    WHITE
            );
        }

        //================================
        // GAME OVER
        //================================

        if (gameOver)
        {
            DrawRectangle(
                    40,
                    180,
                    210,
                    80,
                    Fade(BLACK, 0.7f)
            );

            DrawText(
                    "GAME OVER",
                    55,
                    200,
                    32,
                    RED
            );

            DrawText(
                    "Tap to Restart",
                    70,
                    235,
                    20,
                    WHITE
            );
        }

        EndDrawing();
    }

    //================================
    // UNLOAD TEXTURES
    //================================

    UnloadTexture(background);

    UnloadTexture(ground);

    UnloadTexture(pipeTop);

    UnloadTexture(pipeBottom);

    UnloadTexture(birdTexture);

    for (int i = 0; i < 10; i++)
    {
        UnloadTexture(numbers[i]);
    }

    //================================
    // UNLOAD SOUNDS
    //================================

    UnloadSound(dieSound);

    UnloadSound(hitSound);

    UnloadSound(pointSound);

    UnloadSound(swooshSound);

    UnloadSound(wingSound);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}