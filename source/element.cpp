#include "ra_pch.h"
#include "element.h"

Element::Element()
{
    m_type = EMPTY;
}

Element::Element(ELEMENT_TYPE type, Position position, ELEMENT_ROTATION rotation)
{
    m_type = type;
    m_position = position;
    m_rotation = rotation;
}

int Element::getId()
{
    return m_id;
}

void Element::setId(int id)
{
    m_id = id;
}

Position Element::getPosition()
{
    return m_position;
}

void Element::setPosition(Position position)
{
    m_position = position;
}

void Element::render()
{
}

