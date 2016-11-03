#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//#include "pbma_io.hpp"

using namespace std;

int main(int argc, char *argv[])
{

	cout << "Parsing the image files......." << endl;

	//Parsing the PGM data - am hoping this works.....
	int row = 0, col = 0, numrows = 0, numcols = 0;
	ifstream inFile("image1.pgm");
	stringstream ss;
	string inputLine = "";

	//First line:  Getting version
	getline(inFile, inputLine);
	if (inputLine.compare("P2") != 0) cerr << "PGM image version error." << endl;
	else cout << "Version: " << inputLine << endl;

	// Second line : comment
	getline(inFile, inputLine);
	cout << "Comment : " << inputLine << endl;

	// Continue with a stringstream
	ss << inFile.rdbuf();
	// Third line : size
	ss >> numcols >> numrows;
	cout << numcols << " columns and " << numrows << " rows" << endl;

	//int array[numrows][numcols]; //for some reason this isn't working - note to self need to fix

	//// Following lines : data
	//for (row = 0; row < numrows; ++row)
	//	for (col = 0; col < numcols; ++col) ss >> array[row][col];

	//// Now print the array to see the result
	//for (row = 0; row < numrows; ++row) {
	//	for (col = 0; col < numcols; ++col) {
	//		cout << array[row][col] << " ";
	//	}
	//	cout << endl;
	//}
	inFile.close();

	

}