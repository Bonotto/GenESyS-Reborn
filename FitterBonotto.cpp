/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FitterBonotto.cpp
 * Author: Bruno Bonotto
 * 
 * Created on October 08, 2018, 10:29 PM
 */


#include "FitterBonotto.h"


// ####################################################################################################################
// ################################################ EXTERNAL FUNCTIONS ################################################
// ####################################################################################################################


bool FitterBonotto::isNormalDistributed(double confidencelevel)
{
    double avg = 0;
    double dev = 0;
    
    readFile();
    avgDev(&avg, &dev, samples);

    for (int i = 0; i < samples->size(); i++)
        distribution->push_back(sampler->sampleNormal(avg, dev));

    avgDev(&avg, &dev, distribution);

    auto hypTester = new Traits<HypothesisTester_if>::Implementation();
    hypTester->setDataFilename(_dataFilename);

    clean();

    return hypTester->testAverage(confidencelevel, avg, HypothesisTester_if::EQUAL) &&
           hypTester->testVariance(confidencelevel, sqrt(dev), HypothesisTester_if::EQUAL);
}


void FitterBonotto::fitUniform(double *sqrerror, double *min, double *max)
{
    readFile();
    minMax(min, max, samples);

    for (int i = 0; i < samples->size(); i++)
        distribution->push_back(sampler->sampleUniform(*min, *max));

    endFit(min, max, sqrerror);
}


void FitterBonotto::fitTriangular(double *sqrerror, double *min, double *mo, double *max)
{
    readFile();
    minMaxMode(min, max, mo, samples);

    for (int i = 0; i < samples->size(); i++)
        distribution->push_back(sampler->sampleTriangular(*min, *mo, *max));

    endFit(min, max, sqrerror);
}


void FitterBonotto::fitNormal(double *sqrerror, double *avg, double *stddev)
{
    double min = 0;
    double max = 0;

    readFile();
    avgDevMinMax(avg, stddev, &min, &max, samples);

    for (int i = 0; i < samples->size(); i++)
        distribution->push_back(sampler->sampleNormal(*avg, *stddev));

    endFit(&min, &max, sqrerror);
}


void FitterBonotto::fitExpo(double *sqrerror, double *avg1)
{
    double min = 0;
    double max = 0;

    readFile();
    avgMinMax(avg1, &min, &max, samples);

    for (int i = 0; i < samples->size(); i++)
        distribution->push_back(sampler->sampleExponential(*avg1));

    endFit(&min, &max, sqrerror);
}


void FitterBonotto::fitErlang(double *sqrerror, double *avg, int *m)
{
    double min = 0;
    double max = 0;

    readFile();
    avgShapeMinMax(avg, m, &min, &max, samples);

    for (int i = 0; i < samples->size(); i++)
        distribution->push_back(sampler->sampleErlang(*avg, *m));

    endFit(&min, &max, sqrerror);
}


void FitterBonotto::fitBeta(double *sqrerror, double *alpha, double *beta, double *infLimit,double *supLimit)
{
   readFile();
   alphaBetaMinMax(alpha, beta, infLimit, supLimit, samples);

   for (int i = 0; i < samples->size(); i++)
       distribution->push_back(sampler->sampleBeta(*alpha, *beta, *infLimit, *supLimit));

   endFit(infLimit, supLimit, sqrerror);
}


void FitterBonotto::fitWeibull(double *sqrerror, double *alpha, double *scale)
{
    double min = 0;
    double max = 0;

    readFile();
    alphaScaleMinMax(alpha, scale, &min, &max, samples);

    for (int i = 0; i < samples->size(); i++)
        distribution->push_back(sampler->sampleWeibull(*alpha, *scale));

    endFit(&min, &max, sqrerror);
}


void FitterBonotto::fitAll(double *sqrerror, std::string *name)
{
    int x = 0;
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

   fitErlang(&error, &a, &x);

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
    if ( std::regex_match(dataFilename, std::regex("[a-zA-Z0-9_]+")) )
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


// Clean the samples and distribution vectors
void FitterBonotto::clean()
{
    samples->clear();
    distribution->clear();
}


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
void FitterBonotto::avgDev(double *avg, double *dev, std::vector<double> *data)
{
    for (auto value = data->begin(); value != data->end(); ++value)
        *avg += *value;

    *avg /= data->size();

    for (auto value = data->begin(); value != data->end(); ++value)
        *dev += std::pow(*value - *avg, 2);

    *dev /= data->size();
}


// Calculates the minimun and maximum values from data vector
void FitterBonotto::minMax(double *min, double *max, std::vector<double> *data)
{
    *min = data->front();
    *max = data->front();

    for (auto value = data->begin(); value != data->end(); ++value) {
            if (*value > *max)
                *max = *value;
            if (*value < *min)
                *min = *value;
    }
}


// Calculates the minimun, maximum and mode values from data vector
void FitterBonotto::minMaxMode(double *min, double *max, double *mode, std::vector<double> *data)
{ 
    std::unordered_map<double,int> numberMap;

    *min = data->front();
    *max = data->front();

    for (auto value = data->begin(); value != data->end(); ++value) {
        
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
void FitterBonotto::avgDevMinMax(double *avg, double *dev, double *min, double *max, std::vector<double> *data)
{
    *min = data->front();
    *max = data->front();
    *avg = 0;
    *dev = 0;

    for (auto value = data->begin(); value != data->end(); ++value) {
        
        *avg += *value;

        if (*value > *max)
            *max = *value;
        if (*value < *min)
            *min = *value;           
    }

    *avg /= data->size();

    for (auto value = data->begin(); value != data->end(); ++value)
        *dev += std::pow(*value - *avg, 2);

    *dev /= data->size();
}


// Calculates the average, minimun and maximum values from data file
void FitterBonotto::avgMinMax(double *avg, double *min, double *max, std::vector<double> *data)
{
    *min = data->front();
    *max = data->front();
    *avg = 0;

    for (auto value = data->begin(); value != data->end(); ++value) {
        
        *avg += *value;

        if (*value > *max)
            *max = *value;
        if (*value < *min)
            *min = *value;           
    }

    *avg /= data->size();
}


// Calculates the average, acale, minimun and maximum values from data vector
void FitterBonotto::avgShapeMinMax(double *avg, int *shape, double *min, double *max, std::vector<double> *data)
{
    double dev = 0;

    avgDevMinMax(avg, &dev, min, max, data);

    *shape = (int) (*avg * std::pow(*avg / dev, 2));
}


// Calculates the alpha, beta, minimun and maximum values from data vector
void FitterBonotto::alphaBetaMinMax(double *alpha, double *beta, double *min, double *max, std::vector<double> *data)
{
    double avg = 0;
    double dev = 0;

    avgDevMinMax(&avg, &dev, min, max, data);

    *alpha = ((1 - avg) / std::pow(dev, 2) - 1 / avg) * std::pow(avg, 2);
    *beta = *alpha * (1 / avg - 1);
}


// Calculates the alpha, beta, minimun and maximum values from data vector
void FitterBonotto::alphaScaleMinMax(double *alpha, double *scale, double *min, double *max, std::vector<double> *data)
{
    double avg = 0;
    double dev = 0;

    avgDevMinMax(&avg, &dev, min, max, data);

    *alpha = std::pow(dev / avg, -1.086);
    *scale = avg / std::tgamma(1 + 1 / *alpha);
}


// Ends the square error from samples
void FitterBonotto::endFit(double *min, double *max, double *sqrerror)
{
    double distMin = 0;
    double distMax = 0;

    minMax(&distMin, &distMax, distribution);

    computeError(sqrerror, createClasses(min, max, samples), createClasses(&distMin, &distMax, distribution));

    clean();
}


//! Creates and calculates the classes frequencies
std::vector<int>* FitterBonotto::createClasses(double *min, double *max, std::vector<double> *data)
{
    int numClasses = 1 + 3.322 * std::log10(samples->size());
    auto range = (*max - *min) / numClasses;
    int values[numClasses];

    for (int i = 0; i < numClasses; i++)
        values[i] = 0;

    for (auto value = data->begin(); value != data->end(); ++value)
        for (int i = 0; i < numClasses; i++)
            if (i == numClasses-1 || (*value >= (*min + i*range) && *value < (*min + (i+1)*range))) {
                values[i]++;
                break;
            }

    auto *classes = new std::vector<int>();

    for (int i = 0; i < numClasses; i++)
        classes->push_back(values[i]);

    return classes;
}


//! Calculates the error between two samples
void FitterBonotto::computeError(double *sqrerror, std::vector<int> *data1, std::vector<int> *data2)
{
    *sqrerror = 0;

    for (auto sValue = data1->begin(), dValue = data2->begin(); sValue != data1->end() && dValue != data2->end(); ++sValue, ++dValue)
        *sqrerror += std::pow( ((double)*sValue-*dValue) / samples->size() , 2);
}
