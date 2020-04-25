/*****************************************************************//**
 * \file   Graphic.cpp
 * \brief  Methods definitions of Graphic abstract class.
 * 
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

#include "Graphic.h"

extern string changeToLowerCases(string _in);

/**
 * Trying to open istream to file given as _path.
 * \param [in&] _path Reference of object which store path to file.
 * \param [in&&] _extension Extension of file.
 * \throw openingFileError Exception is thrown when opening stream to original file fail.
 * \todo In the future Graphic can be factory which decide itself which real object has to be created (based on _extension).
 */
Graphic::Graphic(string &_path, string &&_extension) : filePath{ _path }, formatExtension{ _extension }{
	try {
		originalPicture.open(filePath, std::ios_base::in | std::ios_base::binary);
	
		if (!originalPicture.is_open()) {
			throw(openingFileError(filePath));
		}
	}
	catch (openingFileError& Exception) {
		originalPicture.clear();
		cout << Exception.what() << endl;
		terminate();
	}
}

/**
 * Is Closing opened streams.
 */
Graphic::~Graphic(){
	originalPicture.close();
	operationResultPicture.close();
}

/**
 * Method reading signature from original file.
 * \param [in] _bytesToCheck Bytes number to read (format signature size in bytes).
 * \param [in] _beg Signature first byte in original file.
 * \return Value of signature.
 */
Tulli Graphic::getFileSignature(unsigned int _bytesToCheck, std::fstream::pos_type _beg) {
	Tulli takenToCheck{};

	if (!originalPicture.fail()) {
		originalPicture.seekg(_beg, std::ios_base::beg);
		originalPicture.read((char*)&takenToCheck, _bytesToCheck);
	}

	return takenToCheck;
}

/**
 * Method used to find \ref formatExtension tekst in given \ref filePath.
 * \return True if content of \ref formatExtension is found in given \ref filePath.
 * \throw badFileExtension Exception is thrown when content of \ref formatExtension is not found in given \ref filePath.
 */
bool Graphic::checkFileExtensionOnFilePath() noexcept(false) {
	try {
		string help = changeToLowerCases(filePath);
		size_t extensionFormatPos = help.rfind(formatExtension);
		size_t extensionSize = formatExtension.size();

		if (extensionFormatPos != (help.size() - extensionSize)) {
			throw(badFileExtension(formatExtension));
		}
		else {
			return true;
		}
	}
	catch (badFileExtension& Exception) {
		cout << Exception.what() << endl;
		terminate();
	}
}

/**
 * Method used to compare readed signature with model signature.
 * \param [in] _bytesToCheck Bytes number to read (format signature size in bytes).
 * \param [in] _beg Signature first byte in original file.
 * \return True if comparing is done successfull.
 * \throw unequalSignature Exception is thrown when readed signature is not equal to standard signature.
 */
bool Graphic::isFileSignatureValid(unsigned int _bytesToCheck, std::fstream::pos_type _beg) noexcept(false) {
	try {
		Tulli readedSignature = getFileSignature(_bytesToCheck, _beg);

		if (readedSignature != formatSignature) {
			throw(unequalSignature(std::to_string(readedSignature), std::to_string(formatSignature)));
		}
		else {
			return true;
		}
	}
	catch (unequalSignature &Exception) {
		cout << Exception.what() << endl;
		terminate();
	}
}

/**
 * Method to show user possible operations on graphic.
 */
void Graphic::picturePossibleOperations(){
	std::cout << "At this moment only negative function is possible." << std::endl;
	negative();
}
