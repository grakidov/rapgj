#ifndef BOARD_H
#define BOARD_H

#include "element.h"

#define BOARD_WIDTH 60
#define BOARD_HEIGHT 32

#define PLAYER_1_POSITION_X 15
#define PLAYER_1_POSITION_Y 0

#define PLAYER_2_POSITION_X 45
#define PLAYER_2_POSITION_Y 0

#define EMPTY_TILE = 0

#define TARGET_SPRITE_COUNT 16
#define CUBE_SPRITE_INDEX 0
#define PYRAMID_SPRITE_INDEX 8


class Cell
{
public:
	int m_elementId;
	Cell()
	{
		m_elementId = -1;
	}
};

class Board {
private:
	Cell boardArray[BOARD_WIDTH][BOARD_HEIGHT];

    Element currentElement;
    int currentElementId = 0; // 0 = empty tile

	int m_backgroundTexture;
	int m_elementTextures[ELEMENT_TYPE::EMPTY][ELEMENT_ROTATION::COUNT];
	int m_elementMetrics[ELEMENT_TYPE::EMPTY][ELEMENT_ROTATION::COUNT][2];
	//bool m_elementOccupation[ELEMENT_TYPE::EMPTY][ELEMENT_ROTATION::COUNT][4];
	ELEMENT_TYPE m_breakMap[ELEMENT_TYPE::EMPTY];


    int m_targetTextures[TARGET_SPRITE_COUNT];
    int m_targetMetrics[TARGET_SPRITE_COUNT];

	map<int, Element> m_elements;

	void renderElement();

    void testAllElements();

public:
    Board();

	bool init();
    void clear();

	void render();
	void renderElement(const Element& element);
    void renderTarget(int spriteIndex, Position position);

    void addElement(Element element);
    bool isMovementPossible(const Element& element, Position newPosition);
	bool breakTest(const Element& element, set<int>& elementsToBreak);
	void placeElement(const Element& element);
	void removeElement(const Element& element);

	static Board& get();
};

#endif BOARD_H