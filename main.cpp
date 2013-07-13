#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

#include "AI.h"

#define NEURAL_NET_INPUTS    8
#define WIDTH                10
#define HEIGTH               10
#define N                    10

struct NeuralNetCmp {
    bool operator ()(const NeuralNet& lhs, const NeuralNet& rhs) const {
        return lhs.GetFitness() > rhs.GetFitness();
    }
} NeuralNetCmpObj;

const char* names[] = { "gamer0.nn",
                        "gamer1.nn",
                        "gamer2.nn",
                        "gamer3.nn",
                        "gamer4.nn",
                        "gamer5.nn",
                        "gamer6.nn",
                        "gamer7.nn",
                        "gamer8.nn",
                        "gamer9.nn"
                      };

void InitSimulation() {
    for(int i = 0; i < N; ++i) {
        NeuralNet nn(NEURAL_NET_INPUTS);
        nn.Save(names[i]);
    }
}

void ComputeXorError(NeuralNet &nn) {
    double error = 0.0;
    std::vector<double> inputs;
    inputs.push_back(0.0);
    inputs.push_back(0.0);
    double a = nn.Activate(inputs);
    error += 10000*a*a;
    //////////////////
    inputs.clear();
    inputs.push_back(1.0);
    inputs.push_back(1.0);
    a = nn.Activate(inputs);
    error += 10000*a*a;
    //////////////////
    inputs.clear();
    inputs.push_back(0.0);
    inputs.push_back(1.0);
    a = nn.Activate(inputs);
    error += 10000*(a-1)*(a-1);
    //////////////////
    inputs.clear();
    inputs.push_back(1.0);
    inputs.push_back(0.0);
    a = nn.Activate(inputs);
    error += 10000*(a-1)*(a-1);
    double fitness = 1/error;
    nn.SetFitness(fitness);
}

void RunSimulation() {
    std::vector<NeuralNet> base;

    for (int i = 0; i < N; ++i) {
        NeuralNet nn = NeuralNet::Load(names[i]);
        base.push_back(nn);
    }

    int epoches;
    std::cout << "Please enter the number of generations:" << std::endl;
    std::cin >> epoches;

    for (int g = 0; g < epoches; ++g) {
        std::cout << "Generation " << g + 1 << std::endl;
        std::vector<NeuralNet> v;
        for (int i = 0; i < N; ++i) {
            for (int j = i+1; j < N; ++j) {
                NeuralNet nn1 = base[i] * base[j];
                NeuralNet nn2 = base[j] * base[i];
                nn1.Mutate();
                nn2.Mutate();
                v.push_back(nn1);
                v.push_back(nn2);
            }
        }

        for(int i = 0; i < N; ++i) {
            v.push_back(base[i]);
        }

        for (std::vector<NeuralNet>::iterator it = v.begin(); it != v.end(); ++it) {
            Position pos(WIDTH, HEIGTH);
            while (!pos.EndGame()) {
                Direction dir = AI::GetBestDirection(pos, *it);
                pos.SetDirection(dir);
                pos.Move();
                //pos.Display();
            }
            double fitness = pos.GetScore() + 0.001*pos.GetLife();
            it->SetFitness(fitness);
        }

        sort(v.begin(), v.end(), NeuralNetCmpObj);

        for (int i = 0; i < N; ++i) {
            base[i] = v[i];
        }


        for (int i = 0; i < N; ++i) {
            base[i].Save(names[i]);
        }

        if (g == epoches - 1) {
            std::cout << "Player\t\tFitness" << std::endl;
            int i = 0;
            for (std::vector<NeuralNet>::iterator it = v.begin(); it != v.end(); ++it) {
                double fitness = it->GetFitness();
                std::cout << ++i << "\t\t" << fitness << std::endl;
            }
        }
    }

}

int main() {
    std::string action;
    std::cout << "Please enter action (init/simulation/game) :" << std::endl;
    std::cin >> action;

    while(action != "init" && action != "simulation" && action != "game") {
        std::cout << "Please enter action (init/simulation/game) :" << std::endl;
        std::cin >> action;
    }

    if (action == "init") {
        InitSimulation();
    } else if (action == "simulation") {
        RunSimulation();
    } else {
        NeuralNet nn = NeuralNet::Load(names[0]);
        Position pos(WIDTH, HEIGTH);
        while(!pos.EndGame()) {
            Direction dir = AI::GetBestDirection(pos, nn);
            pos.SetDirection(dir);
            pos.Move();
            pos.Display();
			usleep(40000);
        }
        std::cout << "score = " << pos.GetScore() << ", life = " << pos.GetLife() << std::endl;

    }

    return 0;
}
