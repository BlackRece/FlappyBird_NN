# Flappy Bird Evolutionary Neural Network

[![Build Status](https://img.shields.io/travis/BlackRece/flappy-bird-enn.svg)](https://github.com/BlackRece/FlappyBird_NN/workflows/CI/badge.svg)


This project explores the implementation of an evolutionary neural network (ENN) in the classic mobile game, Flappy Bird. The goal is to develop an intelligent agent capable of navigating the game environment without crashing into obstacles. The ENN utilizes principles from genetic algorithms and neural networks to learn and adapt to the challenges presented by the game. By simulating the process of natural selection, the algorithm gradually improves its performance, ultimately surpassing the target of passing at least 400 columns.

## Features

- Implementation of an evolutionary neural network in Flappy Bird
- Utilises genetic algorithms and neural networks for learning and adaptation
- Simulates the process of natural selection to improve performance
- Multiple birds in flight for accelerated evolution
- Provides insights into the potential applications of AI in gaming

## Getting Started

To get started with the project, follow these steps:

1. Clone the repository: `gh repo clone BlackRece/FlappyBird_NN`
2. Open the project in Visual Studio Community 2023.
3. Build the project in Visual Studio.
4. Run the game from within Visual Studio.

## License [![License](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for more details.

## Tags

- Artificial Intelligence
- Machine Learning
- Genetic Algorithm
- Neural Networks
- Game Development
- Flappy Bird
- C++
- Visual Studio Community 2023

 
# Neural Network for Feedforward Process

This project implements a neural network (NN) for the feedforward process, which involves transforming input values through hidden layers to produce output values. The NN is designed to capture complex relationships between inputs and outputs, enabling it to make accurate predictions or decisions.

## Overview

The neural network consists of three main components: the input layer, hidden layer, and output layer. The feedforward process involves passing the input values through the hidden layer to obtain the output values. Here's a brief overview of the key steps:

1. **Input Layer**: The input values are provided as input to the neural network. These values serve as the initial information for the network.

2. **Hidden Layer**: The hidden layer introduces non-linearity to the network and captures complex relationships between the inputs and hidden layer values. The sigmoid activation function is applied element-wise to the hidden layer values to introduce non-linearity.

3. **Output Layer**: The weights between the hidden layer and the output layer are applied to the hidden layer values using the dot product operation. Biases are added to the resulting values, and the sigmoid activation function is applied element-wise to introduce non-linearity in the output layer.

4. **Output**: The final output values are obtained, representing the predictions or decisions made by the neural network based on the learned relationships between inputs and outputs.

## Activation Function

The sigmoid activation function is used in both the hidden layer and output layer. It maps the input values to a range between 0 and 1, introducing non-linearity to the network's output. The sigmoid function is defined as:

σ(x) = 1 / (1 + e^(-x))

This function allows the neural network to capture intricate patterns and relationships in the data, enhancing its predictive capabilities.

