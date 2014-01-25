#ifndef ELEMENT_H
#define ELEMENT_H

enum ELEMENT_TYPE
{
    LONG4,
    CORNER3,
    SMALL2,
    EMPTY
};

enum ELEMENT_ROTATION
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Position
{
    int x;
    int y;
};

class Element
{
private:
    int m_id;
    ELEMENT_TYPE m_type;
    Position m_position;
    ELEMENT_ROTATION m_rotation;

public:
    Element();
	Element(ELEMENT_TYPE type, Position position, ELEMENT_ROTATION rotation);

    int getId();
    void setId(int id);

    Position getPosition();
    void setPosition(Position);

	void render();
};

#endif ELEMENT_H