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
	Graphic(string &_path, string &&_extension) throw(openingFileError);
	virtual ~Graphic() noexcept(true);
			
	void options();										//Function should return adress to choosen operation
		
protected:
	std::string filePath;								//File path	
	Tulli signature;									//signature of format
	const string formatExtension;
	std::ifstream pic;									//Fstream to file
	std::ofstream operationResult;						//Fstream to new file
		
	virtual bool checkFileExtension() throw(badFileExtension);	//f. to check file extension format in given path to file
	virtual bool validateSignature(unsigned int _bytesToCheck, std::fstream::pos_type _beg = 0) throw(unequalSignature) ;	//f. for check file signature
	virtual void getSettings() noexcept(true) = 0;		//f. for metadata extraction

	virtual std::ofstream& negative() = 0;
};

#endif
