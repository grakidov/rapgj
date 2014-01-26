#include "ra_pch.h"

#include "player.h"

#include <stdlib.h>
#include <time.h>

#include "element.h"
#include "board.h"

Player::Player(PLAYER_ID playerId) :
m_breakWaitTime(1.0f),
m_fallStepTime(0.5f),
m_fallSpeed(1.0f),
m_state(PLAYER_STATE::GENERATE)
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

void Player::update(float time)
{
	switch (m_state)
	{
	case GENERATE:
	{
		Element element = generateElement();
		m_board->addElement(element);
		//m_board->placeElement(element);
		m_fallElementId = element.getId();
		m_fallStartTime = time;
		m_fallStartPos = element.getPosition();
		m_state = FALL;
	}
		break;
	case FALL:
	{
		float fallTime = time - m_fallStartTime;
		Position newPos = m_fallStartPos;
		newPos.y -= (int)(fallTime*m_fallSpeed);
		if (newPos.y < 1)
		{
			newPos.y = 1;
		}

		Element* element = m_board->getElement(m_fallElementId);
		assert(element);

		if (m_input[PLAYER_INPUT::IN_RIGHT])
		{
			newPos.x++;
		}
		else if (m_input[PLAYER_INPUT::IN_LEFT])
		{
			newPos.x--;
		}
		else if (m_input[PLAYER_INPUT::IN_ROT])
		{
			if (element)
			{
				int rot = element->getRotation();
				rot = rot + 1;
				if (rot == ELEMENT_ROTATION::COUNT)
				{
					rot = ELEMENT_ROTATION::UP;
				}
				element->setRotation((ELEMENT_ROTATION) rot);
			}
		}

		//collide with vertical boreders
		if (newPos.x < 0)
		{
			newPos.x = 0;
		}
		
		int elementWidth, elementHeight;
		m_board->getElementMetrics(element->getType(), element->getRotation(), elementWidth, elementHeight);
		if ((newPos.x + elementWidth) > BOARD_WIDTH)
		{
			newPos.x = BOARD_WIDTH - elementWidth;
		}

		
		if (element)
		{
			if (!m_board->isMovementPossible(*element, newPos))
			{
				m_elementsToBreak.clear();
				m_board->breakTest(*element, m_elementsToBreak);
				if (m_elementsToBreak.size())
				{
					m_breakStartTime = time;
					
					m_state = BREAK;
				}
				else
				{
					m_board->placeElement(*element);
				}
			}
			else
			{
				element->setPosition(newPos);
				if (newPos.y == 1)
				{
					m_board->placeElement(*element);
					m_state = GENERATE;
				}
			}
		}

		for (int i = 0; i < PLAYER_INPUT::IN_COUNT; i++)
		{
			m_input[i] = false;
		}

	}
		break;
	case BREAK:
	{
		if (time - m_breakStartTime >= m_breakWaitTime)
		{
			for (auto it = m_elementsToBreak.begin(); it != m_elementsToBreak.end(); it++)
			{
				Element* toRemove = m_board->getElement(*it);
				if (toRemove)
				{
					m_board->removeElement(*toRemove);
				}
			}

			m_fallStepStartTime = time;
			m_state = FALL_STEP;
		}
	}
		break;
	case FALL_STEP:
	{
		if ((time - m_fallStepStartTime) >= m_fallStepTime)
		{
			m_elementsToBreak.clear();
			Element* element = m_board->getElement(m_fallElementId);
			assert(element);
			if (element)
			{
				Position newPos = element->getPosition();
				newPos.y -= 1;
				if (newPos.y > 1)
				{
					m_board->breakTest(*element, m_elementsToBreak);
				}

				if (m_elementsToBreak.size())
				{
					m_breakStartTime = time;
					m_state = BREAK;
				}
				else
				{
					m_board->placeElement(*element);
					m_state = GENERATE;
				}
			}
		}
	}
		break;
	}
}