#include <iostream>
#include "Preprocessing.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc!=2) cerr << "[ERRO] INPUT ERRO! " << endl;
	Preprocessing preprocessing(argv[1]);

	if(preprocessing.LoadImg()==-1) cout << "[ERRO] NO IMAGE!" <<endl;
	
	preprocessing.SimpleDeformation(0.3);
	preprocessing.Resize(0.2,0.5,1.5);
	preprocessing.Tailor(0.2);
	preprocessing.IlluminationAndContrast(0.2);
	preprocessing.Saturation(0.2);
	preprocessing.Mosaic(0.4);
	preprocessing.RandomErasure(0.2);

	return 0;
}