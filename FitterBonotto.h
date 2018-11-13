/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FitterBonotto.h
 * Author: Bruno Bonotto
 *
 * Created on November 12, 2018, 4:18 PM
 */

#ifndef FITTERBONOTTO_H
#define FITTERBONOTTO_H

#include <vector>

#include "Sampler_if.h"
#include "Fitter_if.h"

class FitterBonotto: public Fitter_if {

public:
	FitterBonotto();
	FitterBonotto(const FitterBonotto& orig);
	
	~FitterBonotto() {
		delete sampler;
		delete samples;
	}

public:
	bool isNormalDistributed(double confidencelevel);
	void fitUniform (double *sqrerror, double *min, double *max);
	void fitTriangular (double *sqrerror, double *min, double *mo, double *max);
	void fitNormal (double *sqrerror, double *avg, double *stddev);
	void fitExpo (double *sqrerror, double *avg1);
	void fitErlang (double *sqrerror, double *avg, double *m);
	void fitBeta (double *sqrerror, double *alpha, double *beta, double *infLimit,double *supLimit);
	void fitWeibull (double *sqrerror, double *alpha, double *scale);
	void fitAll (double *sqrerror, std::string *name);

private:
	void readFile();
	void minMax(double *min, double *max);
	void avgDev(double *avg, double *dev);
	void avgMinMax(double *avg, double *min, double *max);
	void minMaxMode(double *min, double *max, double *mode);
	void avgDevMinMax(double *avg, double *dev, double *min, double *max);
	void avgShapeMinMax(double *avg, double *shape, double *min, double *max);
	void alphaBetaMinMax(double *alpha, double *beta, double *min, double *max);
	void alphaScaleMinMax(double *alpha, double *scale, double *min, double *max);
	
	std::vector<int>* createClasses(double *min, double *max, double *range);

	template<typename F, typename ... Args>
	void sqrError(double *min, double *max, double *sqrerror, F f, Args ... args);

public:
	void setDataFilename(std::string dataFilename);
	std::string getDataFilename();

private:
	Sampler_if *sampler;
	std::string _dataFilename = "";
	std::vector<double> *samples;
};

#endif /* FITTERBONOTTO_H */