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
        return std::tanh( x );
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

NeuralNet::NeuralNet( )
{
}

NeuralNet::NeuralNet( int32_t num_inputs, int32_t num_hidden )
    : m_num_inputs( num_inputs )
    , m_num_hidden( num_hidden ? num_hidden : ( num_inputs / 2 + 1 ) )
    , m_fitness( 0.0 )

{
    int32_t weights_count = ( m_num_inputs + 1 ) * m_num_hidden + m_num_hidden + 1;
    for ( int32_t i = 0; i < weights_count; ++i )
    {
        m_weights.push_back( get_random_weight( ) );
    }
}

int32_t
NeuralNet::get_num_inputs( ) const
{
    return m_num_inputs;
}

int32_t
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
NeuralNet::activate( const std::vector< double >& input ) const
{
    auto weight_iterator = m_weights.cbegin( );

    std::vector< double > hidden_layer( m_num_hidden, 0.0 );
    for ( auto& hidden_node : hidden_layer )
    {
        for ( auto i : input )
        {
            hidden_node += i * ( *weight_iterator++ );
        }

        hidden_node += *weight_iterator++;
        hidden_node = tan_h( hidden_node );
    }

    auto x = 0.0;
    for ( auto& hidden_node : hidden_layer )
    {
        x += hidden_node * ( *weight_iterator++ );
    }
    x += *weight_iterator++;

    return tan_h( x );
}

void
NeuralNet::mutate( )
{
    for ( auto& weight : m_weights )
    {
        if ( get_random_number( ) < MUTATION_RATE )
        {
            weight += get_random_weight( ) * MAX_PERTURBATION;
        }
    }
}

bool
NeuralNet::save( const std::string& file_name ) const
{
    std::cerr << "saving neural net to " << file_name << std::endl;

    std::ofstream out( file_name.c_str( ), std::ios::out | std::ios::binary );
    if ( !out )
    {
        std::cerr << "Can't open file " << file_name << std::endl;
        return false;
    }
    else
    {
        out.write( (char*)&m_num_inputs, sizeof( int32_t ) );
        out.write( (char*)&m_num_hidden, sizeof( int32_t ) );
        for ( auto& weight : m_weights )
        {
            out.write( (char*)&weight, sizeof( double ) );
        }
        out.write( (char*)&m_fitness, sizeof( double ) );
        out.close( );
        return true;
    }
}

NeuralNet
NeuralNet::load( const std::string& file_name )
{
    std::cerr << "loading neural net from " << file_name << std::endl;
    NeuralNet neural_net;
    std::ifstream in( file_name.c_str( ), std::ios::in | std::ios::binary );
    if ( !in )
    {
        std::cerr << "Can't open file %s\n" << file_name << std::endl;
        return false;
    }
    else
    {
        in.read( (char*)&neural_net.m_num_inputs, sizeof( int32_t ) );
        in.read( (char*)&neural_net.m_num_hidden, sizeof( int32_t ) );
        int32_t total_weights = ( neural_net.m_num_inputs + 1 ) * neural_net.m_num_hidden
                                + neural_net.m_num_hidden + 1;
        neural_net.m_weights = std::vector< double >( total_weights );
        for ( auto& weight : neural_net.m_weights )
        {
            in.read( (char*)&weight, sizeof( double ) );
        }
        in.read( (char*)&neural_net.m_fitness, sizeof( double ) );
        in.close( );
        return neural_net;
    }
}

NeuralNet operator*( const NeuralNet& lhs, const NeuralNet& rhs )
{
    NeuralNet baby;
    baby.m_num_inputs = lhs.m_num_inputs;
    baby.m_num_hidden = lhs.m_num_hidden;
    int32_t total_weights = lhs.m_weights.size( );
    auto ratio = 0.5;
    const auto lhs_fitness = lhs.get_fitness( );
    const auto rhs_fitness = rhs.get_fitness( );
    const auto sum_fitness = lhs_fitness + rhs_fitness;
    if ( !are_equal_with_epsilon( sum_fitness, 0.0 ) )
    {
        ratio = lhs_fitness / sum_fitness;
    }

    for ( int32_t i = 0; i < total_weights; ++i )
    {
        double r = get_random_number( );
        if ( r < ratio )
        {
            baby.m_weights.push_back( lhs.m_weights[ i ] );
        }
        else
        {
            baby.m_weights.push_back( rhs.m_weights[ i ] );
        }
    }

    return baby;
}

bool
operator==( const NeuralNet& lhs, const NeuralNet& rhs )
{
    if ( lhs.m_num_inputs != rhs.m_num_inputs )
    {
        return false;
    }

    if ( lhs.m_num_hidden != rhs.m_num_hidden )
    {
        return false;
    }

    int32_t total_weights = (int32_t)lhs.m_weights.size( );
    if ( total_weights != ( (int32_t)rhs.m_weights.size( ) ) )
    {
        return false;
    }

    for ( int32_t i = 0; i < total_weights; ++i )
    {
        if ( !are_equal_with_epsilon( lhs.m_weights[ i ], rhs.m_weights[ i ] ) )
        {
            return false;
        }
    }

    return true;
}

std::ostream&
operator<<( std::ostream& out, const NeuralNet& neural_net )
{
    out << neural_net.m_num_inputs << " " << neural_net.m_num_hidden << std::endl;
    for ( const auto& weight : neural_net.m_weights )
    {
        out << weight << " ";
    }
    out << std::endl;
    return out;
}
