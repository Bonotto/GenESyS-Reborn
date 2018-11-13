/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationControl.h
 * Author: cancian
 *
 * Created on 10 de Outubro de 2018, 18:01
 */

#ifndef SIMULATIONCONTROL_H
#define SIMULATIONCONTROL_H

#include <map>
#include "SimulationResponse.h"

class SimulationScenario;

/*!
 * Represents any possible parameter or control for a simulation. Any infrastructure or event the model can declare one of its own attribute as a simulation control. It just have to create a SimulationControl object, passing the access to the methods that gets and sets the control value and including this SimulationControl in the corresponding list of the model
 */
class SimulationControl: public SimulationResponse {
public:
	SimulationControl(std::string type, std::string name, memberFunctionGetDoubleVarHandler getHandler, memberFunctionSetDoubleVarHandler setHandler);
	SimulationControl(const SimulationControl& orig);
	SimulationControl() {}
	virtual ~SimulationControl();
public:
	void getValue(double value);
	
	double get(SimulationScenario *scenario) { return _map[scenario]; }
	void set(SimulationScenario *scenario, double value) { _map[scenario]=value; }

private:
	std::map<SimulationScenario*, double> _map;
	memberFunctionSetDoubleVarHandler _memberFunctionSetDoubleHandler; // a pointer to a member function that sets a double
};

#endif /* SIMULATIONCONTROL_H */

