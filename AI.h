#ifndef _AI_H
#define _AI_H

#include "NeuralNet.h"
#include "Position.h"

class AI {
public:
    static Direction GetBestDirection(Position &pos, NeuralNet &nn);
};

#endif
