#include "stdafx.h"
#include "RgbToYCbCr.h"

#include <vector>
#include <array>

using namespace std;

// Data hver pixel har 24 eller 3 * unit8_t i r�kkef�lgen RBG
// Input en lang r�kke af unit8_t
vector<char> RgbToYCbCr::convert(array<uint8_t, in_res_rgb>) {
	

	int Y;
	int Cb;
	int Cr;
	int R;
	int G;
	int B; 
	
	array<uint8_t,1> Blandet;


	// Vi skal have isoleret 3 * unit8_t som s� kan s�ttes ind i formlen nedenunder,
	// Dette kan g�res med en l�kke og lade denne k�rer alle unit8_t igennem
	// De skal s� s�ttes ind i tre nye data holder(vektor eller array)
	
	

	// This loop converts the input data(picture) to tre seperate pictures as chroma data.
	int arrayhold;
	while (true){
		if (convert[arrayhold])
  

		Y = (77 / 256)*R + (150 / 256)*G + (29 / 256)*B;
		Cb = -(44 / 256)*R - (87 / 256)*G + (131 / 256)*B + 128;
		Cr = (131 / 256)*R - (110 / 256)*G - (21 / 256)*B + 128;


	}
}
