#ifndef POSITION_H
#define POSITION_H

#include <vector>

enum Direction {
    RIGHT = 0,
    LEFT,
    UP,
    DOWN,
    PASS
};

struct Square {
    Square()
    {}
    Square(int x, int y) :
        x(x), y(y)
    {}
    bool operator <(const Square &rhs) const {
        if (x != rhs.x) {
            return x < rhs.x;
        } else {
            return y < rhs.y;
        }
    }
    int x;
    int y;
};

class Position {
public:
    Position(int width, int heigth) :
        fWidth(width), fHeigth(heigth), fDirection(RIGHT), fScore(0), fLife(0) {
        fSnake = Square(0, 0);
        int startFoodX = fWidth / 2;
        int startFoodY = fHeigth / 2;
        fFood = Square(startFoodX, startFoodY);
    }

    int GetWidth() {
        return fWidth;
    }
    int GetHeigth() {
        return fHeigth;
    }
    int GetDirection() {
        return fDirection;
    }
    int GetScore() {
        return fScore;
    }
    int GetLife() {
        return fLife;
    }
    void SetDirection(Direction dir) {
        if(dir != PASS) {
            fDirection = dir;
        }
    }
    void Move();
    void GenerateNewFood();
    void Display();
    bool EndGame();
    std::vector<double> GetNNInput();
    std::vector<Direction> GetPossibleDirections();

private:
    Square fFood;
    Square fSnake;
    int fWidth;
    int fHeigth;
    Direction fDirection;
    int fScore;
    int fLife;
    static int fMaxLife;
};

#endif
