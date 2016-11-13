#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int *imageParser(string fileName, int &imageWidth, int &imageHeight)
{

	cout << "Parsing the image files......." << endl;

	//Parsing the PGM data - am hoping this works.....
	int row = 0, col = 0, alpha = 0;
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
	ss >> imageWidth >> imageHeight;
	cout << "Size: " << imageWidth << " columns and " << imageHeight << " rows" << endl;

	//Third line : alpha
	ss >> alpha;
	if (alpha != 255) cerr << "PGM image alpha error." << endl;
	else cout << "Alpha: " << alpha << endl;

	int *array = new int [imageHeight * imageWidth];
	/*for (int i = 0; i < imageHeight; i++)
	{
		array[i] = new int[imageWidth];
	}*/

	// Following lines : data
	/*for (row = 0; row < imageHeight; ++row)
		for (col = 0; col < imageWidth; ++col) ss >> array[row][col];
*/
	for (int i = 0; i < imageHeight * imageWidth; i++)
	{
		ss >> array[i];
	}
	//// Now print the array to see the result - just for testing the parsing.
	//for (row = 0; row < numrows; ++row) {
	//for (col = 0; col < numcols; ++col) {
	//cout << array[row][col] << " ";
	//}
	//cout << endl;
	//}

	inFile.close();

	return array;

}

void applySobelFilter(string fileName, int *inputBuffer, int imageWidth, int imageHeight)
{
	int row = 0, col = 0;

	//Output array
	int *out = new int [imageHeight * imageWidth];
	for (int i = 0; i < imageWidth * imageHeight; i++)
	{
		out[i] = 0;
	}
	/*for (int i = 0; i < imageHeight; i++)
	{
		out[i] = new int[imageWidth];
		for (int p = 0; p < imageWidth; p++) {
			out[i][p] = 0;

		}
	}*/

	//cout << "Out: " << array[0][0] << ' ' << endl;

	//SOBEL IMPLEMENTATION

	//int sobel_x[3][3] = { {-1,0,1} , {-2,0,2} , { -1,0,1 } };
	//int sobel_y[3][3] = { {-1,-2,-1} , {0,0,0} , {1,2,1} };

	//The convolution required to 
	int sobel_x[3][3] = { { 1,0,-1 },{ 2,0,-2 },{ 1,0,-1 } };
	int sobel_y[3][3] = { { 1,2,1 },{ 0,0,0 },{ -1,-2,-1 } };

	int pixel_value;

	for (int y = 1; y < imageHeight - 1; y++) {
		for (int x = 1; x < imageWidth - 1; x++) {

			int sum_x = 0;
			int sum_y = 0;

			for (int m = -1; m <= 1; m++) {
				for (int n = -1; n <= 1; n++) {
					int tmp_x = x + m;
					int tmp_y = y + n;
					int index = (tmp_y * imageWidth) + tmp_x;
					pixel_value = inputBuffer[index];
					//cout << "Pixel: " << pixel_value << endl;
					sum_x += pixel_value * sobel_x[m + 1][n + 1];
					sum_y += pixel_value * sobel_y[m + 1][n + 1];

				}
			}
			int sum = abs(sum_x) + abs(sum_y);
			//cout << "Sum: " << sum << endl;

			int index = (y * imageWidth) + x;
			out[index] = sum;
			//cout << "Out: " << out[x][y] << "Position: X: "<< x << " Y:"<< y <<  endl;


		}
	}

	//Outputting the pgm image file
	ofstream fs(fileName);
	fs << "P2" << endl;
	fs << imageWidth << " " << imageHeight << endl;
	fs << "255" << endl;
	for (int i = 0; i < imageWidth * imageHeight; i++) {
			int aa = out[i];
			//cout << aa << endl;
			fs << aa;
			fs << " ";
		}
	fs.close();
}

int main(int argc, char *argv[])
{
	int imageWidth, imageHeight;
	int *imageBuffer = imageParser("image1.pgm", imageWidth, imageHeight);
	applySobelFilter("sobel_applied.pgm", imageBuffer, imageWidth, imageHeight);


	return 0;

}