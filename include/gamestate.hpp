#pragma once

enum class GameState
{
    MENU = 0,
    GAMEPLAY,
    EXIT
};

enum class RewardState 
{
    NONE = 0,
    ADD_SCORE,
    ADD_MOVE,
    ADD_LIVE,
    ADD_LASER,
    ADD_LASER_SPEED,
};
