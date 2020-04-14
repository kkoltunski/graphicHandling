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

struct pixelBGR{								//In pixels data field oposite direction because of little endian
protected:
	unsigned char Blue,
				  Green,
				  Red,
				  Alpha;						//is not used - should be 0x0
public:
	inline void pixelNegative() {
		this->Blue ^= 0xFF;
		this->Green ^= 0xFF;
		this->Red ^= 0xFF;
		this->Alpha = this->Alpha;
	}

	friend class BMPpicture;
};

class BMPpicture : public Graphic
{
private:
	size_t pixelFormatSize;
	fileHeader picHeader;											//File header structure
	imgHeader picSettings;											//Image header structure
	int colorTableBegining;
	vector<std::fstream::pos_type> colorTable;						//Positions to pixel indexed colours pos 0 = pix 0, pos 1 = pix 1 etc...
		
	void getSettings() noexcept(true) override; 					//f. for metadata extraction
	void calculateDependencies() throw(unknowPixelFormat);			//f, for calculate dependencies as pixelFormatSize, colorTableBegining etc...
	void getColorIndex() noexcept(true);							//f. to get positions of indexed color in orginal file
	
	string rebuildPathForNewFile(string&& _operationName);
	bool makeFile(string _operation) throw(openingFileError);		//f. for new file basic operation 

	virtual std::ofstream& negative() override;
public:
	BMPpicture(string _path);
	~BMPpicture() noexcept(true) override;
		
protected:		
};

#endif
