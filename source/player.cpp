#include "ra_pch.h"

#include "player.h"

#include <stdlib.h>
#include <time.h>

#include "element.h"

Player::Player(PLAYER_ID playerId)
{
    m_playerId = playerId;
}

Element Player::generateElement()
{
    // Randomize numbers - LONG4 - 20%, CORNER3 - 40%, SMALL - 40%
    int weight = rand() % 5;
    ELEMENT_TYPE elementType;

    switch (weight)
    {
    case 0:
        elementType = ELEMENT_TYPE::LONG4;
        break;
    case 1:
    case 2:
        elementType = ELEMENT_TYPE::CORNER3;
        break;
    case 3:
    case 4:
        elementType = ELEMENT_TYPE::SMALL2;
        break;
    }

    cout << weight << " ";

    // Generate a number between ELEMENT_TYPE::UP and ELEMENT_TYPE::LEFT
    ELEMENT_ROTATION elementRotation = static_cast<ELEMENT_ROTATION>(rand() % (ELEMENT_ROTATION::LEFT + 1) + ELEMENT_ROTATION::UP);

    Position position;

    if (PLAYER_ID::PLAYER_1 == m_playerId)
    {
        position.x = DEFAULT_POSITION_PLAYER_1_X;
        position.y = DEFAULT_POSITION_PLAYER_1_Y;
    }
    else
    {
        position.x = DEFAULT_POSITION_PLAYER_2_X;
        position.y = DEFAULT_POSITION_PLAYER_2_Y;
    }

    return Element(elementType, position, elementRotation);
}