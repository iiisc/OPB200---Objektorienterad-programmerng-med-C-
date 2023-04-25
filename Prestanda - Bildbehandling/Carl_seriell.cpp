#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

void setWesternEuropeanCodePage() {
	SetConsoleOutputCP(1252);
	SetConsoleCP(1252);
}

/* GLOBALA VARIABLAR */
int gWidth, gHeight, gPixelStart;

//
// Hj�lpfunktion f�r hexdump. Bra f�r fels�kning.
// Skriv ut en hexdump av alla bytes mellan [start, start+maxbytes-1]
//
void hexdump(const unsigned char *start, int maxbytes) {
	for (int r = 0; r < maxbytes / 16 + 1; r++) {
		cout << setw(4) << hex << setfill('0');
		cout << r * 16 << ": ";
		for (int i = 0; i < 16; i++) {
			int idx = i + r * 16;
			if (idx < maxbytes) {
				cout << setw(2) << hex << setfill('0');
				cout << (int)start[idx] << ' ';
			}
			else {
				cout << "   ";
			}
			if (i == 7)
				cout << ' ';
		}
		cout << "  ";
		for (int i = 0; i < 16; i++) {
			int idx = i + r * 16;
			if (idx < maxbytes) {
				cout << (start[idx] < 32 ? '.' : (char)start[idx]);
			}
		}
		cout << dec << endl;
	}
}

//
//Plockar ut information ur headern 
//	- H�jd, bredd, vart pixlarna b�rjar
//
void parseBMP(const unsigned char *rawimage) {
	gPixelStart = *(int*)(rawimage + 10);
	gWidth = *(int*)(rawimage + 18);
	gHeight = *(int*)(rawimage + 22);
}

void convert_greyscale(unsigned char *rawimage) {
	int tempPixel = gPixelStart;

	for (int row = 0; row < gHeight; row++) {
		for (int col = 0; col < gWidth; col++) {
			unsigned int Bl� = *(unsigned char*)(rawimage + tempPixel);
			unsigned int Gr�n = *(unsigned char*)(rawimage + tempPixel + 1);
			unsigned int R�d = *(unsigned char*)(rawimage + tempPixel + 2);
			int newval = (int)((0.2126 * R�d) + (0.7152 * Gr�n) + (0.0722 * Bl�));

			*(unsigned char*)(rawimage + tempPixel) = newval;
			*(unsigned char*)(rawimage + tempPixel + 1) = newval;
			*(unsigned char*)(rawimage + tempPixel + 2) = newval;

			tempPixel = tempPixel + 3;
		}	
	}
}

//
// H�lpfunktion till sobelfiltret.
// Returnera pixelv�rdet p� rad "row" (y) och kolumn "col" (x).
//
unsigned char get_pixel(const unsigned char *srcbuffer, int width, int row, int col) {
	unsigned int temp = *(unsigned char*)(srcbuffer + gPixelStart + (3 * col) + (3 * width * row));
	return temp;
}

//
// H�lpfunktion till sobelfiltret.
// Skriv nytt pixelv�rdet p� rad "row" (y) och kolumn "col" (x).
//
void set_pixel(unsigned char *dstbuffer, int width, int row, int col, unsigned char newval) {
	*(unsigned char*)(dstbuffer + gPixelStart + (3 * col) + (3 * width * row)) = newval;
	*(unsigned char*)(dstbuffer + gPixelStart + (3 * col) + (3 * width * row) + 1) = newval;
	*(unsigned char*)(dstbuffer + gPixelStart + (3 * col) + (3 * width * row) + 2) = newval;
}

//
// Omvandla src-bilden till en dst-bild med kanter markerade med ljusare gr�skala.
// Anv�nder ett Sobelfilter: https://en.wikipedia.org/wiki/Sobel_operator
//
void filter_sobel(int width, int height, const unsigned char *srcbuffer, unsigned char *destbuffer) {
	int sobelX[3][3] = {
		{ -1, 0, 1 },
		{ -2, 0, 2 },
		{ -1, 0, 1 } };
	int sobelY[3][3] = {
		{ -1, -2, -1 },
		{ 0,  0,  0 },
		{ 1,  2,  1 } };

	// Compute a new pixel for every row and column.
	// Skip edges since the convolution reads neighboring pixels.
	for (int r = 1; r < height - 1; r++)
		for (int c = 1; c < width - 1; c++) {
			// For each pixel, convolve kernel with neighbors
			// Kernels should maybe be flipped (in x) but this doesn't really matter for edge detection

			// Horizontal convolution
			int sumX = 0;
			for (int sx = 0; sx < 3; sx++)
				for (int sy = 0; sy < 3; sy++) {
					sumX += sobelX[sy][sx] * get_pixel(srcbuffer, width, r + sy - 1, c + sx - 1);
				}
			// Vertical convolution
			int sumY = 0;
			for (int sx = 0; sx < 3; sx++)
				for (int sy = 0; sy < 3; sy++) {
					sumY += sobelY[sy][sx] * get_pixel(srcbuffer, width, r + sy - 1, c + sx - 1);
				}
			// Compute combined sum and limit to 255
			int sum = (int)sqrt((double)sumX*sumX + (double)sumY*sumY);
			if (sum > 255)
				sum = 255;
			set_pixel(destbuffer, width, r, c, sum);
		}
}

int main(int argc, char **argv)
{
	setWesternEuropeanCodePage();
	
	if (argc < 3) {
		cerr << "Ange infil och utfil som tv� argument!" << endl;
		return 1;
	}
	cout << "L�ser fr�n " << argv[1] << endl;
	cout << "Skriver till: " << argv[2] << endl;
	
	// Exempel p� fill�sning och skrivning, man kan ha b�ttre felhantering!
	unsigned infil_size = (unsigned)filesystem::file_size(argv[1]);
	cout << "Storlek: " << infil_size << " bytes" << endl;
	unsigned char *rawimage = new unsigned char[infil_size];

	// L�s alla bytes. Notera "binary" h�r �r viktigt.
	ifstream infil(argv[1], fstream::binary);
	infil.read((char *)rawimage, infil_size);
	infil.close();

	// G�r en exakt kopia av rawimage till destimage
	unsigned char *destimage = new unsigned char[infil_size];
	memcpy(destimage, rawimage, infil_size);

	parseBMP(rawimage);
	
	cout << endl << "Hexdump av bildfilen (f�r debuggning):" << endl;
	hexdump(rawimage, 200);
	cout << endl;

	cout << "Konverterar till gr�skala och k�r sobelfiltret..." << endl;
	LARGE_INTEGER startcount, stopcount, frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startcount);

	convert_greyscale(rawimage);
	filter_sobel(gWidth, gHeight, rawimage, destimage);

	QueryPerformanceCounter(&stopcount);
	cout << "Klar!" << endl;
	cout << "Start: " << startcount.QuadPart << "  (freq = " << frequency.QuadPart << ")" << endl;
	cout << "Stopp: " << stopcount.QuadPart << endl;
	cout << "Diff: " << stopcount.QuadPart - startcount.QuadPart << endl;
	cout << "Tid: " << (double)(stopcount.QuadPart - startcount.QuadPart) / frequency.QuadPart * 1000 << " ms" << endl;

	// Skriv destimage som nu �r uppdaterad med nya pixelv�rden
	ofstream utfil(argv[2], fstream::binary);
	utfil.write((char *)destimage, infil_size);
	utfil.close();

	delete[] rawimage;
	delete[] destimage;
	return 0;
}