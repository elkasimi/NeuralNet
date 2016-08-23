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
const auto TICK_TIME = std::chrono::milliseconds( 4 );
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
    double error = 0.0;
    std::vector< double > inputs;
    inputs.push_back( 0.0 );
    inputs.push_back( 0.0 );
    double a = neural_net.activate( inputs );
    error += 10000 * a * a;
    //////////////////
    inputs.clear( );
    inputs.push_back( 1.0 );
    inputs.push_back( 1.0 );
    a = neural_net.activate( inputs );
    error += 10000 * a * a;
    //////////////////
    inputs.clear( );
    inputs.push_back( 0.0 );
    inputs.push_back( 1.0 );
    a = neural_net.activate( inputs );
    error += 10000 * ( a - 1 ) * ( a - 1 );
    //////////////////
    inputs.clear( );
    inputs.push_back( 1.0 );
    inputs.push_back( 0.0 );
    a = neural_net.activate( inputs );
    error += 10000 * ( a - 1 ) * ( a - 1 );
    double fitness = 1 / error;
    neural_net.set_fitness( fitness );
}

void
run_simulation( )
{
    std::vector< NeuralNet > base;

    for ( const auto& name : names )
    {
        base.push_back( NeuralNet::load( name ) );
    }

    int32_t epoches;
    std::cout << "Please enter the number of generations:" << std::endl;
    std::cin >> epoches;

    for ( int32_t g = 0; g < epoches; ++g )
    {
        std::cout << "Generation " << g + 1 << std::endl;
        std::vector< NeuralNet > neural_nets;
        for ( int32_t i = 0; i < N; ++i )
        {
            for ( int32_t j = i + 1; j < N; ++j )
            {
                NeuralNet first_baby = base[ i ] * base[ j ];
                NeuralNet second_baby = base[ j ] * base[ i ];
                first_baby.mutate( );
                second_baby.mutate( );
                neural_nets.push_back( first_baby );
                neural_nets.push_back( second_baby );
            }
        }

        std::cerr << neural_nets.size( ) << " babies created" << std::endl;

        for ( auto& neural_net : neural_nets )
        {
            std::cerr << neural_net << std::endl;

            Position position( WIDTH, HEIGTH );
            while ( !position.end_game( ) )
            {
                Direction dir = AI::get_best_direction( position, neural_net );
                position.set_direction( dir );
                position.move( );
                // position.display();
            }
            double fitness
                = position.get_score( ) + 0.001 * position.get_life( );
            neural_net.set_fitness( fitness );
        }

        std::sort( neural_nets.begin( ), neural_nets.end( ),
                   []( const NeuralNet& lhs, const NeuralNet& rhs ) {
                       return lhs.get_fitness( ) > rhs.get_fitness( );
                   } );

        base = std::vector< NeuralNet >( neural_nets.begin( ),
                                         neural_nets.begin( ) + N );

        int32_t count = 0;
        for ( const auto& neural_net : base )
        {
            neural_net.save( names[ count ] );

            ++count;
        }

        if ( g == epoches - 1 )
        {
            std::cout << "Player\t\tFitness" << std::endl;
            int32_t i = 0;
            for ( const auto& neural_net : base )
            {
                std::cout << ++i << "\t\t" << neural_net.get_fitness( )
                          << std::endl;
            }
        }
    }
}

int32_t
main( )
{
    std::string action;
    std::cout << "Please enter action (init/simulation/game) :" << std::endl;
    std::cin >> action;

    while ( action != "init" && action != "simulation" && action != "game" )
    {
        std::cout << "Please enter action (init/simulation/game) :"
                  << std::endl;
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
        NeuralNet nn = NeuralNet::load( names[ 0 ] );
        Position pos( WIDTH, HEIGTH );
        while ( !pos.end_game( ) )
        {
            Direction dir = AI::get_best_direction( pos, nn );
            pos.set_direction( dir );
            pos.move( );
            pos.display( );
            std::this_thread::sleep_for( TICK_TIME );
        }
        std::cout << "score = " << pos.get_score( )
                  << ", life = " << pos.get_life( ) << std::endl;
    }

    return 0;
}
