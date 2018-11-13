/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BuildExperimentDesignTester.h
 * Author: bonotto
 *
 * Created on November 13, 2018, 5:46 PM
 */

#ifndef BUILDEXPERIMENTDESIGNTESTER_H
#define BUILDEXPERIMENTDESIGNTESTER_H

#include "GenesysApplication_if.h"

class BuildExperimentDesignTester : public GenesysApplication_if {
public:
	BuildExperimentDesignTester();
	
	int main(int argc, char** argv);
};

#endif /* BUILDEXPERIMENTDESIGNTESTER_H */
