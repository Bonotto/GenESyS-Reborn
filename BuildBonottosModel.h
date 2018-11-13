/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BuildBonottosModel.h
 * Author: Bonotto
 *
 * Created on November 12, 2018, 10:33 PM
 */

#ifndef BUILDBONOTTOSMODEL_H
#define BUILDBONOTTOSMODEL_H

#include "GenesysApplication_if.h"

class BuildBonottosModel : public GenesysApplication_if {
public:
	BuildBonottosModel();

	void generate_file();
	
	int main(int argc, char** argv);
};

#endif /* BUILDBONOTTOSMODEL_H */
