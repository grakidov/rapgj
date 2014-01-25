#ifndef BOARD_H
#define BOARD_H

#include "element.h"

#define BOARD_WIDTH 60
#define BOARD_HEIGHT 35

#define PLAYER_1_POSITION_X 15
#define PLAYER_1_POSITION_Y 0

#define PLAYER_2_POSITION_X 45
#define PLAYER_2_POSITION_Y 0

#define EMPTY_TILE = 0

class Board {
private:
    Element boardArray[BOARD_WIDTH][BOARD_HEIGHT];

    Element currentElement;
    int currentElementId = 0; // 0 = empty tile

	int m_backgroundTexture;
	int m_elementTextures[ELEMENT_TYPE::EMPTY][ELEMENT_ROTATION::COUNT];
	int m_elementMetrics[ELEMENT_TYPE::EMPTY][ELEMENT_ROTATION::COUNT][2];

	void renderElement();

public:
    Board();

	bool init();
    void clear();

	void render();
	void renderElement(const Element& element);

    void addElement(Element element);
    bool isMovementPossible(Element element, Position newPosition);

	static Board& get();
};

#endif BOARD_H