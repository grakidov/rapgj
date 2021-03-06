#ifndef ELEMENT_SHAPES_H
#define ELEMENT_SHAPES_H

#define SHAPE_TYPE 3
#define ROTATION 4
#define SHAPE_WIDTH 4
#define SHAPE_HEIGHT 4


int elementShapes[SHAPE_TYPE][ROTATION][SHAPE_HEIGHT][SHAPE_WIDTH] =
{
    // LONG4
    {
        // 0
        {
            { 1, 0, 0, 0 },
			{ 2, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 },
        },
        // 90
        {
			{ 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 1, 2, 1, 1 }
        },
        // 180
        {
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 2, 0, 0, 0 },
			{ 1, 0, 0, 0 },
        },
        // 270
        {
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 1, 1, 2, 1 }
        },
    },
    // CORNER3
    {
        // 0
        {
			{ 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 1, 2, 0, 0 },
        },
        // 90
        {
			{0, 0, 0, 0},
			{ 0, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 2, 1, 0, 0 }
        },
        // 180
        {
            {0, 0, 0, 0 },
            {0, 0, 0, 0 },
            {2, 1, 0, 0 },
            {1, 0, 0, 0 }
        },
        // 270
        {
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 1, 2, 0, 0 },
            { 0, 1, 0, 0 }
        }
    },
    // SMALL2
    {
        // 0
        {
			{ 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 1, 0, 0, 0 },
            { 2, 0, 0, 0 }
        },
        // 90
        {
			{ 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 2, 1, 0, 0 }
        },
        // 180
        {
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 2, 0, 0, 0 }
        },
        // 270
		{
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 1, 2, 0, 0 }
        }
    }
};

#endif ELEMENT_SHAPES_H