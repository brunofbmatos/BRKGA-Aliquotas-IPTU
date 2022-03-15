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

//#include "Population.h"

using namespace std;

class SampleDecoder {
	private:
		fstream dataFileRead;
		string fileName;
		double seed, readerDouble;
		ifstream arquivo;
		string Buffer, linha, tmp;
		vector<string> listSetInstances;
		vector<int> listaNumero;
		vector<double> listaValorVenal, listaValorIptu;
		int k,readerInt;
		double valorTotalIptu = 0.0;

	public:

		SampleDecoder(string _fileName);
		~SampleDecoder();

		void cleanMemory();

		double getSeed();

		// double construirSolucao(std::vector< double > &chromosome,vector< unsigned > &permutation) const;
		double construirSolucao(std::vector< double > &chromosome) const;

		double decode(std::vector< double >& chromosome) const;

};

#endif
