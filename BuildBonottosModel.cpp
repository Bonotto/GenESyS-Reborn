/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BuildBonottosModel.cpp
 * Author: bonotto
 * 
 * Created on November 12, 2018, 10:33 PM
 */

#include "BuildBonottosModel.h"
#include "Traits.h"

#include <iostream>
#include <fstream>

BuildBonottosModel::BuildBonottosModel() {

}

int BuildBonottosModel::main(int argc, char** argv)
{
	// generate_file();

	auto fitter = new Traits<Fitter_if>::Implementation();

	double m = 0;
	double mo = 0;
	double avg = 0;
	double min = 0;
	double max = 0;
	double beta = 0;
	double alpha = 0;
	double scale = 0;
	double stddev = 0;
	double sqrerror = 0;
	std::string name = "";

	// #################################################
	// ############# CASE TEST 1 - UNIFORM #############
	// #################################################

	fitter->setDataFilename("../testUniform");

	fitter->fitUniform (&sqrerror, &min, &max);

	std::cout << "Uniform: "	<< "sqrerror = " << sqrerror
								<< " | min = " << min
								<< " | max = " << max
								<< std::endl << std::flush;

	// ####################################################
	// ############# CASE TEST 2 - TRIANGULAR #############
	// ####################################################

	fitter->setDataFilename("../testTriangular");

	fitter->fitTriangular (&sqrerror, &min, &mo, &max);

	std::cout << "Triangular: "	<< "sqrerror = " << sqrerror
								<< " | min = " << min
								<< " | mo = " << mo
								<< " | max = " << max
								<< std::endl << std::flush;

	// ################################################
	// ############# CASE TEST 3 - NORMAL #############
	// ################################################

	fitter->setDataFilename("../testNormal");
	
	fitter->fitNormal (&sqrerror, &avg, &stddev);

	std::cout << "Normal: "	<< "sqrerror = " << sqrerror
							<< " | avg = " << avg
							<< " | stddev = " << stddev
							<< std::endl << std::flush;

	// #####################################################
	// ############# CASE TEST 4 - EXPONENTIAL #############
	// #####################################################

	fitter->setDataFilename("../testExponential");
	
	fitter->fitExpo (&sqrerror, &avg);

	std::cout << "Exponential: "	<< "sqrerror = " << sqrerror
									<< " | avg = " << avg
									<< std::endl << std::flush;

	// ################################################
	// ############# CASE TEST 5 - ERLANG #############
	// ################################################

	fitter->setDataFilename("../testErlang");
	
	fitter->fitErlang (&sqrerror, &avg, &m);

	std::cout << "Erlang: "	<< "sqrerror = " << sqrerror
							<< " | avg = " << avg
							<< " | m = " << m
							<< std::endl << std::flush;

	// ##############################################
	// ############# CASE TEST 6 - BETA #############
	// ##############################################

	fitter->setDataFilename("../testBeta");
	
	fitter->fitBeta (&sqrerror, &alpha, &beta, &min, &max);

	std::cout << "Beta: "	<< "sqrerror = " << sqrerror
							<< " | alpha = " << alpha
							<< " | beta = " << beta
							<< " | min = " << min
							<< " | max = " << max
							<< std::endl << std::flush;

	// #################################################
	// ############# CASE TEST 7 - WEIBULL #############
	// #################################################

	fitter->setDataFilename("../testWeibull");
	
	fitter->fitWeibull (&sqrerror, &alpha, &scale);

	std::cout << "Weibull: "	<< "sqrerror = " << sqrerror
								<< " | alpha = " << alpha
								<< " | scale = " << scale
								<< std::endl << std::flush;

	// #################################################
	// ############# CASE TEST 8 - FIT ALL #############
	// #################################################

	fitter->setDataFilename("../testUniform");
	
	fitter->fitAll (&sqrerror, &name);

	std::cout << "(Uniform) All: "	<< "sqrerror = " << sqrerror
									<< " | name = " << name
									<< std::endl << std::flush;

	fitter->setDataFilename("../testTriangular");
	
	fitter->fitAll (&sqrerror, &name);

	std::cout << "(Triangular) All: "	<< "sqrerror = " << sqrerror
										<< " | name = " << name
										<< std::endl << std::flush;

	fitter->setDataFilename("../testNormal");
	
	fitter->fitAll (&sqrerror, &name);

	std::cout << "(Normal) All: "	<< "sqrerror = " << sqrerror
									<< " | name = " << name
									<< std::endl << std::flush;

	fitter->setDataFilename("../testExponential");
	
	fitter->fitAll (&sqrerror, &name);

	std::cout << "(Exponential) All: "	<< "sqrerror = " << sqrerror
										<< " | name = " << name
										<< std::endl << std::flush;

	fitter->setDataFilename("../testErlang");
	
	fitter->fitAll (&sqrerror, &name);

	std::cout << "(Erlang) All: "	<< "sqrerror = " << sqrerror
									<< " | name = " << name
									<< std::endl << std::flush;

	fitter->setDataFilename("../testBeta");
	
	fitter->fitAll (&sqrerror, &name);

	std::cout << "(Beta) All: "	<< "sqrerror = " << sqrerror
									<< " | name = " << name
									<< std::endl << std::flush;

	fitter->setDataFilename("../testWeibull");
	
	fitter->fitAll (&sqrerror, &name);

	std::cout << "(Weibull) All: "	<< "sqrerror = " << sqrerror
									<< " | name = " << name
									<< std::endl << std::flush;

	delete fitter;
};

void BuildBonottosModel::generate_file()
{
	auto sampler = new Traits<Sampler_if>::Implementation();

	std::ofstream file("../testUniform", std::ios::out);

    if (!file.is_open())
    	throw std::system_error(ECANCELED, std::generic_category(), "Can not open file!");

    for (int i = 0; i < 1000; i++)
		file << sampler->sampleUniform(0, 100) << std::endl;

	file.close();

	file.open("../testTriangular", std::ios::out);

    if (!file.is_open())
    	throw std::system_error(ECANCELED, std::generic_category(), "Can not open file!");

    for (int i = 0; i < 1000; i++)
		file << sampler->sampleTriangular(0, 50, 100) << std::endl;

	file.close();

	file.open("../testNormal", std::ios::out);

    if (!file.is_open())
    	throw std::system_error(ECANCELED, std::generic_category(), "Can not open file!");

    for (int i = 0; i < 1000; i++)
		file << sampler->sampleNormal(50, 10) << std::endl;

	file.close();

	file.open("../testExponential", std::ios::out);

    if (!file.is_open())
    	throw std::system_error(ECANCELED, std::generic_category(), "Can not open file!");

    for (int i = 0; i < 1000; i++)
		file << sampler->sampleExponential(50) << std::endl;

	file.close();

	file.open("../testErlang", std::ios::out);

    if (!file.is_open())
    	throw std::system_error(ECANCELED, std::generic_category(), "Can not open file!");

    for (int i = 0; i < 1000; i++)
		file << sampler->sampleErlang(50, 10) << std::endl;

	file.close();

	file.open("../testBeta", std::ios::out);

    if (!file.is_open())
    	throw std::system_error(ECANCELED, std::generic_category(), "Can not open file!");

    for (int i = 0; i < 1000; i++)
		file << sampler->sampleBeta(50, 10, 0, 100) << std::endl;

	file.close();

	file.open("../testWeibull", std::ios::out);

    if (!file.is_open())
    	throw std::system_error(ECANCELED, std::generic_category(), "Can not open file!");

    for (int i = 0; i < 1000; i++)
		file << sampler->sampleWeibull(50, 10) << std::endl;

	file.close();
	delete sampler;
}