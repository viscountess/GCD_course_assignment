#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//#include "pbma_io.hpp"

using namespace std;

int main(int argc, char *argv[])
{

	//Parsing the PGM data - am hoping this works.....
	int row = 0, col = 0, numrows = 0, numcols = 0;
	ifstream infile("image1.pgm");
	stringstream ss;
	string inputLine = "";

	//First line:  Getting version

}