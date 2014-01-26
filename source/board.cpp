#include "ra_pch.h"

#include <vector>
#include <algorithm>

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

    // ELEMENT_TYPE::LONG4
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

    // ELEMENT_TYPE::CORNER3
    m_elementTextures[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::UP] = Engine::get().loadTexture("../content/block_2_pos1.tga", 1, 1);
    if (m_elementTextures[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::UP] == -1)
    {
        return false;
    }

    m_elementTextures[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::DOWN] = Engine::get().loadTexture("../content/block_2_pos3.tga", 1, 1);
    if (m_elementTextures[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::DOWN] == -1)
    {
        return false;
    }

    m_elementTextures[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::RIGHT] = Engine::get().loadTexture("../content/block_2_pos2.tga", 1, 1);
    if (m_elementTextures[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::RIGHT] == -1)
    {
        return false;
    }

    m_elementTextures[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::LEFT] = Engine::get().loadTexture("../content/block_2_pos4.tga", 1, 1);
    if (m_elementTextures[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::LEFT] == -1)
    {
        return false;
    }

    m_elementMetrics[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::UP][0] = 2;
    m_elementMetrics[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::UP][1] = 2;

    m_elementMetrics[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::DOWN][0] = 2;
    m_elementMetrics[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::DOWN][1] = 2;

    m_elementMetrics[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::RIGHT][0] = 2;
    m_elementMetrics[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::RIGHT][1] = 2;

    m_elementMetrics[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::LEFT][0] = 2;
    m_elementMetrics[ELEMENT_TYPE::CORNER3][ELEMENT_ROTATION::LEFT][1] = 2;

    // ELEMENT_TYPE::SMALL2
    m_elementTextures[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::UP] = Engine::get().loadTexture("../content/block_3_pos1.tga", 1, 1);
    if (m_elementTextures[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::UP] == -1)
    {
        return false;
    }

    m_elementTextures[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::DOWN] = m_elementTextures[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::UP];

    m_elementTextures[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::RIGHT] = Engine::get().loadTexture("../content/block_3_pos2.tga", 1, 1);
    if (m_elementTextures[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::RIGHT] == -1)
    {
        return false;
    }

    m_elementTextures[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::LEFT] = m_elementTextures[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::RIGHT];

    m_elementMetrics[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::UP][0] = 1;
    m_elementMetrics[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::UP][1] = 2;

    m_elementMetrics[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::DOWN][0] = 1;
    m_elementMetrics[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::DOWN][1] = 2;

    m_elementMetrics[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::RIGHT][0] = 2;
    m_elementMetrics[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::RIGHT][1] = 1;

    m_elementMetrics[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::LEFT][0] = 2;
    m_elementMetrics[ELEMENT_TYPE::SMALL2][ELEMENT_ROTATION::LEFT][1] = 1;

    // CUBE
    m_elementTextures[ELEMENT_TYPE::CUBE][ELEMENT_ROTATION::UP] = Engine::get().loadTexture("../content/cube.tga", 1, 1);
    if (m_elementTextures[ELEMENT_TYPE::CUBE][ELEMENT_ROTATION::UP] == -1)
    {
        return false;
    }

    m_elementMetrics[ELEMENT_TYPE::CUBE][ELEMENT_ROTATION::UP][0] = 6;
    m_elementMetrics[ELEMENT_TYPE::CUBE][ELEMENT_ROTATION::UP][1] = 6;

    // PYRAMID
    m_elementTextures[ELEMENT_TYPE::PYRAMID][ELEMENT_ROTATION::UP] = Engine::get().loadTexture("../content/pyramid.tga", 1, 1);
    if (m_elementTextures[ELEMENT_TYPE::PYRAMID][ELEMENT_ROTATION::UP] == -1)
    {
        return false;
    }

    m_elementMetrics[ELEMENT_TYPE::PYRAMID][ELEMENT_ROTATION::UP][0] = 6;
    m_elementMetrics[ELEMENT_TYPE::PYRAMID][ELEMENT_ROTATION::UP][1] = 6;

	return true;
}

void Board::clear()
{
    for (int row = 0; row < BOARD_WIDTH; row++)
    {
        for (int column = 0; column < BOARD_HEIGHT; column++)
        {
			boardArray[row][column] = Cell();
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

    testAllElements();
}

void Board::addElement(Element element)
{
    element.setId(++currentElementId);
    this->currentElement = element;

	m_elements[element.getId()] = element;
}

bool Board::isMovementPossible(Element element, Position newPosition)
{
	int width = m_elementMetrics[ELEMENT_TYPE::EMPTY][ELEMENT_ROTATION::COUNT][0];
	for (int cell = 0; cell < width; cell++)
	{
		Position cellPos = newPosition;
		cellPos.x += cell;
		if (elementShapes[element.getType()][element.getRotation()][3][cell] && boardArray[cellPos.x][cellPos.y].m_elementId != -1)
		{
			return false;
		}
	}

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

void Board::testAllElements()
{
    std::vector<Element> elements;

    elements.push_back(Element(ELEMENT_TYPE::LONG4, Position(1, 4), ELEMENT_ROTATION::UP));
    elements.push_back(Element(ELEMENT_TYPE::LONG4, Position(3, 4), ELEMENT_ROTATION::DOWN));
    elements.push_back(Element(ELEMENT_TYPE::LONG4, Position(5, 4), ELEMENT_ROTATION::LEFT));
    elements.push_back(Element(ELEMENT_TYPE::LONG4, Position(10, 4), ELEMENT_ROTATION::RIGHT));
    elements.push_back(Element(ELEMENT_TYPE::CORNER3, Position(1, 10), ELEMENT_ROTATION::UP));
    elements.push_back(Element(ELEMENT_TYPE::CORNER3, Position(4, 10), ELEMENT_ROTATION::DOWN));
    elements.push_back(Element(ELEMENT_TYPE::CORNER3, Position(7, 10), ELEMENT_ROTATION::LEFT));
    elements.push_back(Element(ELEMENT_TYPE::CORNER3, Position(10, 10), ELEMENT_ROTATION::RIGHT));
    elements.push_back(Element(ELEMENT_TYPE::SMALL2, Position(1, 14), ELEMENT_ROTATION::UP));
    elements.push_back(Element(ELEMENT_TYPE::SMALL2, Position(3, 14), ELEMENT_ROTATION::DOWN));
    elements.push_back(Element(ELEMENT_TYPE::SMALL2, Position(5, 14), ELEMENT_ROTATION::LEFT));
    elements.push_back(Element(ELEMENT_TYPE::SMALL2, Position(8, 14), ELEMENT_ROTATION::RIGHT));
    elements.push_back(Element(ELEMENT_TYPE::CUBE, Position(14, 20), ELEMENT_ROTATION::UP));
    elements.push_back(Element(ELEMENT_TYPE::PYRAMID, Position(42, 20), ELEMENT_ROTATION::UP));

    for (size_t i = 0; i < elements.size(); i++)
        renderElement(elements[i]);
    
}

/*
void justSomething()
{
    // CUBE
    m_targetTextures[CUBE_SPRITE_INDEX + 0] = Engine::get().loadTexture("../content/cube.tga", 1, 1);
    m_targetTextures[CUBE_SPRITE_INDEX + 1] = Engine::get().loadTexture("../content/cube_sprite_1.tga", 1, 1);
    m_targetTextures[CUBE_SPRITE_INDEX + 2] = Engine::get().loadTexture("../content/cube_sprite_2.tga", 1, 1);
    m_targetTextures[CUBE_SPRITE_INDEX + 3] = Engine::get().loadTexture("../content/cube_sprite_3.tga", 1, 1);
    m_targetTextures[CUBE_SPRITE_INDEX + 4] = Engine::get().loadTexture("../content/cube_sprite_4.tga", 1, 1);
    m_targetTextures[CUBE_SPRITE_INDEX + 5] = Engine::get().loadTexture("../content/cube_sprite_5.tga", 1, 1);
    m_targetTextures[CUBE_SPRITE_INDEX + 6] = Engine::get().loadTexture("../content/cube_sprite_6.tga", 1, 1);
    m_targetTextures[CUBE_SPRITE_INDEX + 7] = Engine::get().loadTexture("../content/cube_sprite_7.tga", 1, 1);

    // PYRAMID
    m_targetTextures[PYRAMID_SPRITE_INDEX + 0] = Engine::get().loadTexture("../content/pyramid.tga", 1, 1);
    m_targetTextures[PYRAMID_SPRITE_INDEX + 1] = Engine::get().loadTexture("../content/pyramid.tga", 1, 1);
    m_targetTextures[PYRAMID_SPRITE_INDEX + 2] = Engine::get().loadTexture("../content/pyramid.tga", 1, 1);
    m_targetTextures[PYRAMID_SPRITE_INDEX + 3] = Engine::get().loadTexture("../content/pyramid.tga", 1, 1);
    m_targetTextures[PYRAMID_SPRITE_INDEX + 4] = Engine::get().loadTexture("../content/pyramid.tga", 1, 1);
    m_targetTextures[PYRAMID_SPRITE_INDEX + 5] = Engine::get().loadTexture("../content/pyramid.tga", 1, 1);
    m_targetTextures[PYRAMID_SPRITE_INDEX + 6] = Engine::get().loadTexture("../content/pyramid.tga", 1, 1);
    m_targetTextures[PYRAMID_SPRITE_INDEX + 7] = Engine::get().loadTexture("../content/pyramid.tga", 1, 1);
= Engine::get().loadTexture("../content/block_1_sprite_1_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_1_sprite_1_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_1_sprite_2_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_1_sprite_2_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_1_sprite_3_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_1_sprite_3_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_1_sprite_4_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_1_sprite_4_pos2.tga, 1, 1);

= Engine::get().loadTexture("../content/block_2_sprite_1_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_1_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_1_pos3.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_1_pos4.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_2_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_2_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_2_pos3.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_2_pos4.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_3_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_3_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_3_pos3.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_3_pos4.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_4_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_4_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_4_pos3.tga, 1, 1);
= Engine::get().loadTexture("../content/block_2_sprite_4_pos4.tga, 1, 1);

= Engine::get().loadTexture("../content/block_3_sprite_1_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_3_sprite_1_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_3_sprite_2_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_3_sprite_2_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_3_sprite_3_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_3_sprite_3_pos2.tga, 1, 1);
= Engine::get().loadTexture("../content/block_3_sprite_4_pos1.tga, 1, 1);
= Engine::get().loadTexture("../content/block_3_sprite_4_pos2.tga, 1, 1);
}
*/