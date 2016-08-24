This is a very simple illustration of artificial neural nets with genetic algorithms.

the project consist of 3 classes:
- Position
- NeutalNet
- AI

#Position:
describe a simple position of snake game consisting of snake and a randomly generated food

#NeuralNet:
Basic neural net class for:
- compute an activation from an input
- mutate the net
- compute cross-over of two nets

#AI:
given a neural net it choses the best direction to go

when running program 3 options are possibles:
#init
create the first generation and save it to files candidate0..9.nn

#simulation
perform simulation to iteratively find the best candidates that have the best fitness using a genetic algorithm

#test
test candidate0.nn performance

two tests are available:
- learning xor
- learning how to eat food without touching walls
you can use xor test by setting XOR_TEST to true
you can use snake game by setting it to false

#Build

install cmake and build-essentials
$sudo apt-get install cmake build-essentials

$cmake .
$make

#Run
$./NeuralNetGeneticAlgo

