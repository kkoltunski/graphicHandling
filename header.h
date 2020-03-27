#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Graphic.h"
#include "BMPpicture.h"
#include "PNGpicture.h"
#include "Chunk.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

template<typename T>
T bigToLittle(T _valToSwap){					//Endians handshake
	T result {0x0000};
	unsigned short ct = (sizeof(T));
	
	if(ct){									
		for(int i = 0; i < ct; ++i){
			result <<= 8;
			result += (_valToSwap & 0xFF);
			_valToSwap >>= 8;	
		}	
	}
	else result = _valToSwap;
	
	return result;
}

//Operator used to show file header info of BMP pic
std::ostream& operator<<(std::ostream &out, fileHeader &fileHeader){
	out //<< "\nFile path : " << var.filePath
		<< "\nfileHeader.bfType = " << fileHeader.bfType
		<< "\nfileHeader.bfSize = " << fileHeader.bfSize
		<< "\nfileHeader.bfReserved1 = " << fileHeader.bfReserved1
		<< "\nfileHeader.bfReserved2 = " << fileHeader.bfReserved2
		<< "\nfileHeader.bfOffBits = " << fileHeader.bfOffBits << endl;
	
	return out;
}

//Operator used to show image header info of BMP pic
std::ostream& operator<<(std::ostream &out, imgHeader &imgHeader){
	out //<< "\nFile path : " << var.filePath
		<< "\nimgHeader.biSize = " << imgHeader.biSize
		<< "\nimgHeader.biWidth = " << imgHeader.biWidth
		<< "\nimgHeader.biHeight = " << imgHeader.biHeight
		<< "\nimgHeader.biPlanes = " << imgHeader.biPlanes
		<< "\nimgHeader.biBitCount = " << imgHeader.biBitCount
		<< "\nimgHeader.biCompression = " << imgHeader.biCompression
		<< "\nimgHeader.biSizeImage = " << imgHeader.biSizeImage
		<< "\nimgHeader.biXPelsPerMeter = " << imgHeader.biXPelsPerMeter
		<< "\nimgHeader.biYPelsPerMeter = " << imgHeader.biYPelsPerMeter
		<< "\nimgHeader.biClrUsed = " << imgHeader.biClrUsed
		<< "\nimgHeader.biClrImportant = " << imgHeader.biClrImportant << endl;
	
	return out;
}

//Operator used to show IHDR chunk of PNG image
std::ostream& operator<<(std::ostream &out, IHDR &var){
	out //<< "\nFile path : " << var.filePath
		<< "\nIHDR.width = " << var.width
		<< "\nIHDR.height = " << var.height
		<< "\nIHDR.bitDepth = " << static_cast<int>(var.bitDepth)
		<< "\nIHDR.colorType = " << static_cast<int>(var.colorType)
		<< "\nIHDR.compressionMeth = " << static_cast<int>(var.compressionMeth)
		<< "\nIHDR.filterMeth = " << static_cast<int>(var.filterMeth)
		<< "\nIHDR.intelaceMeth = " << static_cast<int>(var.intelaceMeth) << endl;
	
	return out;
}
