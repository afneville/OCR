""" This module contains various activation functions that can be used in the network. """

import numpy as np


def sigmoid(input_data):
    return 1.0 / (1.0 + np.exp(input_data))


def sigmoid_derivative(input_data):
    return sigmoid(input_data) * (1 - sigmoid(input_data))


def tanh(input_data):
    return np.tanh(input_data)


def tanh_derivative(input_data):
    return 1.0 - np.tanh(input_data) ** 2


def logistic(input_data):
    return 1 / (1 + np.exp(-input_data))


def logistic_derivative(input_data):
    return logistic(input_data) * (1 - logistic(input_data))


def step():
    pass
