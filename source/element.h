#ifndef ELEMENT_H
#define ELEMENT_H
/*
enum ELEMENT_TYPE
{
    LONG4,
    CORNER3,
    SMALL2,
    EMPTY
};*/

enum ELEMENT_TYPE
{
    LONG4,
    CORNER3,
    SMALL2,
    CUBE,
    PYRAMID,
    EMPTY
};

enum ELEMENT_ROTATION
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
	COUNT
};

struct Position
{
    int x;
    int y;

	Position() :x(0), y(0){}
	Position(int _x, int _y) :x(_x), y(_y){}
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

    Position getPosition() const;
    void setPosition(Position);

	ELEMENT_TYPE getType() const { return m_type; }
	ELEMENT_ROTATION getRotation() const { return m_rotation; }

	void render();
};

#endif ELEMENT_H