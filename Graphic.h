/*****************************************************************//**
 * \file   Graphic.h
 * \brief  Abstract class for graphic objects.
 * 
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "errorsHandling.h"
#include "settings.h"

using std::cout;
using std::endl;
using std::string;

using Tulli = unsigned long long int;

/**
 * Abstract class which represent basic methods and fields of graphics objects.
 */
class Graphic
{
protected:
	///Path to directory with original picture.
	std::string filePath;
	///Format magic numbers.
	Tulli formatSignature;
	///Format extension.
	const string formatExtension;
	///Stream to orginal picture.
	std::ifstream originalPicture;
	///Stream to picture which is result of operations.
	std::ofstream operationResultPicture;

	///Read signature.
	Tulli getFileSignature(unsigned int _bytesToCheck, std::fstream::pos_type _beg = 0);
	///Checking file extension.
	virtual bool checkFileExtensionOnFilePath() noexcept(false);
	///File signature checking.
	virtual bool isFileSignatureValid(unsigned int _bytesToCheck, std::fstream::pos_type _beg = 0) noexcept(false);
	///Extracting pictrure settings.
	virtual void getMetadata() noexcept(true) = 0;

	///Negative operation.
	virtual std::ofstream& negative() = 0;

public:
	///Constructor.
	Graphic(string &_path, string &&_extension) noexcept(false);
	///Destructor.
	virtual ~Graphic() noexcept(true);
			
	///Possible operation overview.
	void picturePossibleOperations();
};

#endif
