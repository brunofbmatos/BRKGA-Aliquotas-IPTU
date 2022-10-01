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

    const unsigned p = 300;     // size of population //*** Antes era 200 o valor desse parâmetro
    const double pe = 0.20;     // fraction of population to be the elite-set
    const double pm = 0.10;     // fraction of population to be replaced by mutants
    const double rhoe = 0.70;   // probability that offspring inherit an allele from elite parent
    const unsigned K = 1;       // number of independent populations         
    const unsigned MAXT = 2;    // number of threads for parallel decoding 

    const unsigned X_INTVL = 100;   // exchange best individuals at every 100 generations
    const unsigned X_NUMBER = 2;    // exchange top 2 best
    const unsigned MAX_GENS = 1000; // run for 1000 gens   //***Antes era 1000 -> 200 -> 500 -> 1000

    // const long unsigned rngSeed = 0;    // seed to the random number generator
    // MTRand rng(rngSeed);                // initialize the random number generator

    ifstream filaNomeArquivos;
    ofstream dataFileWrite, dataFileWrite2;

    string Buffer, fileName, ArquivoMelhorGeracao, ArquivoMelhorSolucao;
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
		SampleDecoder decoder("instancias/aptos-contagem.csv");
		// SampleDecoder decoder("instancias/aptos-salvador.csv");
		ArquivoMelhorGeracao = "resultados/teste-geracoes-";
		ArquivoMelhorSolucao = "resultados/teste-solucoes";

	    //MTRand rng(time(NULL)); // initialize the random number generator Alterado para time(NULL) o valor da semente
	    MTRand rng( decoder.getSeed() ); // initialize the random number generator Alterado para time(NULL) o valor da semente

	    const unsigned n = 3;     // size of chromosomes


	    // initialize the BRKGA-based heuristic
	    BRKGA< SampleDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

	    
		minGeneration= 0;
		iterSemMelhora= 0;
		iterMax= 200;

		int quantTestes = 30;
		string nomeArquivo, nomeArquivo2;
		string execucao = "";


		nomeArquivo2 = ArquivoMelhorSolucao+".txt";

		dataFileWrite2.open(nomeArquivo2.c_str());
		if(!dataFileWrite2.is_open()) {
			cout<<"Não foi possivel abrir o arquivo dataFileWrite2!: "<<nomeArquivo2<<endl;
			dataFileWrite2.clear();
			exit(1);
		}

		for(int i= 0; i< quantTestes; i++) {

			cout<<"Execucao "<<i+1<<endl;

			execucao = std::to_string( (i+1) );
			
			nomeArquivo = ArquivoMelhorGeracao+execucao+".txt";

			dataFileWrite.open(nomeArquivo.c_str());
			if(!dataFileWrite.is_open()) {
				cout<<"Não foi possivel abrir o arquivo dataFileWrite!: "<<nomeArquivo<<endl;
				dataFileWrite.clear();
				exit(1);
			}





				unsigned generation = 0; // current generation

				TempoFO_StarInic = clock();

					FO_Star = DBL_MAX;
					FO_Min= DBL_MAX*-1;
					bestGeneration = 0;
				
				do {
					   algorithm.evolve(); // evolve the population for one generation


						if ((++generation) % X_INTVL == 0) {
							algorithm.exchangeElite(X_NUMBER); // exchange top individuals
						}

						//***std::cout << "Best solution found has objective value in generation "<<generation<<"= "
						//***<< algorithm.getBestFitness() << std::endl;

						double tempoGeracao = (((double) (clock() - TempoFO_StarInic))/ CLOCKS_PER_SEC);
						//cout<<"BestFitness "<<std::setprecision(21)<<algorithm.getBestFitness()<<endl;
						//Os sinais < e > são invertidos pois o valor do fitness é multiplicado por -1
						if (algorithm.getBestFitness() < FO_Star) {
							TempoFO_Star = (((double) (clock() - TempoFO_StarInic))/ CLOCKS_PER_SEC);
							FO_Star = algorithm.getBestFitness();
							bestGeneration = generation;
							maxCromossomo = algorithm.getBestChromosome();
							iterSemMelhora= 0;

							// cout<<"BestFitness "<<std::setprecision(21)<<algorithm.getBestFitness()*-1.0<<endl;
							// cout<<"bestGeneration "<<bestGeneration<<endl;

						} else {
						 	 iterSemMelhora++;
						}					

					dataFileWrite<<std::setprecision(21)<<algorithm.getBestFitness()*-1.0<<"  "<<generation<<"  "<<tempoGeracao<<"\n";

				//} while (iterSemMelhora<iterMax);

				} while (generation < MAX_GENS);


				TempoExecTotal = (((double) (clock() - TempoFO_StarInic))/ CLOCKS_PER_SEC);
				dataFileWrite2<<i+1<<" "<<std::setprecision(21)<<FO_Star*-1.0<<"  "<<TempoExecTotal<<"  "<<bestGeneration<<"\n";

				cout<<i+1<<" "<<std::setprecision(21)<<FO_Star*-1.0<<"  "<<TempoExecTotal<<"  "<<bestGeneration<<"\n";

				cout<<"\n\n\n";


				decoder.verificarSolucao(maxCromossomo, i);

				// cout<<"FO_Star: "<<std::setprecision(21)<<FO_Star<<endl;
				// cout<<"Valor total IPTU: "<<std::setprecision(21)<<FO_Star<<endl;
				// cout<<"Tempo de execução: "<<TempoExecTotal<<endl;
				// valor total iptu
				// tempo de execução 

				decoder.cleanMemory();


				dataFileWrite.clear();
				dataFileWrite.close();
		}

		dataFileWrite2.clear();
		dataFileWrite2.close();


    return 0;
}


// arquivo MelhorGeracao loop algorithm.getBestFitness() generation  tempoGeracao  referente ao melhor chromossomo de cada geração

//  arquivo MelhorSolucao  FO_Start TempoExecTotal BestGeneration

//dataFileWrite<<"  "<<k<<")   "<<FO_Star/10.0<<"  "<<(algorithm.getBestFitness()*(-1))/10.0<<"  "<<bestGeneration<<"  "<<TempoFO_Star<<"  "<<(FO_Min*-1)/10.0<<"  "<<minGeneration<<"  "<<generation<<"  "<<TempoExecTotal<<"\n";