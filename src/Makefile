#Para escrever comentários ##
############################# Makefile ##########################
all: BRKGA
BRKGA: main.o SampleDecoder.o Utility.o
	g++ -o BRKGA main.o SampleDecoder.o Utility.o
main.o: main.cpp SampleDecoder.h MTRand.h BRKGA.h Utility.h
	g++ -c main.cpp

SampleDecoder.o: SampleDecoder.cpp SampleDecoder.h Structures.h Utility.h
	g++ -c SampleDecoder.cpp

Utility.o: Utility.cpp Structures.h
	g++ -c Utility.cpp

clean:
	rm -rf *.o
mrproper: clean
	rm -rf teste
