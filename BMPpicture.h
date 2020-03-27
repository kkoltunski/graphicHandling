#ifndef BMPPICTURE_H
#define BMPPICTURE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Graphic.h"

using std::vector;
using std::string;
using std::fstream;

struct fileHeader{
	private:
		unsigned short bfType;						//The characters "BM"
		unsigned int   bfSize;						//The size of the file in bytes
		unsigned short bfReserved1,					//Unused - must be zero
					   bfReserved2;					//Unused - must be zero
		unsigned int   bfOffBits;					//Offset to start of Pixel Data
		
	friend class BMPpicture;
	friend std::ostream& operator<<(std::ostream &out, fileHeader &fileHeader);	
};

struct imgHeader{
	private:
		unsigned int   biSize, 						//Header Size - Must be at least 40
					   biWidth,						//Image width in pixels
					   biHeight;					//Image height in pixels
		unsigned short biPlanes,					//Must be 1
					   biBitCount;					//Bits per pixel - 1, 4, 8, 16, 24, or 32
		unsigned int   biCompression,				//Compression type (0 = uncompressed)
					   biSizeImage,					//Image Size - may be zero for uncompressed images
					   biXPelsPerMeter,				//Preferred resolution in pixels per meter
					   biYPelsPerMeter,				//Preferred resolution in pixels per meter
					   biClrUsed,					//Number Color Map entries that are actually used
					   biClrImportant;				//Number of significant colors
				 
	friend class BMPpicture;
	friend std::ostream& operator<<(std::ostream &out, imgHeader &imgHeader);		 
};

struct pixel{										//In pixels data field oposite direction because of little endian
	unsigned char Blue,
				  Green,
				  Red;
};

class BMPpicture : public Graphic
{
	private:
		fileHeader picIntro;					//File header structure
		imgHeader picSettings;					//Image header structure
		
		//construktor for the future solutions
		bool checkFile() override;				//f. for preliminary file check
		void getSettings() override; 			//f. for metadata extraction
		
		void makeHeader();
		
	public:
		BMPpicture(string _path);
		~BMPpicture() override;
		
		virtual std::ofstream& negative() override;
		
		
	protected:
		
};

#endif
