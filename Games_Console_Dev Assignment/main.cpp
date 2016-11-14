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

int *applySobelFilter(string fileName, int *inputBuffer, int imageWidth, int imageHeight)
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
			//No value checks for going above 255
			//int sum = abs(sum_x) + abs(sum_y);

			//Magnitude formula from phung & bouzerdoum paper
			int sum = sqrt((sum_x*sum_x) + (sum_y*sum_y));

			//Clamping the sobel - so it doesnt go out of bounds
			if (sum < 0)
				sum = 0;
			if (sum > 255)
				sum = 255;

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

	return out;
}

//Aiming to work out the sum edge density - I am assuming this is working
int getEdgeDensitySumForRegion(int *imageBuffer, int xpos, int ypos, int width, int height, int imageWidth)
{
	int magnitudeSum = 0;
	for (int x = xpos; x < xpos + width; x++)
	{
		for (int y = ypos; y < ypos + height; y++)
		{
			int index = (y * imageWidth) + x;
			magnitudeSum += imageBuffer[index];
		}
	}

	return magnitudeSum;
}


int *calculateEdgeIntegral(int *imageBuffer, int imageWidth, int imageHeight)
{
	int *outputBuffer = new int[imageHeight * imageWidth];
	

	for (int i = 0; i < imageHeight * imageWidth; i++)
	{
		int y = i / imageWidth;
		int x = i - (y * imageWidth);
		outputBuffer[i] = getEdgeDensitySumForRegion(imageBuffer, 0, 0, x, y, imageWidth);
	}

	return outputBuffer;
}

//Using the formula:
// f = 1/Ar {{ S(x2, y2) + S(x1 ? 1, y1 ? 1) ? S(x2, y1 ? 1) ? S(x1 ? 1, y2) }
float getEdgeDensityFeatureForRegion(int *integralBuffer, int xpos, int ypos, int width, int height, int imageWidth)
{
	int endXpos = xpos + width;
	int endYpos = ypos + height;
	int endIndex = (endYpos * imageWidth) + endXpos;
	int index2 = ((ypos - 1) * imageWidth) + (xpos - 1);
	int index3 = ((ypos - 1) * imageWidth) + endXpos;
	int index4 = (endYpos * imageWidth) + (xpos - 1);
	float area = width * height;
	float density = (1.f / area) * (integralBuffer[endIndex] + integralBuffer[index2] - integralBuffer[index3] - integralBuffer[index4]);

	return density;
}

int main(int argc, char *argv[])
{
	int imageWidth, imageHeight;
	int *imageBuffer = imageParser("image1.pgm", imageWidth, imageHeight);
	int *sobelImage = applySobelFilter("sobel_applied.pgm", imageBuffer, imageWidth, imageHeight);
	int *integralImage = calculateEdgeIntegral(sobelImage, imageWidth, imageHeight);

	getEdgeDensityFeatureForRegion(integralImage, 1, 1, 200, 200, imageWidth);
	

	return 0;

}