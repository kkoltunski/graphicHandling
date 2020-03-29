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

struct pixelBGR{									//In pixels data field oposite direction because of little endian
	protected:
		unsigned char Blue,
					  Green,
					  Red,
					  Alpha;						//is not used - should be 0x0
					  
	friend class BMPpicture;
};

class BMPpicture : public Graphic
{
	private:
		fileHeader picIntro;					//File header structure
		imgHeader picSettings;					//Image header structure
		vector<std::fstream::pos_type> colorTable; //Positions to pixel indexed colours pos 0 = pix 0, pos 1 = pix 1 etc...
		
		//construktor for the future solutions(?)
		bool checkFile() override;				//f. for preliminary file check
		void getSettings() override; 			//f. for metadata extraction
		void getColorIndex();					//f. to get positions of indexed color in orginal file
		
		bool makeFile(string _operation);		//f for new file basic operation 
		
	public:
		BMPpicture(string _path);
		~BMPpicture() override;
		
		virtual std::ofstream& negative() override;
		
		
	protected:
		
};

#endif
