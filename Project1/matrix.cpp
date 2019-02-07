//Jessica Hialrio
//Joel Rodriguez
//This program reads a matrix from a text file 
//and outputs a 3x3 matrix by combining the 2
//matrices given from the 2 inputs.

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sstream>

using namespace std;

#include "../Utilities/Utils.h"
#include "../Utilities/Scanner.h"
#include "../Utilities/ScanLine.h"

static const string TAG = "Main: ";

#define MAX 3 //For 3x3 matrices
#define NUM_THREADS 9 //For the amount of threads needed

int in1[3][3];
int in2[3][3];
int result[3][3];
struct info {
	int i;
	int j;
};

int main(int argc, char *argv[]) {
	string inFileName1 = "";
	string inFileName2 = "";
	ofstream outStream;

	Scanner scan1;
	Scanner scan2;

	string element = "";

	Utils::CheckArgs(2, argc, argv, "infilename (first matrix) outfilename (second matrix)");
	inFileName1 = static_cast<string>(argv[1]);
	inFileName2 = static_cast<string>(argv[2]);

	scan1.openFile(inFileName1);
	scan2.openFile(inFileName2);

	int i, j;

	pthread_t threads[NUM_THREADS]; //Declares 9 threads
	
	cout << "Matrix 1" << endl;
	for (i = 0; i < MAX; i++) { //Outputs first input file matrix
		for (j = 0; j < MAX; j++) {
			if (scan1.hasNext()) {
				//Converts the string into an int
				istringstream(scan1.next()) >> in1[i][j];
				cout << in1[i][j] << " ";
			}
		}
		cout << endl;
	}

	cout << "Matrix 2" << endl;
	for (i = 0; i < MAX; i++) { //Outputs second input file matrix
		for (j = 0; j < MAX; j++) {
			if (scan2.hasNext()) {
				//Converts the string into an int
				istringstream(scan2.next()) >> in2[i][j];
				cout << in2[i][j] << " ";
			}
		}
		cout << endl;
	}

	for (i = 0; i < NUM_THREADS; i++) { //Creates the 9 threads
		int *p;
		cout << "creating thread: " << i << endl;
		pthread_create(&threads[i], NULL, matrix_mult, (void*)(p))
	}

	for (i = 0; i < NUM_THREADS; i++) { //Joins and waits for all threads
		pthread_join(threads[i], NULL);
	}
	
	cout << endl;

	//Display the resulting matrix of multiplying the 2 input matrices
	cout << "Results of Matrix Multiplication" << endl;
	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++) {
			cout << result[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}
