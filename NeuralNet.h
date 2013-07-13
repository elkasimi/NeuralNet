#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include <vector>
#include <fstream>

enum {
    eERROR_FILE_NOT_FOUND = 0,
    eERROR_DIFFERENT_CFG
};

class NeuralNet {
public:
    NeuralNet() {}
    NeuralNet(int n, int m = 0);
    int GetNumInputs() const {
        return fNumInputs;
    }
    int GetNumHidden() const {
        return fNumHidden;
    }

    double GetFitness() const {
        return fFitness;
    }
    void SetFitness(double val) {
        fFitness = val;
    }

    double Activate(std::vector<double> &v);
    void Mutate();
    bool Save(const char*);
    static NeuralNet Load(const char*);

    friend NeuralNet operator *(const NeuralNet &nn1, const NeuralNet &nn2);
    friend bool operator == (const NeuralNet &nn1, const NeuralNet &nn2);
    friend std::ostream& operator << (std::ostream &out, NeuralNet &nn);

private:
    std::vector<double> fWeights;
    int fNumInputs;
    int fNumHidden;
    double fFitness;
    static double fMutationRate;
    static double fMaxPerturbation;

    static double GetRandomNumber();
    static double GetRandomWeight();
    static double Tanh(double x);
    static double Sigmoid(double x);
    static bool AreEqualWithEpsilon(double x, double y);
};

#endif
