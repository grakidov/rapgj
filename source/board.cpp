#include "ra_pch.h"
#include "board.h"
#include "engine.h"

#include "element_shapes.h"

Board::Board()
{
	for (int i = 0; i < ELEMENT_TYPE::EMPTY; i++)
	{
		for (int j = 0; j < ELEMENT_ROTATION::COUNT; j++)
		{
			m_elementTextures[i][j] = -1;
		}
	}
}

bool Board::init()
{
	m_backgroundTexture = Engine::get().loadTexture("../content/Background.tga", 1, 1);
	if (m_backgroundTexture == -1)
	{
		return false;
	}


	m_elementTextures[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::UP] = Engine::get().loadTexture("../content/block_1_pos1.tga", 1, 1);
	if (m_elementTextures[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::UP] == -1)
	{
		return false;
	}

	m_elementTextures[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::DOWN] = m_elementTextures[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::UP];
	
	m_elementTextures[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::RIGHT] = Engine::get().loadTexture("../content/block_1_pos2.tga", 1, 1);
	if (m_elementTextures[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::RIGHT] == -1)
	{
		return false;
	}

	m_elementTextures[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::LEFT] = m_elementTextures[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::RIGHT];

	m_elementMetrics[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::UP][0] = 1;
	m_elementMetrics[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::UP][1] = 4;

	m_elementMetrics[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::DOWN][0] = 1;
	m_elementMetrics[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::DOWN][1] = 4;

	m_elementMetrics[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::RIGHT][0] = 4;
	m_elementMetrics[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::RIGHT][1] = 1;

	m_elementMetrics[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::LEFT][0] = 4;
	m_elementMetrics[ELEMENT_TYPE::LONG4][ELEMENT_ROTATION::LEFT][1] = 1;

	return true;
}

void Board::clear()
{
    for (int row = 0; row < BOARD_WIDTH; row++)
    {
        for (int column = 0; column < BOARD_HEIGHT; column++)
        {
            boardArray[row][column] = Element();
        }
    }
}

void Board::renderElement(const Element& element)
{
	int textureId = m_elementTextures[element.getType()][element.getRotation()];


	float elementBottom = ((float)element.getPosition().y / BOARD_HEIGHT)* 2.0f - 1.0f;
	float elementLeft = ((float)element.getPosition().x / BOARD_WIDTH) * 2.0f - 1.0f;

	float elementWidth = ((float) m_elementMetrics[element.getType()][element.getRotation()][0] / BOARD_WIDTH)* 2.0f;
	float elementHeight = ((float) m_elementMetrics[element.getType()][element.getRotation()][1] / BOARD_HEIGHT)* 2.0f;
	
	float topX = elementLeft;
	float topY = elementBottom + elementHeight;
	float bottomX = elementLeft + elementWidth;
	float bottomY = elementBottom;

	Engine::get().renderRectangle(textureId, 0, topX, topY, bottomX, bottomY);
}

void Board::render()
{
	Engine::get().renderRectangle(m_backgroundTexture, 0, -1, 1, 1, -1);

	Element test(ELEMENT_TYPE::LONG4,Position(0,0),ELEMENT_ROTATION::UP);
	renderElement(test);
}

void Board::addElement(Element element)
{
    element.setId(++currentElementId);
    this->currentElement = element;
}

bool Board::isMovementPossible(Element element, Position newPosition)
{
 

    return true;
}

static Board* g_board = nullptr;
Board& Board::get()
{
	if (!g_board)
	{
		g_board = new Board;
	}

	return *g_board;
}