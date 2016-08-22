#pragma once

class Position;
class NeuralNet;

enum class Direction;

class AI
{
public:
    static Direction get_best_direction( const Position& position,
                                         const NeuralNet& nueral_net );
};
