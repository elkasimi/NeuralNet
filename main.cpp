#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "AI.h"
#include "NeuralNet.h"
#include "Position.h"

namespace
{
const int32_t NEURAL_NET_INPUTS = 8;
const int32_t WIDTH = 10;
const int32_t HEIGTH = 10;
const int32_t N = 10;
const auto TICK_TIME = std::chrono::milliseconds( 80 );
const auto EPOCHES_BETWEEN_DISPLAYS = 1;
const std::vector< std::string > names{
    "game0.nn", "game1.nn", "game2.nn", "game3.nn", "game4.nn",
    "game5.nn", "game6.nn", "game7.nn", "game8.nn", "game9.nn",
};
}

void
init_simulation( )
{
    for ( const auto& name : names )
    {
        NeuralNet neural_net( NEURAL_NET_INPUTS );
        neural_net.save( name );
    }
}

void
compute_xor_error( NeuralNet& neural_net )
{
    auto error = 1.0;

    auto a = neural_net.activate( {0.0, 0.0} );
    error += 10000.0 * a * a;

    a = neural_net.activate( {1.0, 1.0} );
    error += 10000.0 * a * a;

    a = neural_net.activate( {0.0, 1.0} );
    error += 10000.0 * ( a - 1 ) * ( a - 1 );

    a = neural_net.activate( {1.0, 0.0} );
    error += 10000.0 * ( a - 1 ) * ( a - 1 );

    auto fitness = 1.0 / error;
    neural_net.set_fitness( fitness );
}

void
run_simulation( )
{
    std::vector< NeuralNet > best_neural_nets;

    for ( const auto& name : names )
    {
        best_neural_nets.push_back( NeuralNet::load( name ) );
    }

    int32_t epoches;
    std::cout << "Please enter the number of generations:" << std::endl;
    std::cin >> epoches;

    for ( int32_t g = 0; g < epoches; ++g )
    {
        std::vector< NeuralNet > neural_nets;
        for ( const auto& first_neural_net : best_neural_nets )
        {
            for ( const auto& second_neural_net : best_neural_nets )
            {
                auto first_baby = first_neural_net * second_neural_net;
                auto second_baby = second_neural_net * first_neural_net;
                first_baby.mutate( );
                second_baby.mutate( );
                neural_nets.push_back( first_baby );
                neural_nets.push_back( second_baby );
            }
        }

        neural_nets.insert( neural_nets.cend( ), best_neural_nets.cbegin( ),
                            best_neural_nets.cend( ) );

        for ( auto& neural_net : neural_nets )
        {
            Position position( WIDTH, HEIGTH );
            while ( !position.end_game( ) )
            {
                const auto direction = AI::get_best_direction( position, neural_net );
                position.set_direction( direction );
                position.move( );
                // position.display();
            }

            auto fitness = position.get_score( ) + 0.001 * position.get_life( );
            neural_net.set_fitness( fitness );
        }

        std::sort( neural_nets.begin( ), neural_nets.end( ),
                   []( const NeuralNet& lhs, const NeuralNet& rhs ) {
                       return lhs.get_fitness( ) > rhs.get_fitness( );
                   } );

        best_neural_nets
            = std::vector< NeuralNet >( neural_nets.begin( ), neural_nets.begin( ) + N );

        auto name_iterator = names.cbegin( );
        for ( const auto& neural_net : best_neural_nets )
        {
            neural_net.save( *name_iterator++ );
        }

        if ( ( g + 1 ) % EPOCHES_BETWEEN_DISPLAYS == 0 )
        {
            std::cout << "Generation " << g + 1 << std::endl;
            for ( const auto& neural_net : best_neural_nets )
            {
                std::cerr << neural_net.get_fitness( ) << " ";
            }
            std::cerr << std::endl;
        }

        if ( g == epoches - 1 )
        {
            std::cout << "Player\t\tFitness" << std::endl;
            int32_t i = 0;
            for ( const auto& neural_net : best_neural_nets )
            {
                std::cout << ++i << "\t\t" << neural_net.get_fitness( ) << std::endl;
            }
        }
    }
}

int
main( int argc, char* argv[] )
{
    std::string action;
    std::cout << "Please enter action (init/simulation/game) :" << std::endl;
    std::cin >> action;

    while ( action != "init" && action != "simulation" && action != "game" )
    {
        std::cout << "Please enter action (init/simulation/game) :" << std::endl;
        std::cin >> action;
    }

    if ( action == "init" )
    {
        init_simulation( );
    }
    else if ( action == "simulation" )
    {
        run_simulation( );
    }
    else
    {
        auto neural_net = NeuralNet::load( names.front( ) );
        Position position( WIDTH, HEIGTH );
        while ( !position.end_game( ) )
        {
            auto direction = AI::get_best_direction( position, neural_net );
            position.set_direction( direction );
            position.move( );
            position.display( );
            std::this_thread::sleep_for( TICK_TIME );
        }
        std::cout << "score = " << position.get_score( ) << ", life = " << position.get_life( )
                  << std::endl;
    }

    return 0;
}
