/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BuildExperimentDesignTester.cpp
 * Author: bonotto
 * 
 * Created on November 13, 2018, 5:46 PM
 */

#include "BuildExperimentDesignTester.h"
#include "ExperementDesign_BrunoBonotto_JoaoSouto.h"
#include "ProcessAnalyserMyImpl1.h"
#include "SimulationResponse.h"
#include "SimulationControl.h"
#include <iostream>

//#include "traits_if.h"
#include "List.h"

BuildExperimentDesignTester::BuildExperimentDesignTester()
{}

int BuildExperimentDesignTester::main(int argc, char** argv)
{
	auto S1 = new SimulationScenario();
	auto S2 = new SimulationScenario();
	auto S3 = new SimulationScenario();
	auto S4 = new SimulationScenario();
	auto S5 = new SimulationScenario();
	auto S6 = new SimulationScenario();
	auto S7 = new SimulationScenario();
	auto S8 = new SimulationScenario();

	auto A = new SimulationControl();
	auto B = new SimulationControl();
	auto C = new SimulationControl();

	std::cout << "A = " << A << std::endl;
	std::cout << "B = " << B << std::endl;
	std::cout << "C = " << C << std::endl;

	auto R1 = new SimulationResponse();
	auto R2 = new SimulationResponse();

	A->set(S1, -1);
	A->set(S2, 1);
	A->set(S3, -1);
	A->set(S4, 1);
	A->set(S5, -1);
	A->set(S6, 1);
	A->set(S7, -1);
	A->set(S8, 1);

	B->set(S1, -1);
	B->set(S2, -1);
	B->set(S3, 1);
	B->set(S4, 1);
	B->set(S5, -1);
	B->set(S6, -1);
	B->set(S7, 1);
	B->set(S8, 1);

	C->set(S1, -1);
	C->set(S2, -1);
	C->set(S3, -1);
	C->set(S4, -1);
	C->set(S5, 1);
	C->set(S6, 1);
	C->set(S7, 1);
	C->set(S8, 1);

	R1->set(S1, 550);
	R1->set(S2, 669);
	R1->set(S3, 633);
	R1->set(S4, 642);
	R1->set(S5, 1037);
	R1->set(S6, 749);
	R1->set(S7, 1075);
	R1->set(S8, 729);

	R2->set(S1, 604);
	R2->set(S2, 650);
	R2->set(S3, 601);
	R2->set(S4, 635);
	R2->set(S5, 1052);
	R2->set(S6, 868);
	R2->set(S7, 1063);
	R2->set(S8, 860);

	auto controlList = new List<SimulationControl*>();
	auto responseList = new List<SimulationResponse*>();
	auto scenarioList = new List<SimulationScenario*>();

	controlList->insert(A);
	controlList->insert(B);
	controlList->insert(C);

	responseList->insert(R1);
	responseList->insert(R2);

	scenarioList->insert(S1);
	scenarioList->insert(S2);
	scenarioList->insert(S3);
	scenarioList->insert(S4);
	scenarioList->insert(S5);
	scenarioList->insert(S6);
	scenarioList->insert(S7);
	scenarioList->insert(S8);

	auto processAnalyser = new ProcessAnalyserMyImpl1(scenarioList, controlList, responseList);

	auto expDesign = new ExperementDesign_BrunoBonotto_JoaoSouto();

	expDesign->_processAnalyser = processAnalyser;

	expDesign->calculateContributionAndCoefficients();

	for (auto it: *expDesign->getContributions()) {
		
		std::cout << "Conjunto = " << std::endl;
		for (auto cont: *it->getControls())
		 	std::cout << cont << ", ";

		std::cout << std::endl << "contribution = " << it->getContribution() << " | coeficient = " << it->getModelCoefficient() << std::endl;
	}

}