/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProbDistrib.cpp
 * Author: cancian
 * 
 * Created on 23 de Agosto de 2018, 17:25
 */

#include "ProbDistrib.h"
#include <cmath>

//https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)
double ProbDistrib::uniform(double x, double min, double max) {
    return 1 / (max - min);
}

// https://en.wikipedia.org/wiki/Exponential_distribution
double ProbDistrib::exponential(double x, double mean) {
    return (1 / mean) * exp(-(1 / mean) * x);
}

double factorial(double fact) {
    if (fact == 0 || fact == 1)
        return 1;
    else
        return fact * factorial(fact - 1);
}

// https://en.wikipedia.org/wiki/Erlang_distribution
double ProbDistrib::erlang(double x, double mean, double M) {
    return pow(x, M - 1) * exp(- x / mean) / (pow(mean, M) * factorial(M - 1));
}

// https://en.wikipedia.org/wiki/Normal_distribution
double ProbDistrib::normal(double x, double mean, double stddev) {
    return (1 / (stddev * sqrt(2 * M_PI))) * exp( -0.5 * pow((x - mean), 2) / pow(stddev, 2));
}

double ProbDistrib::gamma(double x, double mean, double alpha) {
}

double outherGamma(double n){
    return sqrt(2 * M_PI * n) * pow((n/exp(1)), n);
}

double newBeta(double x, double y) {
    return outherGamma(x) * outherGamma(y) / outherGamma(x + y);
}

//https://en.wikipedia.org/wiki/Beta_distribution
double ProbDistrib::beta(double x, double alpha, double beta) {
    return (1 / newBeta(alpha, beta)) * pow(x, alpha - 1) * pow(1 - x, beta - 1);
}

//https://en.wikipedia.org/wiki/Weibull_distribution
double ProbDistrib::weibull(double x, double alpha, double scale) {
    if (x < 0)
	return 0;
    else
        return alpha / scale * pow(x / scale, alpha - 1) * exp(-pow(x / scale, alpha));
}

double logNormal(double x, double mean, double stddev) {
    
}

// https://en.wikipedia.org/wiki/Triangular_distribution
double ProbDistrib::triangular(double x, double min, double mode, double max) {
    if (min <= x && x < mode)
        return 2 * (x - min) / ((max - min) * (mode - min));
    if (x == mode)
        return 2 / (max - min);
    if (mode < x && x <= max)
        return 2 * (max - x) / ((max - min) * (max - mode));
    return 0; // implicit x < a || b < x
}