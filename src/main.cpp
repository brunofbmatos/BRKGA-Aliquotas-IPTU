/*
 * BRKGA_SNW.cpp
 *
 *  Created on: 20 de jan de 2022
 *      Author: guest-2FMpQr
 */


#include <iostream>
#include <vector>
#include <new>//bad alloc do comando new
#include <float.h>
#include <string>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <float.h>
#include "SampleDecoder.h"
#include "MTRand.h"
#include "BRKGA.h"
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]) {

    const unsigned p = 200;     // size of population //*** Antes era 200 o valor desse parâmetro
    const double pe = 0.20;     // fraction of population to be the elite-set
    const double pm = 0.10;     // fraction of population to be replaced by mutants
    const double rhoe = 0.70;   // probability that offspring inherit an allele from elite parent
    const unsigned K = 1;       // number of independent populations         
    const unsigned MAXT = 1;    // number of threads for parallel decoding 

    const unsigned X_INTVL = 100;   // exchange best individuals at every 100 generations
    const unsigned X_NUMBER = 2;    // exchange top 2 best
    const unsigned MAX_GENS = 1000; // run for 1000 gens   //***Antes era 1000

    // const long unsigned rngSeed = 0;    // seed to the random number generator
    // MTRand rng(rngSeed);                // initialize the random number generator

    ifstream filaNomeArquivos;
    ofstream dataFileWrite;
	
    string Buffer, fileName;
    clock_t TempoFO_StarInic;
    double TempoExecTotal= 0.0, TempoFO_Star= 0.0, TempoMinFO= 0.0, FO_Star= DBL_MAX, FO_Min= DBL_MIN;
    int bestGeneration=0, minGeneration= 0;
    stringstream converterIntToString;

	vector<double> maxCromossomo, minCromossomo;

    int iterSemMelhora = 0, iterMax= 0;
	double ValorTotalIPTU = 0.0;

	// Utility utilities;



	    //====== Inicialização do Decoder e do Gerador de Números Aleatórios
	    // SampleDecoder decoder(fileName.c_str()); // initialize the decoder
		SampleDecoder decoder("APTOS.csv");

	    //MTRand rng(time(NULL)); // initialize the random number generator Alterado para time(NULL) o valor da semente
	    MTRand rng( decoder.getSeed() ); // initialize the random number generator Alterado para time(NULL) o valor da semente

	    const unsigned n = 3;     // size of chromosomes


	    // initialize the BRKGA-based heuristic
	    BRKGA< SampleDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

	    TempoFO_StarInic = clock();


		FO_Star = DBL_MAX;
		FO_Min= DBL_MAX*-1;
		bestGeneration = 0;
		minGeneration= 0;

		//Aqui!!! 
		        iterSemMelhora= 0;
				iterMax= 1;

				unsigned generation = 0;        // current generation
				do {
					   algorithm.evolve(); // evolve the population for one generation


						if ((++generation) % X_INTVL == 0) {
							algorithm.exchangeElite(X_NUMBER); // exchange top individuals
						}

						//***std::cout << "Best solution found has objective value in generation "<<generation<<"= "
						//***<< algorithm.getBestFitness() << std::endl;

						//Os sinais < e > são invertidos pois o valor do fitness é multiplicado por -1
						if (algorithm.getBestFitness() < FO_Star) {
							TempoFO_Star = (((double) (clock() - TempoFO_StarInic))/ CLOCKS_PER_SEC);
							FO_Star = algorithm.getBestFitness();
							bestGeneration = generation;
							//maxCromossomo= algorithm.getBestChromosome();

							iterSemMelhora= 0;
						}
					 	else{
						 	 iterSemMelhora++;
						 	}					

				} while (iterSemMelhora<iterMax);


				TempoExecTotal = (((double) (clock() - TempoFO_StarInic))/ CLOCKS_PER_SEC);



			//cout<<"FO_Star: "<<std::setprecision(21)<<FO_Star<<endl;
			//cout<<"Valor total IPTU: "<<std::setprecision(21)<<FO_Star<<endl;
			//cout<<"Tempo de execução: "<<TempoExecTotal<<endl;
// valor total iptu
//tempo de execução 
			decoder.cleanMemory();


    return 0;
}


