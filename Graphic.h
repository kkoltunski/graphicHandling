#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <iostream>
#include <string>
#include <fstream>

using std::string;

//Base class for graphic's objects
class Graphic
{
	public:
		Graphic(std::string _path);
		virtual ~Graphic();
		
		//typedef std::ofstream& (Graphic::*pToFunc)();		//pointer for options() result
		
		void options();										//Function should return adress to choosen operation
		virtual std::ofstream& negative() = 0;
		
	protected:
		std::string filePath;								//File path	
		unsigned long long int signature;					//signature of format
		std::ifstream pic;									//Fstream to file
		std::ofstream operationResult;						//Fstream to new file
		
		virtual bool checkFile() = 0;						//f. for preliminary file check
		virtual bool validateSignature(unsigned int _bytesToCheck, std::fstream::pos_type _beg = 0);	//f. for check file signature
		virtual void getSettings() = 0;						//f. for metadata extraction
};

#endif
