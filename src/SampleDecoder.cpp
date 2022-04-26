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

#include "MTRand.h"

const double EulerConstant = std::exp(1.0);

using namespace std;

SampleDecoder::SampleDecoder(string _fileName) {
	//cout<<"Entrou no metodo construtor!!!"<<endl;

	

	quantidadeAliquotas = 8;
	quantidadeDadosIPTU = 3;

  	fileName= _fileName;

  	dataFileRead.open( fileName.c_str(), fstream::in);

	cout<<"Passou pela abertura!!!"<<endl;

  	if(dataFileRead.is_open()){
  	  dataFileRead.seekg(0, ios::beg);
  	  //Aqui!!! cout<<"Operation successfully performed Aqui!!!"<<endl;
  	} else{
  	    cout<<"Não foi possivel abrir o arquivo!"<<endl;
  	    dataFileRead.clear();
  	    exit(1);
  	}

	valorTotalIptu = 0;

	for(int i=0; i<quantidadeAliquotas; i++) {

		tabelaDados.push_back( vector<double>() );

		for(int j=0; j<quantidadeDadosIPTU; j++) {

			dataFileRead >> readerDouble;
			tabelaDados[i].push_back( readerDouble );

			if (j==0) {
				listaNumero.push_back( readerDouble ); // Antes listaNumero era int
			} else if (j==1) {
				listaValorVenal.push_back(readerDouble);
			} else if (j==2) {
				listaValorIptu.push_back(readerDouble);
				valorTotalIptu = valorTotalIptu + readerDouble;
			}

		}
	}

	dataFileRead.clear();
	dataFileRead.close();

	printMatrix(tabelaDados);

  seed = 1480813184;

}

SampleDecoder::~SampleDecoder() {

}

void SampleDecoder::cleanMemory() {
	
}

double SampleDecoder::getSeed(){
  return seed;
}

void SampleDecoder::printMatrix(vector< vector<double> > _matrix) {

	for(int i=0; i<_matrix.size(); i++) {
		for(int j=0; j<_matrix[i].size(); j++) {
			cout<<"  "<<_matrix[i][j];
		}
		cout<<endl;
	}

}


// double SampleDecoder::construirSolucao(std::vector< double > &chromosome,vector< unsigned > &permutation) const
double SampleDecoder::construirSolucao(std::vector< double > &chromosome) const {
	double fitness = 0, valortotalvenal, readleDouble = 0;
	double rho, theta1, theta2; // parâmetros usados pelos modelo e obtidos pela leitura do cromossomo
	double Xi; 
	double listaAliquotas[quantidadeAliquotas];

	// randInt( const uint32 n ); // integer in [0,n] for n < 2^32
	int randomValue, interval;
	double randomValueDouble;

	const long unsigned rngSeed = 0; // seed to the random number generator
	MTRand rng(rngSeed);

	Xi = chromosome[0];
	theta1 = chromosome[1]; // theta1 deve ser maior ou igual a 0.00042 e menor ou igual a 0.00098
	theta2 = chromosome[2]; // theta2 deve ser maior ou igual a 0.26670 e menor ou igual a 0.62230

	cout<<"\n\n\n=== Construir Solucao ==="<<endl;

	cout<<"EulerConstant = "<<EulerConstant<<endl;

	cout<<"Valores originais"<<endl;
	cout<<"theta1 = "<<theta1<<endl;
	cout<<"theta2 = "<<theta2<<endl;
	cout<<"Xi = "<<Xi<<endl;


    // theta1 deve ser maior ou igual a 0.00042 e menor ou igual a 0.00098
	if ( (theta1 >= 0.42) && (theta1 <= 0.98) ) {
		cout<<"Entrou if de theta1"<<endl;
		theta1 = theta1/1000.0;
	}
	else 
	{

		cout<<"Entrou else de theta1"<<endl;
		interval = 98 - 42;
		randomValue = rng.randInt(interval);
		randomValueDouble = 42 + randomValue;
		randomValueDouble = randomValueDouble/100.0;
		chromosome[1] = randomValueDouble; // checar como se faz
		randomValueDouble = randomValueDouble/1000.0;
		theta1 = randomValueDouble;
	}
	
	// theta2 deve ser maior ou igual a 0.26670 e menor ou igual a 0.62230
	if ( (theta2 >= 0.26670) && (theta2 <= 0.62230) ) {
		cout<<"Entrou if de theta2"<<endl;
	}
	else 
	{
		interval = 62230 - 26670;
		randomValue = rng.randInt(interval);
		randomValueDouble = 26670 + randomValue;
		randomValueDouble = randomValueDouble/100000.0;
		theta2 = randomValueDouble;
		chromosome[2] = randomValueDouble;
	}

	// X1 deve ser maior ou igual a 0.0010 e menor ou igual a 0.0020
	if ( (Xi > 0.09) && (Xi < 0.21)) {
		cout<<"Entrou if de Xi"<<endl;
		Xi = Xi /100.0;
	}
	else
	{        
		cout<<"Entrou else de  Xi"<<endl;
 		interval = 20 - 10;
		randomValue = rng.randInt(interval);
		randomValueDouble = 10 + randomValue;
		randomValueDouble = randomValueDouble/100.0;
		chromosome[0] = randomValueDouble;
		randomValueDouble = randomValueDouble/1000.0;
		Xi = randomValueDouble;
	}	

	// X1 deve ser maior ou igual a 0.0010 e menor ou igual a 0.0020
	//listaAliquotas[ 0 ] = theta1 * pow(EulerConstant, theta2 * Xi); // X1 deve ser maior ou igual a 0.0010 e menor ou igual a 0.0020

    //cout<<"Depois do IF que recalcula X1"<<endl;
    //cout<<"theta1 = "<<theta1<<endl;
	//cout<<"theta2 = "<<theta2<<endl;
	cout<<"Xi = "<<Xi<<endl;
	listaAliquotas[ 0 ] = Xi;
	cout<<"X1= "<<listaAliquotas[ 0 ]<<endl;

	if ( (listaAliquotas[ 0 ] <= 0.0010) && (listaAliquotas[ 0 ] >= 0.0020)) {
		cout<<"X1 não atende"<<endl;
		listaAliquotas[ 0 ] = Xi;
	}	
	listaAliquotas[ 0 ] = Xi;
	 


	cout<<"=== Aliquotas ==="<<endl;
	cout<<"X1 = "<<listaAliquotas[ 0 ]<<endl;
	for(int i=1; i < quantidadeAliquotas; i++) {
		listaAliquotas[i] = listaAliquotas[ i-1 ] * pow( EulerConstant,  theta2 * listaAliquotas[ i-1 ]);
		cout<<"X"<<i+1<<" = "<<listaAliquotas[i]<<endl;
	}

	if ( listaAliquotas[ quantidadeAliquotas -1 ] > 0.0140 ) {
		// X8 deve ser menor ou igual a 0.0140
		cout<<"X8 maior que 0.0140"<<endl;

		interval = 14;
		randomValue = rng.randInt(interval);
		randomValueDouble = randomValue;
		randomValueDouble = randomValueDouble/1000.0;
		listaAliquotas[ quantidadeAliquotas -1 ] = randomValueDouble;
		cout<<"X"<<quantidadeAliquotas -1<<" = "<<listaAliquotas[quantidadeAliquotas -1]<<endl;
	}
	
	for(int i=0; i<listaValorVenal.size(); i++) {
		fitness = fitness + ( listaValorVenal[ i ] * listaAliquotas[ i ] );
	}



	cout<<"\n=== Dados ==="<<endl;
	cout<<"theta1 = "<<theta1<<endl;
	cout<<"theta2 = "<<theta2<<endl;
	cout<<"Xi = "<<Xi<<endl;
	cout<<"X1 = "<<listaAliquotas[ 0 ]<<endl;
	cout<<"X8 = "<<listaAliquotas[ quantidadeAliquotas -1 ]<<endl;

	/*
	Desenvolve a lógica para calcular segundo o modelo matemático
	*/

	return fitness;
}

// Runs in \Theta(n \log n): // Exemplo consultado em brkga API
double SampleDecoder::decode(std::vector< double >& chromosome) const {
	double myFitness = 0.0;
	double penalidade = valorTotalIptu*0.15; // Penalidade de 15% relativo ao valor total do IPTU
	myFitness = construirSolucao(chromosome);

	cout<<"IPTU Total: "<<std::setprecision(21)<<valorTotalIptu<<endl;
	cout<<"Fitness: "<<std::setprecision(21)<<myFitness<<endl;
	
	if (myFitness < valorTotalIptu) {
	 	myFitness = myFitness - penalidade;
	} 
	
	myFitness = myFitness * -1.0;

	return myFitness;
}

// primeira sem penalidade - depois com penalidade