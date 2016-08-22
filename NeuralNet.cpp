#include "NeuralNet.h"

#include <cmath>
#include <fstream>
#include <iostream>

namespace
{
const double MUTATION_RATE = 0.12;
const double MAX_PERTURBATION = 0.3;

double
get_random_number( )
{
    double res = rand( );
    res /= RAND_MAX;
    return res;
}

double
get_random_weight( )
{
    double r = get_random_number( );
    double res = 2 * r - 1;
    return res;
}

double
tan_h( double x )
{
    if ( x > +20.0 )
    {
        return +1.0;
    }
    else if ( x < -20.0 )
    {
        return -1.0;
    }
    else
    {
        return tan_h( x );
    }
}

double
sigmoid( double x )
{
    if ( x > +20.0 )
    {
        return +1.0;
    }
    else if ( x < -20.0 )
    {
        return 0.0;
    }
    else
    {
        double res = 1 / ( 1 + std::exp( -x ) );
        return res;
    }
}

bool
are_equal_with_epsilon( double x, double y )
{
    const double eps = 1e-15;
    return fabs( x - y ) < eps;
}
}

NeuralNet::NeuralNet( int num_inputs, int num_hidden )
{
    m_num_inputs = num_inputs;
    if ( num_hidden == 0 )
    {
        num_hidden = num_inputs / 2 + 1;
    }
    m_num_hidden = num_hidden;
    m_fitness = 0.0;
    int totalWeights = ( m_num_inputs + 1 ) * m_num_hidden + m_num_hidden + 1;
    for ( int i = 0; i < totalWeights; ++i )
    {
        m_weights.push_back( get_random_weight( ) );
    }
}

int
NeuralNet::get_num_inputs( ) const
{
    return m_num_inputs;
}

int
NeuralNet::get_num_hidden( ) const
{
    return m_num_hidden;
}

double
NeuralNet::get_fitness( ) const
{
    return m_fitness;
}

void
NeuralNet::set_fitness( double val )
{
    m_fitness = val;
}

double
NeuralNet::activate( const std::vector< double >& v ) const
{
    auto it = m_weights.cbegin( );
    double x;
    std::vector< double > tmp;
    for ( int i = 0; i < m_num_hidden; ++i )
    {
        x = 0.0;
        for ( int j = 0; j < m_num_inputs; ++j )
        {
            x += ( *it++ ) * v[ j ];
        }
        x += *it++;
        tmp.push_back( tan_h( x ) );
    }

    x = 0.0;
    for ( int j = 0; j < m_num_hidden; ++j )
    {
        x += tmp[ j ] * ( *it++ );
    }
    x += *it++;
    double res = tan_h( x );

    return res;
}

void
NeuralNet::mutate( )
{
    for ( auto& weight : m_weights )
    {
        double r1 = get_random_number();
        if ( r1 < MUTATION_RATE )
        {
            double r2 = get_random_weight();
            weight += r2 * MAX_PERTURBATION;
        }
    }
}

bool
NeuralNet::save( const char* fileName )
{
    std::ofstream out( fileName, std::ios::out | std::ios::binary );
    if ( !out )
    {
        std::cerr << "Can't open file " << fileName << std::endl;
        return false;
    }
    else
    {
        // std::cerr << "Saving neural net to file "<< fileName << std::endl;
        out.write( (char*)&m_num_inputs, sizeof( int ) );
        out.write( (char*)&m_num_hidden, sizeof( int ) );
        int totalWeights = m_weights.size( );
        for ( int i = 0; i < totalWeights; ++i )
        {
            out.write( (char*)&m_weights[ i ], sizeof( double ) );
        }
        out.write( (char*)&m_fitness, sizeof( double ) );
        out.close( );
        return true;
    }
}

NeuralNet
NeuralNet::Load( const char* fileName )
{
    NeuralNet nn;
    std::ifstream in( fileName, std::ios::in | std::ios::binary );
    if ( !in )
    {
        std::cerr << "Can't open file %s\n" << fileName << std::endl;
        return false;
    }
    else
    {
        // std::cerr << "loading neural net from file " << fileName <<
        // std::endl;
        in.read( (char*)&nn.m_num_inputs, sizeof( int ) );
        in.read( (char*)&nn.m_num_hidden, sizeof( int ) );
        int totalWeights
            = ( nn.m_num_inputs + 1 ) * nn.m_num_hidden + nn.m_num_hidden + 1;
        for ( int i = 0; i < totalWeights; ++i )
        {
            double x;
            in.read( (char*)&x, sizeof( double ) );
            nn.m_weights.push_back( x );
        }
        in.read( (char*)&nn.m_fitness, sizeof( double ) );
        in.close( );
        return nn;
    }
}

NeuralNet operator*( const NeuralNet& nn1, const NeuralNet& nn2 )
{
    NeuralNet baby;
    baby.m_num_inputs = nn1.m_num_inputs;
    baby.m_num_hidden = nn1.m_num_hidden;
    int totalWeights = nn1.m_weights.size( );
    double ratio = 0.5;

    if ( !are_equal_with_epsilon( nn1.get_fitness() + nn2.get_fitness(), 0 ) )
    {
        ratio = nn1.get_fitness() / ( nn1.get_fitness() + nn2.get_fitness() );
    }

    for ( int i = 0; i < totalWeights; ++i )
    {
        double r = get_random_number();
        if ( r < ratio )
        {
            baby.m_weights.push_back( nn1.m_weights[ i ] );
        }
        else
        {
            baby.m_weights.push_back( nn2.m_weights[ i ] );
        }
    }

    return baby;
}

bool
operator==( const NeuralNet& nn1, const NeuralNet& nn2 )
{
    if ( nn1.m_num_inputs != nn2.m_num_inputs )
    {
        std::cout << "different number of inputs" << std::endl;
        return false;
    }
    if ( nn1.m_num_hidden != nn2.m_num_hidden )
    {
        std::cout << "different number of hidden nodes" << std::endl;
        return false;
    }
    int totalWeights = (int)nn1.m_weights.size( );
    if ( totalWeights != ( (int)nn2.m_weights.size( ) ) )
    {
        std::cout << "total weights differents" << std::endl;
        return false;
    }
    for ( int i = 0; i < totalWeights; ++i )
    {
        if ( fabs( nn1.m_weights[ i ] - nn2.m_weights[ i ] ) > 1e-15 )
        {
            std::cout << i + 1 << "th weights are different "
                      << nn1.m_weights[ i ] << " # " << nn2.m_weights[ i ]
                      << std::endl;
            return false;
        }
    }

    return true;
}

std::ostream&
operator<<( std::ostream& out, NeuralNet& nn )
{
    out << nn.m_num_inputs << " " << nn.m_num_hidden << std::endl;
    std::vector< double >::iterator it = nn.m_weights.begin( );
    for ( int i = 0; i < nn.m_num_hidden; ++i )
    {
        for ( int j = 0; j <= nn.m_num_inputs; ++j )
        {
            out << *it++ << " ";
        }
        out << std::endl;
    }
    for ( int i = 0; i <= nn.m_num_hidden; ++i )
    {
        out << *it++ << " ";
    }
    out << std::endl;
    return out;
}
