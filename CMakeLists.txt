cmake_minimum_required (VERSION 2.6)
project (NeuralNetGeneticAlgo)

SET(CMAKE_CXX_FLAGS "-pg -Wall -std=c++14 -O2 -pthread")

SET(HEADERS
	Position.h
	NeuralNet.h
	AI.h
)

SET(SOURCES
	Position.cpp
	NeuralNet.cpp
	AI.cpp
	main.cpp
)

ADD_EXECUTABLE(NeuralNetGeneticAlgo ${HEADERS} ${SOURCES})
