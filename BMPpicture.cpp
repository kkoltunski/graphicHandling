#include "BMPpicture.h"

BMPpicture::BMPpicture(string _path) : Graphic{_path, ".bmp"}{
	signature = 19778;								//66 77 ASCII - There is many possible signatures but only "BM" is supported by Windows - rest are OS/2 (IBM)

	if (checkFileExtension()) {
		if (validateSignature(2)) {
			getSettings();
			calculateDependencies();

			if (picSettings.biCompression) {			//Decompression is necessary
				std::cout << "Decompression necessary" << std::endl;
			}

			if (picSettings.biClrUsed) getColorIndex();	//Color table is necessary

			std::cout << "BMP PICTURE SETTINGS";
			std::cout << picHeader;
			std::cout << picSettings << std::endl;
		}
	}
}

BMPpicture::~BMPpicture() noexcept(true) {
	
}

void BMPpicture::getSettings() noexcept(true) {
//FILE HEADER INFO
	pic.seekg(0, std::ios_base::beg);					//File header begining
	pic.read((char*)&picHeader.bfType, 2);
	pic.read((char*)&picHeader.bfSize, 4);
	pic.read((char*)&picHeader.bfReserved1, 2);
	pic.read((char*)&picHeader.bfReserved2, 2);
	pic.read((char*)&picHeader.bfOffBits, 4);

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

void BMPpicture::calculateDependencies() throw(unknowPixelFormat) {
	try {
		colorTableBegining = picSettings.biSize + (sizeof(picHeader) - 2);							// "-2" is necessary because of memory rounding to words

		if (picSettings.biClrUsed) pixelFormatSize = ((picHeader.bfOffBits - colorTableBegining) / picSettings.biClrUsed); //Result should be 4(BRGA) or 3(BRG) depend on pixel representation
		else pixelFormatSize = (picSettings.biBitCount / 8);

		if ((pixelFormatSize != 3) && (pixelFormatSize != 4)) throw(unknowPixelFormat(std::to_string(pixelFormatSize)));
	}
	catch (unknowPixelFormat& Exception) {
		cout << Exception.what() << endl;
	}
}

void BMPpicture::getColorIndex() noexcept(true) {
	/*color table is array which contain indexed colors of pixel.
	Each pixel has his own index which point here.*/
	pic.seekg(colorTableBegining, std::ios_base::beg);							//color table begining
	for(;pic.tellg() != picHeader.bfOffBits;){									//save positions of each index in vector 
		colorTable.push_back(pic.tellg());
		pic.seekg(pixelFormatSize, std::ios_base::cur);
	}
}

string BMPpicture::rebuildPathForNewFile(string &&_operationName) {
	return filePath.insert(filePath.find(formatExtension), _operationName);
}

bool BMPpicture::makeFile(string _operation) throw(openingFileError) {
	try {
		string resultFilePath = rebuildPathForNewFile("_" + _operation);		//Prepare file path for output file

		operationResult.close();
		operationResult.open(resultFilePath, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

		if (!operationResult.is_open())	throw(openingFileError{ resultFilePath });

		//FILE HEADER INFO
		operationResult.seekp(0, std::ios_base::beg);					//File header begining
		operationResult.write((char*)&picHeader.bfType, 2);
		operationResult.write((char*)&picHeader.bfSize, 4);
		operationResult.write((char*)&picHeader.bfReserved1, 2);
		operationResult.write((char*)&picHeader.bfReserved2, 2);
		operationResult.write((char*)&picHeader.bfOffBits, 4);

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

		return true;
	}
	catch (openingFileError &Exception) {
		operationResult.close();
		cout << Exception.what() << endl;
	}
}

std::ofstream& BMPpicture::negative(){
	if (makeFile("negative")) {

		if (picSettings.biClrUsed) pic.seekg(colorTableBegining, std::ios_base::beg);			//for color table inversion
		else pic.seekg(picHeader.bfOffBits, std::ios_base::beg);				//for pixels direct inversion

		auto pToPix = new pixelBGR;												//auxiliary struct

		for (int y = 0; y <= picSettings.biHeight; ++y) {							//Pixels net (height)
			for (int x = 0; x <= picSettings.biWidth; ++x) {						//Pixels net (width)
				if (picSettings.biClrUsed && pic.tellg() == colorTableBegining) {	//color table inversion (if exist - only in first entrance to loop when there is color table and reading pointer is set at 54pos)
					for (; pic.tellg() < picHeader.bfOffBits;) {
						pic.read((char*)pToPix, pixelFormatSize);
						pToPix->pixelNegative();
						operationResult.write((char*)pToPix, pixelFormatSize);
					}
				}
				pic.read((char*)pToPix, pixelFormatSize);
				if (picSettings.biBitCount == 24) pToPix->pixelNegative();
				operationResult.write((char*)pToPix, pixelFormatSize);
			}
		}
		delete pToPix;
		std::cout << "Negative operation done.\n" << std::endl;
		return operationResult;
	}
}

//Operator used to show file header info of BMP pic
std::ostream& operator<<(std::ostream& outStream, fileHeader& fileHeader) {
	outStream //<< "\nFile path : " << var.filePath
		<< "\nfileHeader.bfType = " << fileHeader.bfType
		<< "\nfileHeader.bfSize = " << fileHeader.bfSize
		<< "\nfileHeader.bfReserved1 = " << fileHeader.bfReserved1
		<< "\nfileHeader.bfReserved2 = " << fileHeader.bfReserved2
		<< "\nfileHeader.bfOffBits = " << fileHeader.bfOffBits << endl;

	return outStream;
}

//Operator used to show image header info of BMP pic
std::ostream& operator<<(std::ostream& outStream, imgHeader& imgHeader) {
	outStream //<< "\nFile path : " << var.filePath
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

	return outStream;
}
