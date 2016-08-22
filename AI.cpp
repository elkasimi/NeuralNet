#include "AI.h"
#include "NeuralNet.h"
#include "Position.h"

#include <limits>

Direction
AI::get_best_direction( const Position& position, const NeuralNet& nueral_net )
{
    const auto directions = position.get_possible_directions( );
    Direction best_direction = PASS;
    double best_value = -std::numeric_limits< double >::infinity( );

    for ( auto direction : directions )
    {
        auto tmp_position = position;
        tmp_position.set_direction( direction );
        tmp_position.move( );
        const auto v = tmp_position.get_neural_net_input( );
        double value = nueral_net.activate( v );
        if ( best_value < value )
        {
            best_value = value;
            best_direction = direction;
        }
    }

    return best_direction;
}
