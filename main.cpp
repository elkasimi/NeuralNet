#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "AI.h"
#include "NeuralNet.h"
#include "Position.h"

namespace
{
const bool XOR_TEST = true;
const int32_t NEURAL_NET_INPUTS = XOR_TEST ? 2 : 8;
const int32_t WIDTH = 10;
const int32_t HEIGTH = 10;
const int32_t N = 10;
const auto TICK_TIME = std::chrono::milliseconds( 80 );
const auto EPOCHES_BETWEEN_DISPLAYS = 1000;
const std::vector< std::string > names{
    "candidate0.nn", "candidate1.nn", "candidate2.nn", "candidate3.nn", "candidate4.nn",
    "candidate5.nn", "candidate6.nn", "candidate7.nn", "candidate8.nn", "candidate9.nn"};
const auto THRESHOLD = 0.5;
struct XORData
{
    std::vector< double > input;
    double output;
};
const std::vector< XORData > XOR_TESTS{
    {{0.0, 0.0}, 0.0}, {{0.0, 1.0}, 1.0}, {{1.0, 0.0}, 1.0}, {{1.0, 1.0}, 0.0}};

void
init_simulation( )
{
    for ( const auto& name : names )
    {
        NeuralNet( NEURAL_NET_INPUTS ).save( name );
    }
}

void
compute_xor_fitness( NeuralNet& neural_net )
{
    auto error = 0.0;
    for ( const auto& xor_data : XOR_TESTS )
    {
        if ( neural_net.activate( xor_data.input ) >= THRESHOLD )
        {
            error += std::fabs( xor_data.output - 1.0 );
        }
        else
        {
            error += std::fabs( xor_data.output );
        }
    }

    auto fitness = -error;
    neural_net.set_fitness( fitness );
}

void
compute_snake_game_fitness( NeuralNet& neural_net )
{
    Position position( WIDTH, HEIGTH );
    while ( !position.end_game( ) )
    {
        const auto direction = AI::get_best_direction( position, neural_net );
        position.set_direction( direction );
        position.move( );
    }

    auto fitness = position.get_score( ) + 0.01 * position.get_life( );
    neural_net.set_fitness( fitness );
}

void
test_snake_game( const NeuralNet& neural_net )
{
    Position position( WIDTH, HEIGTH );
    while ( !position.end_game( ) )
    {
        auto direction = AI::get_best_direction( position, neural_net );
        position.set_direction( direction );
        position.move( );
        position.display( );
        std::this_thread::sleep_for( TICK_TIME );
    }
}

void
test_xor( const NeuralNet& neural_net )
{
    for ( const auto& xor_data : XOR_TESTS )
    {
        std::cout << "(" << xor_data.input[ 0 ] << ", " << xor_data.input[ 1 ] << ") "
                  << ( neural_net.activate( xor_data.input ) >= THRESHOLD ) << std::endl;
    }
}

using FitnessFunction = std::function< void( NeuralNet& ) >;
FitnessFunction compute_fitness = XOR_TEST ? &compute_xor_fitness : &compute_snake_game_fitness;

using TestFunction = std::function< void( const NeuralNet& ) >;
TestFunction test_function = XOR_TEST ? &test_xor : &test_snake_game;

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
        std::cout << "Generation " << g + 1 << std::endl;
        std::vector< NeuralNet > neural_nets;
        for ( const auto& first_neural_net : best_neural_nets )
        {
            for ( const auto& second_neural_net : best_neural_nets )
            {
                auto baby = first_neural_net * second_neural_net;
                baby.mutate( );
                neural_nets.push_back( baby );
            }
        }

        neural_nets.insert( neural_nets.cend( ), best_neural_nets.cbegin( ),
                            best_neural_nets.cend( ) );

        for ( auto& neural_net : neural_nets )
        {
            compute_fitness( neural_net );
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
}

int
main( int argc, char* argv[] )
{
    std::string action;
    std::cout << "Please enter action (init/simulation/test) :" << std::endl;
    std::cin >> action;

    while ( action != "init" && action != "simulation" && action != "test" )
    {
        std::cout << "Please enter action (init/simulation/test) :" << std::endl;
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
        const auto neural_net = NeuralNet::load( names.front( ) );
        test_function( neural_net );
    }

    return 0;
}
