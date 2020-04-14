#include "Graphic.h"

extern string changeToLowerCases(string _in);

Graphic::Graphic(string &_path, string &&_extension) : filePath{ _path }, formatExtension{ _extension }{
	try {
		pic.open(filePath, std::ios_base::in | std::ios_base::binary);
	
		if (!pic.is_open()) throw(openingFileError(filePath));
	}
	catch (openingFileError& Exception) {
		pic.clear();
		cout << Exception.what() << endl;
		terminate();
	}
}

Graphic::~Graphic(){
	pic.close();
	operationResult.close();
}

Tulli Graphic::getFileSignature(unsigned int _bytesToCheck, std::fstream::pos_type _beg) {
	Tulli takenToCheck{};

	if (!pic.fail()) {
		pic.seekg(_beg, std::ios_base::beg);
		pic.read((char*)&takenToCheck, _bytesToCheck);
	}

	return takenToCheck;
}

bool Graphic::checkFileExtension() throw(badFileExtension) {
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

bool Graphic::validateSignature(unsigned int _bytesToCheck, std::fstream::pos_type _beg) throw(unequalSignature){
	try {
		Tulli readedSignature = getFileSignature(_bytesToCheck, _beg);

		if (readedSignature != signature) throw(unequalSignature(std::to_string(readedSignature), std::to_string(signature)));
		else return true;
	}
	catch (unequalSignature &Exception) {
		cout << Exception.what() << endl;
		terminate();
	}
}

void Graphic::options(){
	std::cout << "At this moment only negative function is possible." << std::endl;
	negative();
}
