/*
 * SampleDecoder.h
 *
 * Any decoder must have the format below, i.e., implement the method decode(std::vector< double >&)
 * returning a double corresponding to the fitness of that vector. If parallel decoding is to be
 * used in the BRKGA framework, then the decode() method _must_ be thread-safe; the best way to
 * guarantee this is by adding 'const' to the end of decode() so that the property will be checked
 * at compile time.
 *
 * The chromosome inside the BRKGA framework can be changed if desired. To do so, just use the
 * first signature of decode() which allows for modification. Please use double values in the
 * interval [0,1) when updating, thus obeying the BRKGA guidelines.
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#ifndef SAMPLEDECODER_H
#define SAMPLEDECODER_H

#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include "SampleDecoder.h"

#include "MTRand.h"

// #include "Population.h"

using namespace std;

class SampleDecoder {
	private:
		fstream dataFileRead;
		ifstream arquivo;

		string fileName;
		string Buffer, linha, tmp;

		double seed, readerDouble;
		double valorTotalIptu = 0.0;
		const long unsigned rngSeed = 0;
	    // MTRand rng;

		int k, readerInt;
		int quantidadeAliquotas, quantidadeDadosIPTU;

		vector<string> listSetInstances;
		vector<double> listaNumero; // vector<int> listaNumero;
		vector<double> listaValorVenal, listaValorIptu;
		vector< vector<double> > tabelaDados;
		
		

	public:

		SampleDecoder(string _fileName);
		~SampleDecoder();

		void cleanMemory();

		double getSeed();

		// double construirSolucao(std::vector< double > &chromosome,vector< unsigned > &permutation) const;
		double construirSolucao(std::vector< double > &chromosome) const;

		double decode(std::vector< double >& chromosome) const;

		void printMatrix(vector< vector<double> > _matrix);
		
};

#endif
