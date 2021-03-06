/*****************************************************************//**
 * \file   main.cpp
 * \brief  Software principle realization.
 * 
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

#include "header.h"

int main(int argc, char *argv []){	
/*	string path {argv[1]}, fileName {};
	
	cout << "Directory : " << path << endl;
	cout << "Type .bmp file name (extension format will be added) located in above directory : ";
	std::getline(cin, fileName);
	
	string::size_type posInFile = fileName.rfind(".bmp");
	
	if((posInFile != (fileName.size() - 4)) || (posInFile == string::npos)) 
		fileName += ".bmp";
		
	cout << endl << fileName << endl;
	Graphic *pToBMP = new BMPpicture(path + fileName);
	pToBMP->options();*/
	
	string path = R"(C:\Users\kkoltunski\Desktop\GRAPHIC\)";
//////////////////////////////BMP///////////////////////////////////
	Graphic *pToTiger = new BMPpicture(path + "00_tiger" + ".bmp");
	pToTiger->picturePossibleOperations();

	Graphic *pToMarbles = new BMPpicture(path + "00_marbles" + ".bmp");
	pToMarbles->picturePossibleOperations();
	
	Graphic *pToGrayLena = new BMPpicture(path + "00_graylena" + ".bmp");
	pToGrayLena->picturePossibleOperations();
	
	Graphic *pToLand = new BMPpicture(path + "00_land" + ".bmp");
	//pToLand->negative();
	pToLand->picturePossibleOperations();
	
//////////////////////////////PNG///////////////////////////////////	
	Graphic *pToTriangleDice = new PNGpicture(path + "01_triangleDice" + ".png");
	PNGpicture *ptoTD = dynamic_cast<PNGpicture*>(pToTriangleDice);
	ptoTD->showChunks();
	ptoTD->showChunkData(5);
	
/*	Graphic *pTo3x3 = new PNGpicture(path + "01_3x3" + ".png");
	PNGpicture *pto33 = dynamic_cast<PNGpicture*>(pTo3x3);
	pto33->showChunks();
	pto33->showChunkData(1);*/
	
/*	Graphic *pToGreyscale = new PNGpicture(path + "01_greyscale" + ".png");
	PNGpicture *pToGs = dynamic_cast<PNGpicture*>(pToGreyscale);
	pToGs->showChunks();
	pToGs->showChunkData(3);*/
	
/*	Graphic *pToLena = new PNGpicture(path + "01_lena" + ".png");
	PNGpicture *pToLenaPNG = dynamic_cast<PNGpicture*>(pToLena);
	pToLenaPNG->showChunks();
	pToLenaPNG->showChunkData(4);*/
	
	
/*	Graphic *pToLenaGreyScale = new PNGpicture(path + "01_lena_greyscale" + ".png");
	PNGpicture *pToGLena = dynamic_cast<PNGpicture*>(pToLenaGreyScale);
	pToGLena->showChunks();
	pToGLena->showChunkData(4);*/

	return 0;
}
