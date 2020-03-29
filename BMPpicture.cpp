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
			
			if(picSettings.biCompression){			//Decompression is necessary
				std::cout << "Decompression necessary" << std::endl;
			}
			
			if(picSettings.biClrUsed){				//Color table is necessary
				getColorIndex();
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

void BMPpicture::getColorIndex(){
	/*color table is array which contain indexed colors of pixel.
	Each pixel has his own index which point here.*/
	short pixelFormat = ((picIntro.bfOffBits - 54) / picSettings.biClrUsed);	//Result should be 4(BRGA) or 3(BRG) depend on pixel representation //54 is pos after headers
	pic.seekg(54, std::ios_base::beg);											//color table begining
	for(;pic.tellg() != picIntro.bfOffBits;){									//save positions of each index in vector 
		colorTable.push_back(pic.tellg());
		pic.seekg(pixelFormat, std::ios_base::cur);
	}
}

bool BMPpicture::makeFile(string _operation){
	bool result;
	string resultFilePath = filePath;									//Prepare file path for output file
	resultFilePath = resultFilePath.insert(resultFilePath.find(".bmp"), "_" + _operation);
	
	operationResult.close();
	operationResult.open(resultFilePath, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	
	if(operationResult.is_open()){
		if(!operationResult.fail()){
			//FILE HEADER INFO
			operationResult.seekp(0, std::ios_base::beg);					//File header begining
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
			
			result = 1;
		}
		else{
			std::cout << "Fail appear." << std::endl; 
			result = 0;
		}
	}
	else{
		std::cout << "File is not opened." << std::endl; 
		result = 0;
	}
	
	return result;
}

std::ofstream& BMPpicture::negative(){
	if(!makeFile("negative")){
		std::cout << "Negative operation unsucessfull.\n" << std::endl;
		return operationResult;
	}
	
	if(picSettings.biClrUsed) pic.seekg(54, std::ios_base::beg);			//for color table inversion
	else pic.seekg(picIntro.bfOffBits, std::ios_base::beg);					//for pixels direct inversion
	
	auto pToPix = new pixelBGR;												//auxiliary struct
				
	for(int y = 0; y <= picSettings.biHeight; ++y){							//Pixels net (height)
		for(int x = 0; x <= picSettings.biWidth; ++x){						//Pixels net (width)
			if(picSettings.biClrUsed && pic.tellg() == 54){					//color table inversion (if exist - only in first entrance to loop when there is color table and reading pointer is set at 54pos)
				for(;pic.tellg() < picIntro.bfOffBits;){					
					pic.read((char*)pToPix, 4);	
					pToPix->Blue ^= 0xFF;
					pToPix->Green ^= 0xFF;
					pToPix->Red ^= 0xFF;
					operationResult.write((char*)pToPix, 4);
				}
			}
			
			if(picSettings.biBitCount != 24){								//For all cases except 24bpp we ned to just copy-paste pixels (color table is already inverted)
				char tempB;
				pic.read((char*)&tempB, 1);
				operationResult.write((char*)&tempB, 1);
			}
			else{															//For 24bpp direct change at orginal pixel is necessary
				pic.read((char*)pToPix, 3);			
				pToPix->Blue ^= 0xFF;
				pToPix->Green ^= 0xFF;
				pToPix->Red ^= 0xFF;
				operationResult.write((char*)pToPix, 3);
			}	
		}
	}
	
	delete pToPix;	
	std::cout << "Negative operation done.\n" << std::endl;
	return operationResult;
}
