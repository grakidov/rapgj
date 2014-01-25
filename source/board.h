#ifndef BOARD_H
#define BOARD_H

class Board {
private:
    int weight;
    int height;

    int texture;

public:
    Board();
    void render();
};

#endif BOARD_H