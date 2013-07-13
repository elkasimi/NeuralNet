#include "AI.h"

Direction AI::GetBestDirection(Position &pos, NeuralNet &nn) {
    std::vector<Direction> directions = pos.GetPossibleDirections();
    Direction bestDirection = PASS;
    double bestValue = -1E9;
    int dirSize = (int) directions.size();
    for (int i = 0; i < dirSize; ++i) {
        Position tmp = pos;
        Direction &dir = directions[i];
        tmp.SetDirection(dir);
        tmp.Move();
        std::vector<double> v = tmp.GetNNInput();
        double value = nn.Activate(v);
        if (bestValue < value) {
            bestValue = value;
            bestDirection = dir;
        }
    }

    return bestDirection;
}
