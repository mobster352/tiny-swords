#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SPRITE_SIZE 192
#define SPRITE_DIVISOR 1.2f
#define COLLIDER_WIDTH_SIZING 70
#define COLLIDER_HEIGHT_SIZING 50

#define PLAYER_SPEED 100

// enum Animation {
//     IDLE_ANIMATION,
//     WALK_ANIMATION,
//     HOOK_ANIMATION,
//     UPPERCUT_ANIMATION,
//     DOWN_SLASH_RIGHT_ANIMATION,
//     DOWN_SLASH_LEFT_ANIMATION,
//     UP_SLASH_RIGHT_ANIMATION,
//     UP_SLASH_LEFT_ANIMATION
// }; //yIndex

// bool isLooping[8] = {
//     true,
//     true,
//     false,
//     false,
//     false,
//     false,
//     false,
//     false
// };

enum Animation {
    IDLE_ANIMATION,
    RUN_ANIMATION,
    ATTACK1_ANIMATION,
    ATTACK2_ANIMATION,
    GUARD_ANIMATION
};

bool isLooping[5] = {
    true,
    true,
    false,
    false,
    true
};


int animationSteps[5] = {
    8,
    6,
    4,
    4,
    6
};

#define ANIMATION_STEPS 5

typedef struct transform2D {
    Vector2 position;
} Transform2D;

typedef struct player {
    Transform2D transform2D;
    Rectangle collider;
} Player;

#endif