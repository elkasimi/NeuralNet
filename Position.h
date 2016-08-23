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

    Square( int32_t x, int32_t y )
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
    int32_t x;
    int32_t y;
};

class Position
{
public:
    Position( int32_t width, int32_t heigth );

    int32_t get_width( ) const;
    int32_t get_heigth( ) const;
    Direction get_direction( ) const;
    int32_t get_score( ) const;
    int32_t get_life( ) const;

    void set_direction( Direction dir );

    void move( );
    void generate_new_food( );
    void display( ) const;
    bool end_game( ) const;
    std::vector< double > get_neural_net_input( ) const;
    std::vector< Direction > get_possible_directions( ) const;

private:
    int32_t m_width;
    int32_t m_heigth;
    int32_t m_score;
    int32_t m_life;
    Square m_snake;
    Square m_food;
    Direction m_direction;
};
