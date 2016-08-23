#pragma once

#include <vector>

enum class Direction
{
    RIGHT = 0,
    LEFT,
    UP,
    DOWN,
    PASS
};

struct Square
{
    Square( )
        : x( 0 )
        , y( 0 )
    {
    }

    Square( int x, int y )
        : x( x )
        , y( y )
    {
    }

    bool
    operator<( const Square& rhs ) const
    {
        if ( x != rhs.x )
        {
            return x < rhs.x;
        }
        else
        {
            return y < rhs.y;
        }
    }
    int x;
    int y;
};

class Position
{
public:
    Position( int width, int heigth );

    int get_width( ) const;
    int get_heigth( ) const;
    Direction get_direction( ) const;
    int get_score( ) const;
    int get_life( ) const;

    void set_direction( Direction dir );

    void move( );
    void generate_new_food( );
    void display( ) const;
    bool end_game( ) const;
    std::vector< double > get_neural_net_input( ) const;
    std::vector< Direction > get_possible_directions( ) const;

private:
    int m_width;
    int m_heigth;
    int m_score;
    int m_life;
    Square m_snake;
    Square m_food;
    Direction m_direction;
};
