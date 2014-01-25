#include "ra_pch.h"
#include "board.h"

#include "element_shapes.h"

Board::Board()
{
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

void Board::render()
{
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