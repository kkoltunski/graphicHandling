#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <iostream>
#include <string>
#include <fstream>
#include "errorsHandling.h"

using std::cout;
using std::endl;
using std::string;

using Tulli = unsigned long long int;

//Base class for graphic's objects
class Graphic
{
protected:
	Tulli getFileSignature(unsigned int _bytesToCheck, std::fstream::pos_type _beg = 0);

public:
	Graphic(string &_path, string &&_extension) noexcept(false);
	virtual ~Graphic() noexcept(true);
			
	void picturePossibleOperations() ;
		
protected:
	std::string filePath;
	Tulli formatSignature;
	const string formatExtension;
	std::ifstream originalPicture;
	std::ofstream operationResultPicture;
		
	virtual bool checkFileExtensionOnFilePath() noexcept(false);
	virtual bool isFileSignatureValid(unsigned int _bytesToCheck, std::fstream::pos_type _beg = 0) noexcept(false);
	virtual void getMetadata() noexcept(true) = 0;

	virtual std::ofstream& negative() = 0;
};

#endif
