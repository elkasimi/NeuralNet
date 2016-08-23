#pragma once

#include <ostream>
#include <string>
#include <vector>

enum class NN_ERROR
{
    eERROR_FILE_NOT_FOUND = 0,
    eERROR_DIFFERENT_CFG
};

class NeuralNet
{
public:
    NeuralNet( );

    NeuralNet( int32_t num_inputs, int32_t num_hidden = 0 );

    int32_t get_num_inputs( ) const;
    int32_t get_num_hidden( ) const;
    double get_fitness( ) const;
    void set_fitness( double val );
    double activate( const std::vector< double >& input ) const;
    void mutate( );
    bool save( const std::string& file_name ) const;
    static NeuralNet load( const std::string& file_name );

    friend NeuralNet operator*( const NeuralNet& lhs, const NeuralNet& rhs );
    friend bool operator==( const NeuralNet& lhs, const NeuralNet& rhs );
    friend std::ostream& operator<<( std::ostream& out,
                                     const NeuralNet& neural_net );

private:
    std::vector< double > m_weights;
    int32_t m_num_inputs;
    int32_t m_num_hidden;
    double m_fitness;
};
