/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FitterBonotto.cpp
 * Author: bonotto
 * 
 * Created on November 12, 2018, 2:20 PM
 */

#include <cmath>
#include <regex>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "FitterBonotto.h"
#include "ProbDistrib.h"
#include "Traits.h"


// ####################################################################################################################
// ################################################ EXTERNAL FUNCTIONS ################################################
// ####################################################################################################################


FitterBonotto::FitterBonotto():
	sampler(new Traits<Sampler_if>::Implementation()),
	samples(new std::vector<double>())
{}


FitterBonotto::FitterBonotto(const FitterBonotto& orig):
	sampler(new Traits<Sampler_if>::Implementation()),
	samples(new std::vector<double>())
{}


bool FitterBonotto::isNormalDistributed(double confidencelevel)
{
    // double avg = 0;
    // double dev = 0;
    
    // readFile();
    // avgDev(&avg, &dev);

    // for (int i = 0; i < samples->size(); i++)
    //     distribution->push_back(sampler->sampleNormal(avg, dev));

    // avgDev(&avg, &dev, distribution);

    // auto hypTester = new Traits<HypothesisTester_if>::Implementation();
    // hypTester->setDataFilename(_dataFilename);

    // clean();

    // return hypTester->testAverage(confidencelevel, avg, HypothesisTester_if::EQUAL) &&
    //        hypTester->testVariance(confidencelevel, sqrt(dev), HypothesisTester_if::EQUAL);
}


void FitterBonotto::fitUniform(double *sqrerror, double *min, double *max)
{
    readFile();
    minMax(min, max);
    sqrError(min, max, sqrerror, ProbDistrib::uniform, *min, *max);
}


void FitterBonotto::fitTriangular(double *sqrerror, double *min, double *mo, double *max)
{
    readFile();
    minMaxMode(min, max, mo);
    sqrError(min, max, sqrerror, ProbDistrib::triangular, *min, *mo, *max);
}


void FitterBonotto::fitNormal(double *sqrerror, double *avg, double *stddev)
{
    double min = 0;
    double max = 0;

    readFile();
    avgDevMinMax(avg, stddev, &min, &max);
    sqrError(&min, &max, sqrerror, ProbDistrib::normal, *avg, *stddev);
}


void FitterBonotto::fitExpo(double *sqrerror, double *avg1)
{
    double min = 0;
    double max = 0;

    readFile();
    avgMinMax(avg1, &min, &max);
    sqrError(&min, &max, sqrerror, ProbDistrib::exponential, *avg1);
}


void FitterBonotto::fitErlang(double *sqrerror, double *avg, double *m)
{
    double min = 0;
    double max = 0;

    readFile();
    avgShapeMinMax(avg, m, &min, &max);
    sqrError(&min, &max, sqrerror, ProbDistrib::exponential, *avg);
}


void FitterBonotto::fitBeta(double *sqrerror, double *alpha, double *beta, double *infLimit,double *supLimit)
{
   readFile();
   alphaBetaMinMax(alpha, beta, infLimit, supLimit);
   sqrError(infLimit, supLimit, sqrerror, ProbDistrib::beta, *alpha, *beta);
}


void FitterBonotto::fitWeibull(double *sqrerror, double *alpha, double *scale)
{
    double min = 0;
    double max = 0;

    readFile();
    alphaScaleMinMax(alpha, scale, &min, &max);
    sqrError(&min, &max, sqrerror, ProbDistrib::weibull, *alpha, *scale);
}


void FitterBonotto::fitAll(double *sqrerror, std::string *name)
{
    double a = 0;
    double b = 0;
    double c = 0;
    double d = 0;
    double error = 0;

    fitUniform(sqrerror, &a, &b);
    *name = "Uniform";
    
    fitTriangular(&error, &a, &b, &c);

    if (error < *sqrerror) {
        *sqrerror = error;
        *name = "Triangular";
    }

    fitNormal(&error, &a, &b);

    if (error < *sqrerror) {
        *sqrerror = error;
        *name = "Normal";
    }

    fitExpo(&error, &a);

    if (error < *sqrerror) {
        *sqrerror = error;
        *name = "Exponential";
    }

   fitErlang(&error, &a, &b);

   if (error < *sqrerror) {
       *sqrerror = error;
       *name = "Erlang";
   }

    fitBeta(&error, &a, &b, &c, &d);

    if (error < *sqrerror) {
        *sqrerror = error;
        *name = "Beta";
    }

   fitWeibull(&error, &a, &b);

   if (error < *sqrerror) {
       *sqrerror = error;
       *name = "Weibull";
   }
}


void FitterBonotto::setDataFilename(std::string dataFilename)
{
    if ( std::regex_match(dataFilename, std::regex("[./]*[a-zA-Z0-9_]+")) )
        _dataFilename = dataFilename;
    else
        throw std::system_error(ECANCELED, std::generic_category(), "Invalid file name!");
}


std::string FitterBonotto::getDataFilename() {
    return _dataFilename;
}


// ####################################################################################################################
// ################################################ INTERNAL FUNCTIONS ################################################
// ####################################################################################################################


// Read the file and place the values in the samples vector
void FitterBonotto::readFile()
{
    if(!_dataFilename.size())
      throw std::system_error(ECANCELED, std::generic_category(), "Empty data file!");

    std::string line;
    std::ifstream file(_dataFilename);

    if (file)
    {
        while (std::getline(file, line))
            samples->push_back(std::stod(line));
        
        file.close();
    }
    else
      throw std::system_error(ECANCELED, std::generic_category(), "Error when opening file");
}


// Calculates the average and deviation values from a data vector
void FitterBonotto::avgDev(double *avg, double *dev)
{
    for (auto value = samples->begin(); value != samples->end(); ++value)
        *avg += *value;

    *avg /= samples->size();

    for (auto value = samples->begin(); value != samples->end(); ++value)
        *dev += std::pow(*value - *avg, 2);

    *dev /= samples->size();
}


// Calculates the minimun and maximum values from data vector
void FitterBonotto::minMax(double *min, double *max)
{
    *min = samples->front();
    *max = samples->front();

    for (auto value = samples->begin(); value != samples->end(); ++value) {
            if (*value > *max)
                *max = *value;
            if (*value < *min)
                *min = *value;
    }
}


// Calculates the minimun, maximum and mode values from data vector
void FitterBonotto::minMaxMode(double *min, double *max, double *mode)
{ 
    std::unordered_map<double,int> numberMap;

    *min = samples->front();
    *max = samples->front();

    for (auto value = samples->begin(); value != samples->end(); ++value) {
        
        if (*value > *max)
            *max = *value;
        if (*value < *min)
            *min = *value;
        
        auto aux = numberMap.find(*value);
        if (aux != numberMap.end())
            numberMap[*value] = aux->second+1;
        else
            numberMap[*value] = 1;
    }

    int i = -1;
    for (auto value = numberMap.begin(); value != numberMap.end(); ++value)
        if (value->second > i) {
            *mode = value->first;
            i = value->second;
        }
}


// Calculates the average, deviation, minimun and maximum values from data vector
void FitterBonotto::avgDevMinMax(double *avg, double *dev, double *min, double *max)
{
    *min = samples->front();
    *max = samples->front();
    *avg = 0;
    *dev = 0;

    for (auto value = samples->begin(); value != samples->end(); ++value) {
        
        *avg += *value;

        if (*value > *max)
            *max = *value;
        if (*value < *min)
            *min = *value;           
    }

    *avg /= samples->size();

    for (auto value = samples->begin(); value != samples->end(); ++value)
        *dev += std::pow(*value - *avg, 2);

    *dev /= samples->size();
}


// Calculates the average, minimun and maximum values from data file
void FitterBonotto::avgMinMax(double *avg, double *min, double *max)
{
    *min = samples->front();
    *max = samples->front();
    *avg = 0;

    for (auto value = samples->begin(); value != samples->end(); ++value) {
        
        *avg += *value;

        if (*value > *max)
            *max = *value;
        if (*value < *min)
            *min = *value;           
    }

    *avg /= samples->size();
}


// Calculates the average, acale, minimun and maximum values from data vector
void FitterBonotto::avgShapeMinMax(double *avg, double *shape, double *min, double *max)
{
    double dev = 0;

    avgDevMinMax(avg, &dev, min, max);

    *shape = *avg * std::pow(*avg / dev, 2);
}


// Calculates the alpha, beta, minimun and maximum values from data vector
void FitterBonotto::alphaBetaMinMax(double *alpha, double *beta, double *min, double *max)
{
    double avg = 0;
    double dev = 0;

    avgDevMinMax(&avg, &dev, min, max);

    *alpha = ((1 - avg) / std::pow(dev, 2) - 1 / avg) * std::pow(avg, 2);
    *beta = *alpha * (1 / avg - 1);
}


// Calculates the alpha, beta, minimun and maximum values from data vector
void FitterBonotto::alphaScaleMinMax(double *alpha, double *scale, double *min, double *max)
{
    double avg = 0;
    double dev = 0;

    avgDevMinMax(&avg, &dev, min, max);

    *alpha = std::pow(dev / avg, -1.086);
    *scale = avg / std::tgamma(1 + 1 / *alpha);
}


//! Creates and calculates the classes frequencies
std::vector<int>* FitterBonotto::createClasses(double *min, double *max, double *range)
{    
    int numClasses = 1 + 3.322 * std::log10(samples->size());
    
    *range = (*max - *min) / numClasses;

    auto classes = new std::vector<int>(numClasses);

    for (auto value = samples->begin(); value != samples->end(); ++value)
        for (int i = 0; i < numClasses; i++)
            if (i == numClasses-1 || (*value >= (*min + i*(*range)) && *value < (*min + (i+1)*(*range)))) {
                classes->at(i) = classes->at(i) + 1;
                break;
            }

    return classes;
}


template<typename F, typename ... Args>
void FitterBonotto::sqrError(double *min, double *max, double *sqrerror, F f, Args ... args)
{
	*sqrerror = 0;
    double range = 0;
    auto classes = createClasses(min, max, &range);

	double a = *min;
	double b = a + range;

    Traits<Integrator_if>::Implementation integrator;

    for (int index = 0; index < classes->size(); index++, a = b, b += range) {
    	double area = integrator.integrate(a, b, f, args...);
    	
    	if (isnan(area) || area <= 0 || area >= 1)
    		continue;

    	double sup = pow(classes->at(index) - (area * samples->size()), 2);
    	double inf = (area * samples->size());

        *sqrerror += sup/inf;
    }
    
    samples->clear();
}