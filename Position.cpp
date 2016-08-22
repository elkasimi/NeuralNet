#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "Position.h"

int Position::fMaxLife = 1000;

void
Position::move( )
{
    int dx = 0, dy = 0;
    switch ( fDirection )
    {
    case RIGHT:
        dx = 1;
        break;
    case LEFT:
        dx = -1;
        break;
    case UP:
        dy = -1;
        break;
    case DOWN:
        dy = 1;
        break;
    default:
        break;
    }

    fSnake.x += dx;
    fSnake.y += dy;

    if ( fSnake.x == fFood.x && fSnake.y == fFood.y )
    {
        GenerateNewFood( );
        ++fScore;
    }

    ++fLife;
}

void
Position::GenerateNewFood( )
{
    fFood.x = rand( ) % fWidth;
    fFood.y = rand( ) % fHeigth;
    while ( fFood.x == fSnake.x && fFood.y == fSnake.y )
    {
        fFood.x = rand( ) % fWidth;
        fFood.y = rand( ) % fHeigth;
    }
}

void
Position::Display( )
{
    system( "clear" );
    std::vector< std::string > v;
    std::string s = "";
    s.append( fWidth, '.' );
    for ( int i = 0; i < fHeigth; i++ )
    {
        v.push_back( s );
    }
    v[ fFood.y ][ fFood.x ] = 'o';
    if ( fSnake.x >= 0 && fSnake.x < fWidth && fSnake.y >= 0
         && fSnake.y < fHeigth )
    {
        char c;
        switch ( fDirection )
        {
        case RIGHT:
            c = '>';
            break;
        case LEFT:
            c = '<';
            break;
        case UP:
            c = '^';
            break;
        case DOWN:
            c = 'v';
            break;
        default:
            c = '?';
            break;
        }
        v[ fSnake.y ][ fSnake.x ] = c;
    }

    for ( int i = 0; i < fHeigth; i++ )
    {
        std::cout << v[ i ] << std::endl;
    }

    std::cout << "score = " << fScore << ", life = " << fLife << ", dir = ";
    switch ( fDirection )
    {
    case RIGHT:
        std::cout << "RIGHT";
        break;
    case LEFT:
        std::cout << "LEFT";
        break;
    case UP:
        std::cout << "UP";
        break;
    case DOWN:
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
Position::EndGame( )
{
    if ( fSnake.x < 0 || fSnake.x >= fWidth )
    {
        return true;
    }

    if ( fSnake.y < 0 || fSnake.y >= fHeigth )
    {
        return true;
    }

    if ( fLife >= fMaxLife )
    {
        return true;
    }

    return false;
}

std::vector< double >
Position::get_neural_net_input( )
{
    std::vector< double > res;
    if ( fSnake.x < fWidth )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }
    if ( fSnake.x >= 0 )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }
    if ( fSnake.y < fHeigth )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }
    if ( fSnake.y >= 0 )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }
    if ( fSnake.x < fFood.x )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }
    if ( fSnake.y < fFood.y )
    {
        res.push_back( 1.0 );
    }
    else
    {
        res.push_back( 0.0 );
    }
    switch ( fDirection )
    {
    case RIGHT:
        res.push_back( 0.0 );
        res.push_back( 0.0 );
        break;
    case LEFT:
        res.push_back( 0.0 );
        res.push_back( 1.0 );
        break;
    case UP:
        res.push_back( 1.0 );
        res.push_back( 0.0 );
        break;
    case DOWN:
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
Position::get_possible_directions( )
{
    std::vector< Direction > res;
    switch ( fDirection )
    {
    case RIGHT:
    case LEFT:
        res.push_back( UP );
        res.push_back( DOWN );
        res.push_back( PASS );
        break;
    case UP:
    case DOWN:
        res.push_back( RIGHT );
        res.push_back( LEFT );
        res.push_back( PASS );
        break;

    default:
        std::cerr << "Unknown direction" << std::endl;
        throw 1;
    }

    return res;
}
