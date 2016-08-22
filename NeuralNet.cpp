#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "NeuralNet.h"

double NeuralNet::fMutationRate = 0.12;
double NeuralNet::fMaxPerturbation = 0.3;

NeuralNet::NeuralNet( int n, int m )
{
    fNumInputs = n;
    if ( m == 0 )
    {
        m = n / 2 + 1;
    }
    fNumHidden = m;
    fFitness = 0.0;
    int totalWeights = ( fNumInputs + 1 ) * fNumHidden + fNumHidden + 1;
    for ( int i = 0; i < totalWeights; ++i )
    {
        fWeights.push_back( GetRandomWeight( ) );
    }
}

double
NeuralNet::activate( std::vector< double >& v )
{
    std::vector< double >::iterator it = fWeights.begin( );
    double x;
    std::vector< double > tmp;
    for ( int i = 0; i < fNumHidden; ++i )
    {
        x = 0.0;
        for ( int j = 0; j < fNumInputs; ++j )
        {
            x += ( *it++ ) * v[ j ];
        }
        x += *it++;
        tmp.push_back( Tanh( x ) );
    }

    x = 0.0;
    for ( int j = 0; j < fNumHidden; ++j )
    {
        x += tmp[ j ] * ( *it++ );
    }
    x += *it++;
    double res = Tanh( x );

    return res;
}

void
NeuralNet::Mutate( )
{
    std::vector< double >::iterator it = fWeights.begin( ),
                                    end = fWeights.end( );
    for ( ; it != end; ++it )
    {
        double r1 = GetRandomNumber( );
        if ( r1 < fMutationRate )
        {
            double r2 = GetRandomWeight( );
            *it += r2 * fMaxPerturbation;
        }
    }
}

bool
NeuralNet::Save( const char* fileName )
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
        out.write( (char*)&fNumInputs, sizeof( int ) );
        out.write( (char*)&fNumHidden, sizeof( int ) );
        int totalWeights = fWeights.size( );
        for ( int i = 0; i < totalWeights; ++i )
        {
            out.write( (char*)&fWeights[ i ], sizeof( double ) );
        }
        out.write( (char*)&fFitness, sizeof( double ) );
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
        in.read( (char*)&nn.fNumInputs, sizeof( int ) );
        in.read( (char*)&nn.fNumHidden, sizeof( int ) );
        int totalWeights
            = ( nn.fNumInputs + 1 ) * nn.fNumHidden + nn.fNumHidden + 1;
        for ( int i = 0; i < totalWeights; ++i )
        {
            double x;
            in.read( (char*)&x, sizeof( double ) );
            nn.fWeights.push_back( x );
        }
        in.read( (char*)&nn.fFitness, sizeof( double ) );
        in.close( );
        return nn;
    }
}

double
NeuralNet::GetRandomWeight( )
{
    double r = GetRandomNumber( );
    double res = 2 * r - 1;
    return res;
}

double
NeuralNet::GetRandomNumber( )
{
    double res = rand( );
    res /= RAND_MAX;
    return res;
}

double
NeuralNet::Tanh( double x )
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
        return tanh( x );
    }
}

double
NeuralNet::Sigmoid( double x )
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
        double res = 1 / ( 1 + exp( -x ) );
        return res;
    }
}

bool
NeuralNet::AreEqualWithEpsilon( double x, double y )
{
    const double eps = 1e-15;
    return fabs( x - y ) < eps;
}

NeuralNet operator*( const NeuralNet& nn1, const NeuralNet& nn2 )
{
    NeuralNet baby;
    baby.fNumInputs = nn1.fNumInputs;
    baby.fNumHidden = nn1.fNumHidden;
    int totalWeights = nn1.fWeights.size( );
    double ratio = 0.5;

    if ( !NeuralNet::AreEqualWithEpsilon( nn1.fFitness + nn2.fFitness, 0 ) )
    {
        ratio = nn1.fFitness / ( nn1.fFitness + nn2.fFitness );
    }
    for ( int i = 0; i < totalWeights; ++i )
    {
        double r = NeuralNet::GetRandomNumber( );
        if ( r < ratio )
        {
            baby.fWeights.push_back( nn1.fWeights[ i ] );
        }
        else
        {
            baby.fWeights.push_back( nn2.fWeights[ i ] );
        }
    }

    return baby;
}

bool
operator==( const NeuralNet& nn1, const NeuralNet& nn2 )
{
    if ( nn1.fNumInputs != nn2.fNumInputs )
    {
        std::cout << "different number of inputs" << std::endl;
        return false;
    }
    if ( nn1.fNumHidden != nn2.fNumHidden )
    {
        std::cout << "different number of hidden nodes" << std::endl;
        return false;
    }
    int totalWeights = (int)nn1.fWeights.size( );
    if ( totalWeights != ( (int)nn2.fWeights.size( ) ) )
    {
        std::cout << "total weights differents" << std::endl;
        return false;
    }
    for ( int i = 0; i < totalWeights; ++i )
    {
        if ( fabs( nn1.fWeights[ i ] - nn2.fWeights[ i ] ) > 1e-15 )
        {
            std::cout << i + 1 << "th weights are different "
                      << nn1.fWeights[ i ] << " # " << nn2.fWeights[ i ]
                      << std::endl;
            return false;
        }
    }

    return true;
}

std::ostream&
operator<<( std::ostream& out, NeuralNet& nn )
{
    out << nn.fNumInputs << " " << nn.fNumHidden << std::endl;
    std::vector< double >::iterator it = nn.fWeights.begin( );
    for ( int i = 0; i < nn.fNumHidden; ++i )
    {
        for ( int j = 0; j <= nn.fNumInputs; ++j )
        {
            out << *it++ << " ";
        }
        out << std::endl;
    }
    for ( int i = 0; i <= nn.fNumHidden; ++i )
    {
        out << *it++ << " ";
    }
    out << std::endl;
    return out;
}
