#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "Position.h"

namespace
{
const int32_t MAX_LIFE = 1000;
}

Position::Position( int32_t width, int32_t heigth )
    : m_width( width )
    , m_heigth( heigth )
    , m_score( 0 )
    , m_life( 0 )
    , m_food( m_width / 2, m_heigth / 2 )
    , m_direction( Direction::RIGHT )

{
}

int32_t
Position::get_width( ) const
{
    return m_width;
}

int32_t
Position::get_heigth( ) const
{
    return m_heigth;
}

Direction
Position::get_direction( ) const
{
    return m_direction;
}

int32_t
Position::get_score( ) const
{
    return m_score;
}

int32_t
Position::get_life( ) const
{
    return m_life;
}

void
Position::set_direction( Direction dir )
{
    if ( dir != Direction::PASS )
    {
        m_direction = dir;
    }
}

void
Position::move( )
{
    int32_t dx = 0, dy = 0;
    switch ( m_direction )
    {
    case Direction::RIGHT:
        dx = 1;
        break;
    case Direction::LEFT:
        dx = -1;
        break;
    case Direction::UP:
        dy = -1;
        break;
    case Direction::DOWN:
        dy = 1;
        break;
    default:
        break;
    }

    m_snake.x += dx;
    m_snake.y += dy;

    if ( m_snake.x == m_food.x && m_snake.y == m_food.y )
    {
        generate_new_food( );
        ++m_score;
    }

    ++m_life;
}

void
Position::generate_new_food( )
{
    m_food.x = rand( ) % m_width;
    m_food.y = rand( ) % m_heigth;
    while ( m_food.x == m_snake.x && m_food.y == m_snake.y )
    {
        m_food.x = rand( ) % m_width;
        m_food.y = rand( ) % m_heigth;
    }
}

void
Position::display( ) const
{
    //! TODO use a graphic engine

    int32_t error = std::system( "clear" );
    if ( error != 0 )
    {
        std::cerr << "error when clearing screen!" << error << std::endl;
    }

    std::vector< std::string > v( m_heigth, std::string( m_width, '.' ) );

    v[ m_food.y ][ m_food.x ] = 'o';

    if ( m_snake.x >= 0 && m_snake.x < m_width && m_snake.y >= 0 && m_snake.y < m_heigth )
    {
        char c;
        switch ( m_direction )
        {
        case Direction::RIGHT:
            c = '>';
            break;
        case Direction::LEFT:
            c = '<';
            break;
        case Direction::UP:
            c = '^';
            break;
        case Direction::DOWN:
            c = 'v';
            break;
        default:
            c = '?';
            break;
        }
        v[ m_snake.y ][ m_snake.x ] = c;
    }

    for ( const auto& s : v )
    {
        std::cout << s << std::endl;
    }

    std::cout << "score = " << m_score << ", life = " << m_life << ", dir = ";
    switch ( m_direction )
    {
    case Direction::RIGHT:
        std::cout << "RIGHT";
        break;
    case Direction::LEFT:
        std::cout << "LEFT";
        break;
    case Direction::UP:
        std::cout << "UP";
        break;
    case Direction::DOWN:
        std::cout << "DOWN";
        break;
    default:
        std::cout << "UNKNOWN";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

bool
Position::end_game( ) const
{
    if ( m_snake.x < 0 || m_snake.x >= m_width )
    {
        return true;
    }

    if ( m_snake.y < 0 || m_snake.y >= m_heigth )
    {
        return true;
    }

    if ( m_life >= MAX_LIFE )
    {
        return true;
    }

    return false;
}

std::vector< double >
Position::get_neural_net_input( ) const
{
    std::vector< double > res;
    if ( m_snake.x < m_width )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }

    if ( m_snake.x >= 0 )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }

    if ( m_snake.y < m_heigth )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }

    if ( m_snake.y >= 0 )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }

    if ( m_snake.x < m_food.x )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }

    if ( m_snake.y < m_food.y )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }

    switch ( m_direction )
    {
    case Direction::RIGHT:
        res.push_back( 0.0 );
        res.push_back( 0.0 );
        break;
    case Direction::LEFT:
        res.push_back( 0.0 );
        res.push_back( 1.0 );
        break;
    case Direction::UP:
        res.push_back( 1.0 );
        res.push_back( 0.0 );
        break;
    case Direction::DOWN:
        res.push_back( 1.0 );
        res.push_back( 1.0 );
        break;
    default:
        std::cerr << "Unknown direction!" << std::endl;
        throw 404;
    }

    return res;
}

std::vector< Direction >
Position::get_possible_directions( ) const
{
    switch ( m_direction )
    {
    case Direction::RIGHT:
    case Direction::LEFT:
        return {Direction::UP, Direction::DOWN, Direction::PASS};

    case Direction::UP:
    case Direction::DOWN:
        return {Direction::RIGHT, Direction::LEFT, Direction::PASS};

    default:
        std::cerr << "Unknown direction" << std::endl;
        throw 1;
    }

    return {};
}
