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
#include "Structures.h"
#include "Utility.h"
#include "MTRand.h"

#define _USE_MATH_DEFINES
 
#include <cmath>


const double EulerConstant = std::exp(1.0);

using namespace std;

SampleDecoder::SampleDecoder(string _fileName) {
	//cout<<"Entrou no metodo construtor!!!"<<endl;

	

	quantidadeAliquotas = 8;
	quantidadeDadosIPTU = 3;

  	fileName= _fileName;

  	dataFileRead.open( fileName.c_str(), fstream::in);

	//cout<<"Passou pela abertura!!!"<<endl;

  	if(dataFileRead.is_open()){
  	  dataFileRead.seekg(0, ios::beg);
  	  //Aqui!!! cout<<"Operation successfully performed Aqui!!!"<<endl;
  	} else{
  	    //cout<<"Não foi possivel abrir o arquivo!"<<endl;
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

	//printMatrix(tabelaDados);

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
double SampleDecoder::construirSolucao(bool &verificacao, std::vector< double > &chromosome) const {
	double fitness = 0, valortotalvenal, readleDouble = 0;
	double theta1, theta2; // parâmetros usados pelos modelo e obtidos pela leitura do cromossomo
	double Xi; 
	double listaAliquotas[quantidadeAliquotas];
	// randInt( const uint32 n ); // integer in [0,n] for n < 2^32
	int randomValue, interval;
	double randomValueDouble;

	Xi = chromosome[0];
	theta1 = chromosome[1]; // theta1 deve ser maior ou igual a 0.00042 e menor ou igual a 0.00098
	theta2 = chromosome[2]; // theta2 deve ser maior ou igual a 0.26670 e menor ou igual a 0.62230

	//cout<<"\n\n\n=== Construir Solucao ==="<<endl;

	//cout<<"EulerConstant = "<<EulerConstant<<endl;

	//cout<<"Valores originais"<<endl;
	//cout<<theta1<<";"<<theta2<<";"<<Xi<<endl;


    // theta1 deve ser maior ou igual a 0.00042 e menor ou igual a 0.00098
	if ( (theta1 >= 0.42) && (theta1 <= 0.98) ) {
		//cout<<"Entrou if de theta1"<<endl;
		theta1 = theta1/1000.0;
	} else if (theta1 < 0.42) {
			theta1 = 0.42/1000.0;
	} else if (theta1 > 0.98) {
			theta1 = 0.98/1000.0;
	}
	
	
	// theta2 deve ser maior ou igual a 0.26670 e menor ou igual a 0.62230
	if (theta2 < 0.26670) {
		theta2 = 0.26670;
	} else if (theta2 > 0.62230) {
			theta2 = 0.62230;
	}


	// X1 deve ser maior ou igual a 0.0010 e menor ou igual a 0.0020
	// if ( (Xi == 1) && (Xi <= 1)) {
	// 	Xi = 1;
	// } else {        
	// 	Xi = 1;
	// }	

	// Aqui!!! Xi = 1;

	// X1 deve ser maior ou igual a 0.0010 e menor ou igual a 0.0020
	listaAliquotas[ 0 ] = theta1 * pow(EulerConstant, theta2 * Xi); // X1 deve ser maior ou igual a 0.0010 e menor ou igual a 0.0020

	if (listaAliquotas[ 0 ] < 0.0010) {
		listaAliquotas[ 0 ] = 0.0010;
    } else if (listaAliquotas[ 0 ] > 0.0020) {
			listaAliquotas[ 0 ] = 0.0020;
	}


	// cout<<"=== Aliquotas ==="<<endl;
	// cout<<"X1 = "<<listaAliquotas[ 0 ]<<endl;
	for(int i=1; i < quantidadeAliquotas; i++) {
		listaAliquotas[i] = listaAliquotas[ i-1 ] * pow( EulerConstant,  theta2 * Xi);
		// cout<<"X"<<i+1<<" = "<<listaAliquotas[i]<<endl;
	}

	if ( listaAliquotas[ quantidadeAliquotas -1 ] > 0.0140 ) {
		listaAliquotas[ quantidadeAliquotas -1 ] = 0.0140;
	}


	// cout<<"=== Aliquotas ==="<<endl;
	// for(int i=0; i < quantidadeAliquotas; i++) {
	//  cout<<"X"<<i+1<<" = "<<listaAliquotas[i]<<endl;
	// }


	// cout<<"Xi = "<<Xi<<endl;
	// cout<<"theta1 = "<<theta1<<endl;
	// cout<<"theta2 = "<<theta2<<endl;
	
	for(int i=0; i<listaValorVenal.size(); i++) {
		fitness = fitness + ( listaValorVenal[ i ] * listaAliquotas[ i ] );
		// cout<<"X"<<i+1<<" = "<<listaAliquotas[i]<<endl;
	}


    bool teste = true;
	for(int i=0; i < quantidadeAliquotas-1; i++) {
		if(listaAliquotas[i] > listaAliquotas[i + 1]) {
			// cout<<"Entrou no if de restrição"<<endl;
			// cout<<listaAliquotas[i] <<" > "<< listaAliquotas[i + 1]<<endl;
			teste = false;
			break;
		}
	}

	verificacao = teste;
	
	return fitness;
}

// Runs in \Theta(n \log n): // Exemplo consultado em brkga API
double SampleDecoder::decode(std::vector< double > &chromosome) const {
	double myFitness = 0.0;
	double taxa = 100.0;
	double penalidade = (valorTotalIptu*taxa)/100.0; // Penalidade de 50% relativo ao valor total do IPTU
	bool verificacao = false;

	// cout<<"\n\n\n=== Decode ==="<<endl;


	myFitness = construirSolucao(verificacao, chromosome);
	
	// cout<<std::setprecision(21)<<myFitness<<endl;
	// cout<<"myFitness      = "<<std::setprecision(12)<<myFitness<<endl;
	// cout<<"valorTotalIptu = "<<std::setprecision(12)<<valorTotalIptu<<endl;
	// cout<<"penalidade = "<<std::setprecision(12)<<penalidade<<endl;

	// if (myFitness> valorTotalIptu)
	//  cout<<"myFitness maior"<<endl;
	

	// cout<<"Verificacao "<<verificacao<<endl;
	// string msg = (verificacao==true)? "verdadeiro":"falso";
	// cout<<"Verificacao: "<<msg<<endl;

	// cout<<"Penalidade "<<std::setprecision(21)<<penalidade<<endl;


	if (verificacao == false) {
		//cout<<"Entrou no if de restrição decode"<<endl;
		//cout<<verificacao<<endl;
		//cout<<std::setprecision(21)<<myFitness<<endl;
		penalidade = (myFitness*75)/100.0;
		myFitness = myFitness - penalidade;
		//cout<<std::setprecision(21)<<myFitness<<endl;

		// * cout<<"penalidade = "<<std::setprecision(12)<<penalidade<<endl;
		// * cout<<"myFitness depois da penalidade   = "<<std::setprecision(12)<<myFitness<<endl;
	}

	myFitness = myFitness * -1.0;

	// * cout<<"\n\n\n";

	return myFitness;
}


void SampleDecoder::verificarSolucao(std::vector< double > &chromosome, int execucao) const {
	double fitness = 0, valortotalvenal, readleDouble = 0;
	double theta1, theta2; // parâmetros usados pelos modelo e obtidos pela leitura do cromossomo
	double Xi; 
	double listaAliquotas[quantidadeAliquotas];
	// randInt( const uint32 n ); // integer in [0,n] for n < 2^32
	int randomValue, interval;
	double randomValueDouble;


	bool verificacao = false;



	Xi = chromosome[0];
	theta1 = chromosome[1]; // theta1 deve ser maior ou igual a 0.00042 e menor ou igual a 0.00098
	theta2 = chromosome[2]; // theta2 deve ser maior ou igual a 0.26670 e menor ou igual a 0.62230

	//cout<<"\n\n\n=== Construir Solucao ==="<<endl;

	//cout<<"EulerConstant = "<<EulerConstant<<endl;

	//cout<<"Valores originais"<<endl;
	//cout<<theta1<<";"<<theta2<<";"<<Xi<<endl;


    // theta1 deve ser maior ou igual a 0.00042 e menor ou igual a 0.00098
	if ( (theta1 >= 0.42) && (theta1 <= 0.98) ) {
		//cout<<"Entrou if de theta1"<<endl;
		theta1 = theta1/1000.0;
	} else if (theta1 < 0.42) {
			theta1 = 0.42/1000.0;
	} else if (theta1 > 0.98) {
			theta1 = 0.98/1000.0;
	}
	
	
	// theta2 deve ser maior ou igual a 0.26670 e menor ou igual a 0.62230
	if (theta2 < 0.26670) {
		theta2 = 0.26670;
	} else if (theta2 > 0.62230) {
			theta2 = 0.62230;
	}


	// X1 deve ser maior ou igual a 0.0010 e menor ou igual a 0.0020
	// if ( (Xi == 1) && (Xi <= 1)) {
	// 	Xi = 1;
	// } else {        
	// 	Xi = 1;
	// }	

	// Aqui!!! Xi = 1;


	// X1 deve ser maior ou igual a 0.0010 e menor ou igual a 0.0020
	listaAliquotas[ 0 ] = theta1 * pow(EulerConstant, theta2 * Xi); // X1 deve ser maior ou igual a 0.0010 e menor ou igual a 0.0020


	// if ( (listaAliquotas[ 0 ] < 0.0010) || (listaAliquotas[ 0 ] > 0.0020)) {
	// 	cout<<"X1 não atende"<<endl;
	// 	interval = 20 - 10;
	// 	randomValue = rng.randInt(interval);
	// 	randomValueDouble = 10 + randomValue;
	// 	randomValueDouble = randomValueDouble/10000.0;
	// 	listaAliquotas[ 0 ] = randomValueDouble;
	// 	cout<<"X1" <<" = "<<listaAliquotas[0]<<endl;
    // } 

	if (listaAliquotas[ 0 ] < 0.0010) {
		listaAliquotas[ 0 ] = 0.0010;
    } else if (listaAliquotas[ 0 ] > 0.0020) {
			listaAliquotas[ 0 ] = 0.0020;
	}


	// cout<<"=== Aliquotas ==="<<endl;
	// cout<<"X1 = "<<listaAliquotas[ 0 ]<<endl;
	for(int i=1; i < quantidadeAliquotas; i++) {
		listaAliquotas[i] = listaAliquotas[ i-1 ] * pow( EulerConstant,  theta2 * Xi);
		// cout<<"X"<<i+1<<" = "<<listaAliquotas[i]<<endl;
	}

	if ( listaAliquotas[ quantidadeAliquotas -1 ] > 0.0140 ) {
		listaAliquotas[ quantidadeAliquotas -1 ] = 0.0140;
	}


	// cout<<"=== Aliquotas ==="<<endl;
	// for(int i=0; i < quantidadeAliquotas; i++) {
	//  cout<<"X"<<i+1<<" = "<<listaAliquotas[i]<<endl;
	// }


	// cout<<"Xi = "<<Xi<<endl;
	// cout<<"theta1 = "<<theta1<<endl;
	// cout<<"theta2 = "<<theta2<<endl;
	
	for(int i=0; i<listaValorVenal.size(); i++) {
		fitness = fitness + ( listaValorVenal[ i ] * listaAliquotas[ i ] );
		// cout<<"X"<<i+1<<" = "<<listaAliquotas[i]<<endl;
	}


    bool teste = true;
	for(int i=0; i < quantidadeAliquotas-1; i++) {
		if(listaAliquotas[i] > listaAliquotas[i + 1]) {

			// cout<<"Entrou no if de restrição"<<endl;
			// cout<<listaAliquotas[i] <<" > "<< listaAliquotas[i + 1]<<endl;
			teste = false;
			break;
		}
	}



	verificacao = teste;


	double penalidade = 0.0;

	if(verificacao == false)
	{
		//cout<<"Entrou no if de restrição decode"<<endl;
		//cout<<verificacao<<endl;
		//cout<<std::setprecision(21)<<fitness<<endl;
		penalidade = (fitness*75)/100.0;
		fitness = fitness - penalidade;
		//cout<<std::setprecision(21)<<fitness<<endl;

		// * cout<<"penalidade = "<<std::setprecision(12)<<penalidade<<endl;
		// * cout<<"fitness depois da penalidade   = "<<std::setprecision(12)<<fitness<<endl;
	}



	ofstream dataFileWrite;
	string ArquivoChecagemSolucao = "resultados/checagem-solucao.txt";
	
	dataFileWrite.open(ArquivoChecagemSolucao.c_str(), std::ofstream::out | std::ofstream::app);

	if(!dataFileWrite.is_open()) {
	     cout<<"Não foi possivel abrir o arquivo dataFileWrite!: "<<ArquivoChecagemSolucao<<endl;
	     dataFileWrite.clear();
	     exit(1);
	}

	// cout<<std::setprecision(21)<<fitness<<endl;

	dataFileWrite<<execucao+1<<"  "<<std::setprecision(21)<<fitness<<" "<<theta1<<"  "<<theta2<<"  "<<Xi<<"  "<<listaAliquotas[0]<<"  "<<listaAliquotas[1]<<"  "<<listaAliquotas[2]<<"  "<<listaAliquotas[3]<<"  "<<listaAliquotas[4]<<"  "<<listaAliquotas[5]<<"  "<<listaAliquotas[6]<<"  "<<listaAliquotas[7]<<"  "<<verificacao<<"\n";
	
	dataFileWrite.clear();
	dataFileWrite.close();
}


// primeira sem penalidade - depois com penalidade
// Penalidade 