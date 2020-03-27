#include "Graphic.h"

Graphic::Graphic(std::string _path) : filePath{_path}{
	pic.open(filePath, std::ios_base::in | std::ios_base::binary);
	//checkFile() which create "new" object based on format picture
}

Graphic::~Graphic(){
	pic.close();
	operationResult.close();
}

bool Graphic::validateSignature(unsigned int _bytesToCheck, std::fstream::pos_type _beg){
	unsigned long long int takenToCheck {};
	
	if(!pic.fail()){
		pic.seekg(_beg, std::ios_base::beg);				
		pic.read((char*)&takenToCheck, _bytesToCheck);
	}			
	
	return (takenToCheck == signature ? true : false);	//result of checking
}

void Graphic::options(){
	std::cout << "At this moment only negative function is possible." << std::endl;
	negative();
}
