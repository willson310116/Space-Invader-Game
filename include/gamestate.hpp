#pragma once

enum class GameState : int
{
    MENU = 0,
    GAMEPLAY,
    BOSS_STAGE,
    EXIT
};

enum class RewardState : int
{
    NONE = 0,
    ADD_SCORE,
    ADD_MOVE,
    ADD_LIVE,
    ADD_LASER,
    ADD_LASER_SPEED,
};
