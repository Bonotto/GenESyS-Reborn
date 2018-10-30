/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExperementDesign_BrunoBonotto_JoaoSouto.cpp
 * Author: Bruno Bonotto and João Vicente Souto
 * 
 * Created on October 30, 2018, 2:31 PM
 */

#include "ExperementDesign_BrunoBonotto_JoaoSouto.h"
#include "Assign.h"
#include <map>


ExperementDesign_BrunoBonotto_JoaoSouto::ExperementDesign_BrunoBonotto_JoaoSouto() {}

std::list<FactorOrInteractionContribution*>* ExperementDesign_BrunoBonotto_JoaoSouto::getContributions() const
{
	return _contributions;
}

ExperementDesign_BrunoBonotto_JoaoSouto::ExperementDesign_BrunoBonotto_JoaoSouto(const ExperementDesign_BrunoBonotto_JoaoSouto& orig) {}

ExperementDesign_BrunoBonotto_JoaoSouto::~ExperementDesign_BrunoBonotto_JoaoSouto() {}

bool ExperementDesign_BrunoBonotto_JoaoSouto::generate2krScenarioExperiments() { return true; }

ProcessAnalyser_if* ExperementDesign_BrunoBonotto_JoaoSouto::getProcessAnalyser() const { return _processAnalyser; }


/*
*/

int factorial(int n) {
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

int combination(int n, int s) {
	return factorial(n) / (factorial(s) * factorial(n - s));
}

int total_combination(int n) {
	int total = 0;

	for (int s = 1; s <= n; s++)
		total += combination(n, s);

	return total;
}

bool ExperementDesign_BrunoBonotto_JoaoSouto::calculateContributionAndCoefficients()
{

	// Passo 0	Contruir a tabela com os valores dos controles e respostas

	std::map<SimulationControl_if*, std::map<double, double>> levels;

	for (auto scenario : _processAnalyser->getScenarios())
		for (auto control : _processAnalyser->getControls())
			levels[control][scenario->getControlValue(control)] = 0;

	for (auto l : levels) {
		auto control = l.first;
		auto it = l.second.begin();

		levels[control][*it] = -1;
		levels[control][*(it++)] = 1;
	}

	//! Build combination
	std::set<std::set<SimulationControl*>> sets;
	int k = _processAnalyser->getControls()->size();

	for (auto control : _processAnalyser->getControls())
		sets.insert({control});

	while (sets.size() != total_combination(k)) {

		for (auto control : _processAnalyser->getControls())
			for (std::set<SimulationControl*> set : sets) {
				set.insert(control);
				sets.insert(set);
			}
	}

	std::map<SimulationScenario*, std::map<FactorOrInteractionContribution*, double>> table;

	for (auto set : sets) {
		auto combination = new FactorOrInteractionContribution(0, 0, new std::list<SimulationControl*>(set.begin(), set.end()));
		_contributions.push_back(combination);

		for (auto scenario : _processAnalyser->getScenarios())
			double x = 1;
			for (auto control : set)
				x *= levels[control][scenario->getControlValue(control)];

			table[scenario][combination] = x;
	}

	// Passo 1	soma das respostas
	double response_avg = 0;
	std::map<SimulationScenario*, double> responses_sum;
	for (auto scenario : _processAnalyser->getScenarios()) {
		responses_sum[scenario] = 0;
		for (auto response : _processAnalyser->getResponses())
			responses_sum[scenario] += scenario->getResponseValue(response);

		response_avg += responses_sum[scenario];
	}

	// Passo 2	respostas médias e média total
	response_avg /= (_processAnalyser->getResponses() * _processAnalyser->getScenarios());

	// Passo 3	diferenças entre o valor das respostas e a resposta média total
	// Passo 4 quadrado das diferenças do passo anterior
	// Passo 5	cálculo do SST
	double SST = 0;
	for (auto scenario : _processAnalyser->getScenarios())
		for (auto response : _processAnalyser->getResponses())
			SST += pow(scenario->getResponseValue(response) - response_avg, 2);
	
	// Passo 6	calcular a soma dos efeitos
	std::map<key, value> map;
	for (auto combination : _contributions)
		for (auto scenario : _processAnalyser->getScenarios())

	// Passo 7	calcular a média dos efeitos 
	// Passo 8	calcular o SS para cada iteração
	// Passo 9	Calcular a contribuição de cada fator
	// Passo 10	calcular o coeficiente do modelo de cada iteração


    return true;
}


