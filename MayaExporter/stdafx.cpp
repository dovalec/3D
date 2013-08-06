// stdafx.cpp : source file that includes just the standard includes
// Exporter.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

void err_code(MStatus & stat)
{
	if( stat != MS::kSuccess ) { 
		std::cout << "[ERROR: " << stat.errorString().asChar() << "]" <<std::endl; 		
		exit(-1);
	}
}

void err_stop(string message)
{
	std::cout << "[ERROR: " << message.c_str() << "]" <<std::endl; 
	//Beep(3000,1000);
    printf("\a");
	exit(-1);}


void err_info(string message)
{
	//Beep(3000,200);
	std::cout << "[INFO: " << message.c_str() << "]" <<std::endl; 
}

void err_print(string message)
{
	std::cout << "[" << message.c_str() << "]" <<std::endl; 
}

void err_progress()
{
	std::cout << "."; 
}