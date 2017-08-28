################################################################################
# Compile each iAnt-ARGoS class into a shared object file.
################################################################################
add_library(iAnt_controller     SHARED iAnt_controller.h
                                    iAnt_controller.cpp
                                    iAnt_loop_functions.h
                                    iAnt_loop_functions.cpp
                                    iAnt_pheromone.h
                                    iAnt_pheromone.cpp
                                    nn/Perceptron.cpp
                                    nn/Perceptron.h
                                    nn/Neuron.cpp nn/Neuron.h
                                    nn/NeuralNetwork.cpp
                                    nn/NeuralNetwork.h
                                    ga/Chromosome.cpp
                                    ga/Chromosome.h
                                    nn/NeuralNetFactory.cpp
                                    nn/NeuralNetFactory.h
                                    nn/ValueNeuron.cpp
                                    nn/ValueNeuron.h
                                    ga/BasicGA.cpp
                                    ga/BasicGA.h
                                    ga/Unique.cpp
                                    ga/Unique.h
                                    ga/ChromosomeFactory.cpp
                                    ga/ChromosomeFactory.h)

add_library(iAnt_loop_functions SHARED iAnt_loop_functions.h
                                    iAnt_loop_functions.cpp
                                    iAnt_controller.h
                                    iAnt_controller.cpp
                                    iAnt_qt_user_functions.h
                                    iAnt_qt_user_functions.cpp
                                    iAnt_pheromone.h
                                    iAnt_pheromone.cpp main.cpp
                                    nn/Perceptron.cpp
                                    nn/Perceptron.h
                                    nn/Neuron.cpp nn/Neuron.h
                                    nn/NeuralNetwork.cpp
                                    nn/NeuralNetwork.h
                                    ga/Chromosome.cpp
                                    ga/Chromosome.h
                                    nn/NeuralNetFactory.cpp
                                    nn/NeuralNetFactory.h
                                    nn/ValueNeuron.cpp
                                    nn/ValueNeuron.h
                                    ga/BasicGA.cpp
                                    ga/BasicGA.h
                                    ga/Unique.cpp
                                    ga/Unique.h
                                    ga/ChromosomeFactory.cpp
                                    ga/ChromosomeFactory.h)

add_library(BasicGA SHARED          nn/Perceptron.cpp
                                    nn/Perceptron.h
                                    nn/Neuron.cpp nn/Neuron.h
                                    nn/NeuralNetwork.cpp
                                    nn/NeuralNetwork.h
                                    ga/Chromosome.cpp
                                    ga/Chromosome.h
                                    nn/NeuralNetFactory.cpp
                                    nn/NeuralNetFactory.h
                                    nn/ValueNeuron.cpp
                                    nn/ValueNeuron.h
                                    ga/BasicGA.cpp
                                    ga/BasicGA.h
                                    ga/Unique.cpp
                                    ga/Unique.h
                                    ga/ChromosomeFactory.cpp
                                    ga/ChromosomeFactory.h)

################################################################################
# Correctly link each shared object with its dependencies . . .
################################################################################

add_executable(iant_main main.cpp)

target_link_libraries(iAnt_controller
                      argos3core_simulator
                      argos3plugin_simulator_entities
                      argos3plugin_simulator_footbot
                      argos3plugin_simulator_genericrobot)

target_link_libraries(iAnt_loop_functions
                      argos3core_simulator
                      argos3plugin_simulator_entities
                      argos3plugin_simulator_footbot
                      argos3plugin_simulator_genericrobot
                      argos3plugin_simulator_qtopengl)

target_link_libraries(iant_main
	iAnt_loop_functions
        iAnt_controller
        argos3core_simulator
        BasicGA)
