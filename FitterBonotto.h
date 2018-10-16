/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FitterBonotto.h
 * Author: Bruno Bonotto
 *
 * Created on September 19, 2018, 4:18 PM
 */

#ifndef FITTERBONOTTO_H
#define FITTERBONOTTO_H

#include <cmath>
#include <regex>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "Fitter_if.h"
#include "Traits.h"

class FitterBonotto: public Fitter_if {

public:
	FitterBonotto():
		sampler(new Traits<Sampler_if>::Implementation()),
		distribution(new std::vector<double>()),
		samples(new std::vector<double>()) {}

	FitterBonotto(const FitterBonotto& orig):
		sampler(new Traits<Sampler_if>::Implementation()),
		distribution(new std::vector<double>()),
		samples(new std::vector<double>()) {}

	~FitterBonotto() {
		delete sampler;
		delete samples;
		delete distribution;
	}

public:
	bool isNormalDistributed(double confidencelevel);
	void fitUniform (double *sqrerror, double *min, double *max);
	void fitTriangular (double *sqrerror, double *min, double *mo, double *max);
	void fitNormal (double *sqrerror, double *avg, double *stddev);
	void fitExpo (double *sqrerror, double *avg1);
	void fitErlang (double *sqrerror, double *avg, int *m);
	void fitBeta (double *sqrerror, double *alpha, double *beta, double *infLimit,double *supLimit);
	void fitWeibull (double *sqrerror, double *alpha, double *scale);
	void fitAll (double *sqrerror, std::string *name);

private:
	void clean();
	void readFile();
	void avgDev(double *avg, double *dev, std::vector<double> *data);
	void minMax(double *min, double *max, std::vector<double> *data);
	void minMaxMode(double *min, double *max, double *mode, std::vector<double> *data);
	void avgDevMinMax(double *avg, double *dev, double *min, double *max, std::vector<double> *data);
	void avgMinMax(double *avg, double *min, double *max, std::vector<double> *data);
	void avgShapeMinMax(double *avg, int *shape, double *min, double *max, std::vector<double> *data);
	void alphaBetaMinMax(double *alpha, double *beta, double *min, double *max, std::vector<double> *data);
	void alphaScaleMinMax(double *alpha, double *scale, double *min, double *max, std::vector<double> *data);
	void endFit(double *min, double *max, double *sqrerror);
	std::vector<int>* createClasses(double *min, double *max, std::vector<double> *data);
	void computeError(double *sqrerror, std::vector<int> *data1, std::vector<int> *data2);

public:
	void setDataFilename(std::string dataFilename);
	std::string getDataFilename();

private:
	std::string _dataFilename = "";
	Traits<Sampler_if>::Implementation *sampler;
	std::vector<double> *distribution;
	std::vector<double> *samples;
};

#endif /* FITTERBONOTTO_H */