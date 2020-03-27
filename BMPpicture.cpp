#include "BMPpicture.h"

template<typename T>
T bigToLittle(T _valToSwap){						//Endians handshake
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

BMPpicture::BMPpicture(string _path) : Graphic{_path}{
	signature = 19778;								//66 77 ASCII - There is many possible signatures but only "BM" is supported by Windows - rest are OS/2 (IBM)
	checkFile();
}

BMPpicture::~BMPpicture(){
	
}

bool BMPpicture::checkFile(){
	if(pic.is_open()){
		if(validateSignature(2)){
			getSettings();
			
			if(picSettings.biCompression != 0){		//Decompression is necessary
				std::cout << "Decompression necessary" << std::endl;
			}
			
			if(picSettings.biBitCount != 24){		//Color table is necessary
				std::cout << "Color table necessary" << std::endl;
			}
			
			std::cout << "BMP PICTURE SETTINGS";
			std::cout << picIntro;
			std::cout << picSettings << std::endl;
		}
		else{
			std::cout << "signature nok" << std::endl;
		}
	}
		else{
		std::cout << "Wrong file path" << std::endl;
		pic.clear();
		filePath = "";	
	}
}

void BMPpicture::getSettings(){
//FILE HEADER INFO
	pic.seekg(0, std::ios_base::beg);					//File header begining
	pic.read((char*)&picIntro.bfType, 2);
	pic.read((char*)&picIntro.bfSize, 4);
	pic.read((char*)&picIntro.bfReserved1, 2);
	pic.read((char*)&picIntro.bfReserved2, 2);
	pic.read((char*)&picIntro.bfOffBits, 4);

//IMAGE HEADER INFO
	pic.read((char*)&picSettings.biSize, 4);
	pic.read((char*)&picSettings.biWidth, 4);
	pic.read((char*)&picSettings.biHeight, 4);
	pic.read((char*)&picSettings.biPlanes, 2);
	pic.read((char*)&picSettings.biBitCount, 2);
	pic.read((char*)&picSettings.biCompression, 4);
	pic.read((char*)&picSettings.biSizeImage, 4);
	pic.read((char*)&picSettings.biXPelsPerMeter, 4);
	pic.read((char*)&picSettings.biYPelsPerMeter, 4);
	pic.read((char*)&picSettings.biClrUsed, 4);
	pic.read((char*)&picSettings.biClrImportant, 4);
}

void BMPpicture::makeHeader(){
//FILE HEADER INFO
	operationResult.write((char*)&picIntro.bfType, 2);
	operationResult.write((char*)&picIntro.bfSize, 4);
	operationResult.write((char*)&picIntro.bfReserved1, 2);
	operationResult.write((char*)&picIntro.bfReserved2, 2);
	operationResult.write((char*)&picIntro.bfOffBits, 4);

//IMAGE HEADER INFO
	operationResult.write((char*)&picSettings.biSize, 4);
	operationResult.write((char*)&picSettings.biWidth, 4);
	operationResult.write((char*)&picSettings.biHeight, 4);
	operationResult.write((char*)&picSettings.biPlanes, 2);
	operationResult.write((char*)&picSettings.biBitCount, 2);
	operationResult.write((char*)&picSettings.biCompression, 4);
	operationResult.write((char*)&picSettings.biSizeImage, 4);
	operationResult.write((char*)&picSettings.biXPelsPerMeter, 4);
	operationResult.write((char*)&picSettings.biYPelsPerMeter, 4);
	operationResult.write((char*)&picSettings.biClrUsed, 4);
	operationResult.write((char*)&picSettings.biClrImportant, 4);
}

std::ofstream& BMPpicture::negative(){
	string resultFilePath = filePath;					//Prepare file path for output file
	resultFilePath = resultFilePath.insert(resultFilePath.find(".bmp"), "_negative");
	
	operationResult.close();
	operationResult.open(resultFilePath, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	
	if(operationResult.is_open()){
		if(!operationResult.fail())	makeHeader();
		else std::cout << "Fail appear." << std::endl;
	}
	else std::cout << "File is not opened." << std::endl;	
	
	pic.seekg(picIntro.bfOffBits, std::ios_base::beg);	
	
	for(int y = 0; y <= picSettings.biHeight; ++y){
		for(int x = 0; x <= picSettings.biWidth; ++x){
			auto pToPix = new pixel;
			pic.read((char*)pToPix, 3);
					
			pToPix->Blue ^= 0xFF;
			pToPix->Green ^= 0xFF;
			pToPix->Red ^= 0xFF;
			
			operationResult.write((char*)pToPix, 3);
			delete pToPix;
		}
	}
		
	std::cout << "Negative operation done.\n" << std::endl;
	return operationResult;
}
