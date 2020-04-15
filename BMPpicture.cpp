#include "BMPpicture.h"

BMPpicture::BMPpicture(string _path) : Graphic{_path, ".bmp"}{
	formatSignature = 19778;								//66 77 ASCII - There is many possible signatures but only "BM" is supported by Windows - rest are OS/2 (IBM)

	if (checkFileExtensionOnFilePath()) {
		if (isFileSignatureValid(2)) {
			getMetadata();
			calculatePixelFormat();
			if(isColorTableExist()) getColorTable();

			if (pictureSettings.biCompression)	std::cout << "Decompression necessary" << std::endl;

			std::cout << "BMP PICTURE SETTINGS";
			std::cout << pictureHeader;
			std::cout << pictureSettings << std::endl;
		}
	}
}

BMPpicture::~BMPpicture() noexcept(true) {
	
}

void BMPpicture::getMetadata() noexcept(true) {
//FILE HEADER INFO
	originalPicture.seekg(0, std::ios_base::beg);
	originalPicture.read((char*)&pictureHeader.bfType, 2);
	originalPicture.read((char*)&pictureHeader.bfSize, 4);
	originalPicture.read((char*)&pictureHeader.bfReserved1, 2);
	originalPicture.read((char*)&pictureHeader.bfReserved2, 2);
	originalPicture.read((char*)&pictureHeader.bfOffBits, 4);

//IMAGE HEADER INFO
	originalPicture.read((char*)&pictureSettings.biSize, 4);
	originalPicture.read((char*)&pictureSettings.biWidth, 4);
	originalPicture.read((char*)&pictureSettings.biHeight, 4);
	originalPicture.read((char*)&pictureSettings.biPlanes, 2);
	originalPicture.read((char*)&pictureSettings.biBitCount, 2);
	originalPicture.read((char*)&pictureSettings.biCompression, 4);
	originalPicture.read((char*)&pictureSettings.biSizeImage, 4);
	originalPicture.read((char*)&pictureSettings.biXPelsPerMeter, 4);
	originalPicture.read((char*)&pictureSettings.biYPelsPerMeter, 4);
	originalPicture.read((char*)&pictureSettings.biClrUsed, 4);
	originalPicture.read((char*)&pictureSettings.biClrImportant, 4);
}

bool BMPpicture::isColorTableExist() noexcept(true) {
	if (pictureSettings.biClrUsed == 256) colorTableExist = true;
	return colorTableExist;
}

void BMPpicture::getColorTable() noexcept(true) {
		colorTableBegining = pictureSettings.biSize + (sizeof(pictureHeader) - 2);		// "-2" is necessary because of memory rounding to whole words
		getColorIndexes();
}

void BMPpicture::calculatePixelFormat() noexcept(false) {
	try {
		if (pictureSettings.biClrUsed) pixelFormatSize = ((pictureHeader.bfOffBits - colorTableBegining) / pictureSettings.biClrUsed);
		else pixelFormatSize = (pictureSettings.biBitCount / 8);

		if ((pixelFormatSize != 3) && (pixelFormatSize != 4)) throw(unknowPixelFormat(std::to_string(pixelFormatSize)));
	}
	catch (unknowPixelFormat& Exception) {
		cout << Exception.what() << endl;
	}
}

void BMPpicture::getColorIndexes() noexcept(true) {
	/*color table is array which contain indexed colors of pixel.
	Each pixel has his own index which point here. Color of pixel[0] = colorTable[0]*/
	originalPicture.seekg(colorTableBegining, std::ios_base::beg);

	for(;originalPicture.tellg() != pictureHeader.bfOffBits;){
		colorTable.push_back(originalPicture.tellg());
		originalPicture.seekg(pixelFormatSize, std::ios_base::cur);
	}
}

string BMPpicture::rebuildPathForNewFile(string &&_operationName) {
	return filePath.insert(filePath.find(formatExtension), _operationName);
}

bool BMPpicture::makeFile(string _operation) noexcept(false) {
	try {
		string resultFilePath = rebuildPathForNewFile("_" + _operation);

		operationResultPicture.close();
		operationResultPicture.open(resultFilePath, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

		if (!operationResultPicture.is_open())	throw(openingFileError{ resultFilePath });

		//FILE HEADER INFO
		operationResultPicture.seekp(0, std::ios_base::beg);
		operationResultPicture.write((char*)&pictureHeader.bfType, 2);
		operationResultPicture.write((char*)&pictureHeader.bfSize, 4);
		operationResultPicture.write((char*)&pictureHeader.bfReserved1, 2);
		operationResultPicture.write((char*)&pictureHeader.bfReserved2, 2);
		operationResultPicture.write((char*)&pictureHeader.bfOffBits, 4);

		//IMAGE HEADER INFO
		operationResultPicture.write((char*)&pictureSettings.biSize, 4);
		operationResultPicture.write((char*)&pictureSettings.biWidth, 4);
		operationResultPicture.write((char*)&pictureSettings.biHeight, 4);
		operationResultPicture.write((char*)&pictureSettings.biPlanes, 2);
		operationResultPicture.write((char*)&pictureSettings.biBitCount, 2);
		operationResultPicture.write((char*)&pictureSettings.biCompression, 4);
		operationResultPicture.write((char*)&pictureSettings.biSizeImage, 4);
		operationResultPicture.write((char*)&pictureSettings.biXPelsPerMeter, 4);
		operationResultPicture.write((char*)&pictureSettings.biYPelsPerMeter, 4);
		operationResultPicture.write((char*)&pictureSettings.biClrUsed, 4);
		operationResultPicture.write((char*)&pictureSettings.biClrImportant, 4);

		return true;
	}
	catch (openingFileError &Exception) {
		operationResultPicture.close();
		cout << Exception.what() << endl;
	}
}

std::ofstream& BMPpicture::negative(){
	if (makeFile("negative")) {

		if (colorTableExist) originalPicture.seekg(colorTableBegining, std::ios_base::beg);
		else originalPicture.seekg(pictureHeader.bfOffBits, std::ios_base::beg);
		
		auto pToPix = new pixelBGR;

		for (int y = 0; y <= pictureSettings.biHeight; ++y) {
			for (int x = 0; x <= pictureSettings.biWidth; ++x) {
				if (colorTableExist && originalPicture.tellg() == colorTableBegining) {
					for (; originalPicture.tellg() < pictureHeader.bfOffBits;) {
						originalPicture.read((char*)pToPix, pixelFormatSize);
						pToPix->pixelNegative();
						operationResultPicture.write((char*)pToPix, pixelFormatSize);
					}
				}
				originalPicture.read((char*)pToPix, pixelFormatSize);
				if (pictureSettings.biBitCount == 24) pToPix->pixelNegative();
				operationResultPicture.write((char*)pToPix, pixelFormatSize);
			}
		}
		delete pToPix;
		std::cout << "Negative operation done.\n" << std::endl;
		return operationResultPicture;
	}
}

std::ostream& operator<<(std::ostream& outStream, fileHeader& fileHeader) {
	outStream << "\nfileHeader.bfType = " << fileHeader.bfType
		<< "\nfileHeader.bfSize = " << fileHeader.bfSize
		<< "\nfileHeader.bfReserved1 = " << fileHeader.bfReserved1
		<< "\nfileHeader.bfReserved2 = " << fileHeader.bfReserved2
		<< "\nfileHeader.bfOffBits = " << fileHeader.bfOffBits << endl;

	return outStream;
}

std::ostream& operator<<(std::ostream& outStream, imgHeader& imgHeader) {
	outStream << "\nimgHeader.biSize = " << imgHeader.biSize
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
