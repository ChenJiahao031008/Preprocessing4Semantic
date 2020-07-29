#include <iostream>
#include "Preprocessing.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc!=2) cerr << "[ERRO] INPUT ERRO! " << endl;
	Preprocessing preprocessing(argv[1]);
	// 如果使用labelme，并且批量转换成了诸如0001_json、0002_json的文件夹
	// 该函数的作用是生成符合预处理标准的数据集，命名为image_2与semantic
	preprocessing.LabelmeTool();

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