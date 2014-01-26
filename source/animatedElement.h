#ifndef ANIMATEDELEMENT_H
#define ANIMATEDELEMENT_H

#include <vector>

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

class AnimatedElement
{
private:
    int m_id;
    ELEMENT_TYPE m_type;
    Position m_position;
    ELEMENT_ROTATION m_rotation;

    int m_boardWidth;
    int m_boardHeight;

    int m_elementWidth;
    int m_elementHeight;

    vector<int> m_frames[ELEMENT_ROTATION::COUNT];

public:
    AnimatedElement();
    AnimatedElement(ELEMENT_TYPE type, Position position, ELEMENT_ROTATION rotation,
        int elementWidth, int elementHeight,
        int boardWidth, int boardHeight) :
        m_type(type),
        m_position(position),
        m_rotation(rotation),
        m_elementWidth(elementWidth),
        m_elementHeight(elementHeight),
        m_boardWidth(boardWidth),
        m_boardHeight(boardHeight){};

    int getId() const;
    void setId(int id);

    Position getPosition() const;
    void setPosition(Position);

	ELEMENT_TYPE getType() const { return m_type; }

	ELEMENT_ROTATION getRotation() const { return m_rotation; }
	void setRotation(ELEMENT_ROTATION rotation) { m_rotation = rotation; }


    void addFrameTexture(ELEMENT_ROTATION rotation, int textureId);

    void renderFrame(ELEMENT_ROTATION rotation, int frameNumber);
    void renderAnimation(int delayBetweenFrames);
};

#endif ANIMATEDELEMENT_H