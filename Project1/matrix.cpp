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

//A struct that holds the row and column
struct info {
	int row;
	int col;
};

//Multiplies and adds the matrix by using the row and column from the struct
void *matrix_mult(void *args) {
	struct info *boi = (struct info*) args;
	
	//Multiplies and adds the row and column from the 2 matrices
	for (int i = 0; i < MAX; i++) {
		result[boi -> row][boi -> col] += in1[boi -> row][i] * in2[i][boi -> col];
	}

	//Return NULL
	return NULL;
}

//Takes 2 input files of 3x3 matrices and produce a matrix by multiplying the threads
int main(int argc, char *argv[]) {
	int i, j;
	
	//Initialize the result matrix to 0
	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++) {
			result[i][j] = 0;
		}
	}

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


	pthread_t threads[NUM_THREADS]; //Declares 9 threads

	struct info doggo[MAX][MAX];

	for (i = 0; i < MAX; i++) { //Create a 3D array of the struct info and store the indices
		for (j = 0; j < MAX; j++) {
			doggo[i][j].row = i;
			doggo[i][j].col = j;
		}
	}
	
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

	for (i = 0; i < MAX; i++) { //Creates the 9 threads
		for (j = 0; j < MAX; j++) {
			cout << "creating thread: " << i << endl;
			pthread_create(&threads[j + (MAX * i)], NULL, &matrix_mult, (void*) &doggo[i][j]);
			//Index 1 dimensional array from a 2 dimensional array
		}
	}

	for (i = 0; i < NUM_THREADS; i++) { //Joins all threads while the main thread waits
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
