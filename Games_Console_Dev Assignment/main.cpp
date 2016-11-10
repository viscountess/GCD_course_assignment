#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//#include "pbma_io.hpp"

using namespace std;

void imageParser(string fileName)
{
	cout << "Parsing the image files......." << endl;

	//Parsing the PGM data - am hoping this works.....
	int row = 0, col = 0, alpha = 0;
	int numrows = 0;
	int numcols = 0;
	ifstream inFile(fileName.c_str());
	stringstream ss;
	string inputLine = "";

	//First line:  Getting version
	getline(inFile, inputLine);
	if (inputLine.compare("P2") != 0) cerr << "PGM image version error." << endl;
	else cout << "Version: " << inputLine << endl;

	// Continue with a stringstream
	ss << inFile.rdbuf();
	// Second  line : size
	ss >> numcols >> numrows;
	cout << "Size: " << numcols << " columns and " << numrows << " rows" << endl;

	//Third line : alpha
	ss >> alpha;
	if (alpha != 255) cerr << "PGM image alpha error." << endl;
	else cout << "Alpha: " << alpha << endl;

	int **array = new int *[numrows];
	for (int i = 0; i < numrows; i++)
	{
		array[i] = new int[numcols];
	}

	// Following lines : data
	for (row = 0; row < numrows; ++row)
		for (col = 0; col < numcols; ++col) ss >> array[row][col];

	//// Now print the array to see the result - just for testing the parsing.
	//for (row = 0; row < numrows; ++row) {
	//	for (col = 0; col < numcols; ++col) {
	//		cout << array[row][col] << " ";
	//	}
	//	cout << endl;
	//}

	inFile.close();
}

int main(int argc, char *argv[])
{

	imageParser("image1.pgm");

	return 0;

}