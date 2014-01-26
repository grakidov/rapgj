#include "ra_pch.h"
#include "animatedElement.h"

#include "engine.h"

AnimatedElement::AnimatedElement()
{
    
}

int AnimatedElement::getId() const
{
    return m_id;
}

void AnimatedElement::setId(int id)
{
    m_id = id;
}

Position AnimatedElement::getPosition() const
{
    return m_position;
}

void AnimatedElement::setPosition(Position position)
{
    m_position = position;
}

void AnimatedElement::renderFrame(ELEMENT_ROTATION rotation, int frameNumber)
{
    int textureId = m_frames[rotation].at(frameNumber);


    float elementBottom = ((float)m_position.y / m_boardHeight)* 2.0f - 1.0f;
    float elementLeft = ((float)m_position.x / m_boardWidth) * 2.0f - 1.0f;

    float elementWidth = ((float)m_elementHeight / m_boardHeight)* 2.0f;
    float elementHeight = ((float)m_elementWidth / m_boardWidth)* 2.0f;

    float topX = elementLeft;
    float topY = elementBottom + elementHeight;
    float bottomX = elementLeft + elementWidth;
    float bottomY = elementBottom;

    Engine::get().renderRectangle(textureId, 0, topX, topY, bottomX, bottomY);
}

void AnimatedElement::addFrameTexture(ELEMENT_ROTATION rotation, int textureId)
{
    m_frames[rotation].push_back(textureId);
}

void AnimatedElement::renderAnimation(int delayBetweenFrames)
{
}

