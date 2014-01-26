#ifndef PLAYER_H
#define PLAYER_H

#include "element.h"

#define DEFAULT_POSITION_PLAYER_1_X 21
#define DEFAULT_POSITION_PLAYER_1_Y 26
#define DEFAULT_POSITION_PLAYER_2_X 38
#define DEFAULT_POSITION_PLAYER_2_Y 26

enum PLAYER_ID
{
    PLAYER_1,
    PLAYER_2
};

class Player
{
private:
    PLAYER_ID m_playerId;

public:
    Player(PLAYER_ID playerId);

    Element generateElement();
};

#endif PLAYER_H