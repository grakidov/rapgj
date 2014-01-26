#ifndef PLAYER_H
#define PLAYER_H

#include "element.h"

#define DEFAULT_POSITION_PLAYER_1_X 21
#define DEFAULT_POSITION_PLAYER_1_Y 26
#define DEFAULT_POSITION_PLAYER_2_X 38
#define DEFAULT_POSITION_PLAYER_2_Y 26

class Board;

enum PLAYER_ID
{
    PLAYER_1,
    PLAYER_2
};

enum PLAYER_STATE
{
	GENERATE,
	FALL,
	BREAK,
	FALL_STEP,
};

enum PLAYER_INPUT
{
	IN_LEFT,
	IN_RIGHT,
	IN_ROT,
	IN_COUNT
};

class Player
{
private:
    PLAYER_ID m_playerId;
	PLAYER_STATE m_state;
	Board* m_board;

	//fall state variables
	int m_fallElementId;
	Position m_fallStartPos;
	float m_fallStartTime;
	float m_fallSpeed;

	//break state variables
	float m_breakStartTime;
	float m_breakWaitTime;
	set<int> m_elementsToBreak;

	//fall step state variables
	float m_fallStepStartTime;
	float m_fallStepTime;

public:

	bool m_input[PLAYER_INPUT::IN_COUNT];

    Player(PLAYER_ID playerId);
	void setBoard(Board* board) { m_board = board; }

    Element generateElement();
	void update(float time);
};

#endif PLAYER_H