#pragma once

#include <vector>
#include <ostream>

enum class NN_ERROR
{
    eERROR_FILE_NOT_FOUND = 0,
    eERROR_DIFFERENT_CFG
};

class NeuralNet
{
public:
    NeuralNet( );

    NeuralNet( int num_inputs, int num_hidden = 0 );

    int get_num_inputs( ) const;
    int get_num_hidden( ) const;
    double get_fitness( ) const;
    void set_fitness( double val );
    double activate( const std::vector< double >& v ) const;
    void mutate( );
    bool save( const char* );
    static NeuralNet Load( const char* );

    friend NeuralNet operator*( const NeuralNet& nn1, const NeuralNet& nn2 );
    friend bool operator==( const NeuralNet& nn1, const NeuralNet& nn2 );
    friend std::ostream& operator<<( std::ostream& out, NeuralNet& nn );

private:
    std::vector< double > m_weights;
    int m_num_inputs;
    int m_num_hidden;
    double m_fitness;
};
