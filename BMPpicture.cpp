/*****************************************************************//**
 * \file   BMPpicture.cpp
 * \brief  Methods definitions of BMPpicture class.
 *
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

#include "BMPpicture.h"

/**
 * Is defining \ref formatSignature, checking if orginal file is correct and extracting picture metadata which is shown at the end.
 * \param [in] _path Path to orginal file.
 */
BMPpicture::BMPpicture(string _path) : Graphic{_path, ".bmp"}{
	formatSignature = static_cast<Tulli>(bmpSignature);				//66 77 ASCII - There is many possible signatures but only "BM" is supported by Windows - rest are OS/2 (IBM)

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

/**
 * Method used to extract metadata from orginal picture.
 */
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

/**
 * Method which inform if file include color table.
 * \return True - color table exist, false - color table not exist.
 */
bool BMPpicture::isColorTableExist() noexcept(true) {
	if (pictureSettings.biClrUsed == 256) colorTableExist = true;
	return colorTableExist;
}

/**
 * Method used to extract color table informations such as position and indexes.
 */
void BMPpicture::getColorTable() noexcept(true) {
		colorTableBegining = pictureSettings.biSize + (sizeof(pictureHeader) - 2);		// "-2" is necessary because of memory rounding to whole words
		getColorIndexes();
}

/**
 * Method used to calculate pixel format. It basing on color table (if exist) or bits per pixel information.
 * \throw unknowPixelFormat Exception which is thrown when pixel format is not recognized.
 */
void BMPpicture::calculatePixelFormat() noexcept(false) {
	try {
		if (pictureSettings.biClrUsed) pixelFormatSize = ((pictureHeader.bfOffBits - colorTableBegining) / pictureSettings.biClrUsed);
		else pixelFormatSize = (pictureSettings.biBitCount / bitsPerBytes);

		if ((pixelFormatSize != pixelFormatBGR) && (pixelFormatSize != pixelFormatBGRA)) throw(unknowPixelFormat(std::to_string(pixelFormatSize)));
	}
	catch (unknowPixelFormat& Exception) {
		cout << Exception.what() << endl;
	}
}

/**
 * Method used to extract color indexes of pixels from color table.
 * Color table is array which contain indexed colors of pixel.
 * Each pixel has his own index which point here. Color of pixel[0] = colorTable[0].
 */
void BMPpicture::getColorIndexes() noexcept(true) {

	originalPicture.seekg(colorTableBegining, std::ios_base::beg);

	for(;originalPicture.tellg() != pictureHeader.bfOffBits;){
		colorTable.push_back(originalPicture.tellg());
		originalPicture.seekg(pixelFormatSize, std::ios_base::cur);
	}
}

/**
 * Method used to rebuilding path to picture being operation result picture.
 * \param [in] _operationName String which should be added to actual path.
 * \return Path to operation result picture.
 */
string BMPpicture::rebuildPathForNewFile(string &&_operationName) {
	return filePath.insert(filePath.find(formatExtension), _operationName);
}

/**
 * Method used to create new picture and preparing it's metadata before operation.
 * \param [in] _operation Operation name.
 * \return True - if creation of operation result file is successfull.
 * \throw openingFileError Exception is thrown when opening  stream to operation result picture fail.
 */
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

/**
 * Method used as algorithm to reverse picture colors to negative.
 * \return Reference to stream opened as result picture.
 */
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

/**
 * Overloaded operator to show picture fileHeader informations.
 * \param [in&] outStream Reference to stream used as output.
 * \param [in&] fileHeader Reference to fileHeader informations.
 * \return Reference to used stream.
 */
std::ostream& operator<<(std::ostream& outStream, fileHeader& fileHeader) {
	outStream << "\nfileHeader.bfType = " << fileHeader.bfType
		<< "\nfileHeader.bfSize = " << fileHeader.bfSize
		<< "\nfileHeader.bfReserved1 = " << fileHeader.bfReserved1
		<< "\nfileHeader.bfReserved2 = " << fileHeader.bfReserved2
		<< "\nfileHeader.bfOffBits = " << fileHeader.bfOffBits << endl;

	return outStream;
}

/**
 * Overloaded operator to show picture imgHeader informations.
 * \param [in&] outStream Reference to stream used as output.
 * \param [in&] imgHeader Reference to imgHeader informations.
 * \return Reference to used stream.
 */
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
