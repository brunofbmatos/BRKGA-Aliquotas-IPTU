/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */
#include <iostream>
#include <vector>
#include <new>
#include <float.h>
#include <string>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <math.h>
#include <list>
#include <algorithm>
#include "SampleDecoder.h"
#include <iomanip>
#include <stdlib.h>

#define _USE_MATH_DEFINES
 
#include <cmath>

using namespace std;

SampleDecoder::SampleDecoder(string _fileName) {
	cout<<"Entrou no metodo construtor!!!"<<endl;
  	fileName= _fileName;
  	dataFileRead.open(fileName.c_str(), fstream::in);
	cout<<"Passou pela abertura!!!"<<endl;
  	if(dataFileRead.is_open()){
  	  dataFileRead.seekg(0,ios::beg);
  	  //Aqui!!! cout<<"Operation successfully performed Aqui!!!"<<endl;
  	} else{
  	      cout<<"Não foi possivel abrir o arquivo!"<<endl;
  	      dataFileRead.clear();
  	      exit(1);
  	}
	dataFileRead>>Buffer;
	do{
		 listSetInstances.push_back(Buffer);
		  dataFileRead>>Buffer;
	}while(dataFileRead.good());	
	dataFileRead.clear();
	dataFileRead.close();
	Buffer.clear();
	for(unsigned i=0; i<listSetInstances.size();i++){
		linha = listSetInstances[i];
		stringstream linhaS(linha);
		k = 0;
	    while(getline(linhaS, tmp, ';')){
		 if (k == 1)
		 {
			stringstream tmpS(tmp);
			tmpS>>readerInt;
			listaNumero.push_back(readerInt);
		 }
		 else if (k == 2)
		 {
			stringstream tmpS(tmp);
			tmpS>>readerDouble; 
			listaValorVenal.push_back(readerDouble);
		 }
		 else if (k == 3)
		 {
			stringstream tmpS(tmp);
			tmpS>>readerDouble; 
			listaValorIptu.push_back(readerDouble);
			valorTotalIptu = valorTotalIptu + readerDouble;
		 }
		 k++;
	    }
	}

  seed = 1480813184;

}

SampleDecoder::~SampleDecoder() {

}

void SampleDecoder::cleanMemory() {
	
}

double SampleDecoder::getSeed(){
  return seed;
}

// double SampleDecoder::construirSolucao(std::vector< double > &chromosome,vector< unsigned > &permutation) const
double SampleDecoder::construirSolucao(std::vector< double > &chromosome) const {
	double fitness, valortotalvenal, readleDouble = 0;
	double rho, theta1, theta2; // parâmetros usados pelos modelo e obtidos pela leitura do cromossomo
	rho = chromosome[0];
	theta1 = chromosome[1];
	theta2 = chromosome[2];
	double xi, x1, x2, x3,x4,x5,x6,x7,x8;
	xi = 0.18;
	x1 = theta1 * pow(M_E,theta2 * xi); 
	x2 = x1 * pow(M_E,theta2 * x1); 
	x3 = x2 * pow(M_E,theta2 * x2); 
	x4 = x3 * pow(M_E,theta2 * x3); 
	x5 = x4 * pow(M_E,theta2 * x4); 
	x6 = x5 * pow(M_E,theta2 * x5); 
	x7 = x6 * pow(M_E,theta2 * x6); 
	x8 = x7 * pow(M_E,theta2 * x7); 
	cout<<"X1: "<<x1<<endl;
	cout<<"X2: "<<x2<<endl;
	cout<<"X3: "<<x3<<endl;
	cout<<"X4: "<<x4<<endl;
	cout<<"X5: "<<x5<<endl;
	cout<<"X6: "<<x6<<endl;
	cout<<"X7: "<<x7<<endl;
	cout<<"X8: "<<x8<<endl;
	for (int faixa = 1; faixa <= 8; faixa++) {
		if (faixa == 1) {
			fitness = fitness + (listaValorVenal[faixa] * x1);
		}
		else if (faixa == 2) {
			fitness = fitness + (listaValorVenal[faixa] * x2);
		}
		else if (faixa == 3) {
			fitness = fitness + (listaValorVenal[faixa] * x3);
		}
		else if (faixa == 4) {
			fitness = fitness + (listaValorVenal[faixa] * x4);
		}
		else if (faixa == 5) {
			fitness = fitness + (listaValorVenal[faixa] * x5);
		}
		else if (faixa == 6) {
			fitness = fitness + (listaValorVenal[faixa] * x6);
		}     
		else if (faixa == 7) {
			fitness = fitness + (listaValorVenal[faixa] * x7);
		}
		else if (faixa == 8) {
			fitness = fitness + (listaValorVenal[faixa] * x8);
		}
	}
	/*
	Desenvolve a lógica para calcular segundo o modelo matemático
	*/

	return fitness;
}

// Runs in \Theta(n \log n): // Exemplo consultado em brkga API
double SampleDecoder::decode(std::vector< double >& chromosome) const {
	double myFitness = 0.0;
	myFitness = construirSolucao(chromosome);
	cout<<"IPTU Total: "<<valorTotalIptu<<endl;
	cout<<"Fitness: "<<myFitness<<endl;
	if(myFitness < valorTotalIptu)
	{
		myFitness = myFitness * -1;
	}
	return myFitness;
}