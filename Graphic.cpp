#include "Graphic.h"

extern string changeToLowerCases(string _in);

Graphic::Graphic(string &_path, string &&_extension) : filePath{ _path }, formatExtension{ _extension }{
	try {
		originalPicture.open(filePath, std::ios_base::in | std::ios_base::binary);
	
		if (!originalPicture.is_open()) throw(openingFileError(filePath));
	}
	catch (openingFileError& Exception) {
		originalPicture.clear();
		cout << Exception.what() << endl;
		terminate();
	}
}

Graphic::~Graphic(){
	originalPicture.close();
	operationResultPicture.close();
}

Tulli Graphic::getFileSignature(unsigned int _bytesToCheck, std::fstream::pos_type _beg) {
	Tulli takenToCheck{};

	if (!originalPicture.fail()) {
		originalPicture.seekg(_beg, std::ios_base::beg);
		originalPicture.read((char*)&takenToCheck, _bytesToCheck);
	}

	return takenToCheck;
}

bool Graphic::checkFileExtensionOnFilePath() noexcept(false) {
	try {
		string help = changeToLowerCases(filePath);
		size_t extensionFormatPos = help.rfind(formatExtension);
		size_t extensionSize = formatExtension.size();

		if (extensionFormatPos != (help.size() - extensionSize)) throw(badFileExtension(formatExtension));
		else return true;
	}
	catch (badFileExtension& Exception) {
		cout << Exception.what() << endl;
		terminate();
	}
}

bool Graphic::isFileSignatureValid(unsigned int _bytesToCheck, std::fstream::pos_type _beg) noexcept(false) {
	try {
		Tulli readedSignature = getFileSignature(_bytesToCheck, _beg);

		if (readedSignature != formatSignature) throw(unequalSignature(std::to_string(readedSignature), std::to_string(formatSignature)));
		else return true;
	}
	catch (unequalSignature &Exception) {
		cout << Exception.what() << endl;
		terminate();
	}
}

void Graphic::picturePossibleOperations(){
	std::cout << "At this moment only negative function is possible." << std::endl;
	negative();
}
