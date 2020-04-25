/*****************************************************************//**
 * \file   BMPpicture.h
 * \brief  Header for .bmp format and all additional structures.
 * 
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

#ifndef BMPPICTURE_H
#define BMPPICTURE_H

#include "Graphic.h"

using std::vector;
using std::string;
using std::fstream;

/**
 * Structure which represents part of metadata with format settings.
 */
struct fileHeader{
private:
	///The characters "BM"
	unsigned short bfType;	
	///The size of the file in bytes
	unsigned int bfSize;		
	///Unused - must be zero
	unsigned short bfReserved1;
	///Unused - must be zero
	unsigned short bfReserved2;
	///Offset to start of Pixel Data
	unsigned int bfOffBits;
	
	///Main .bmp format class.
	friend class BMPpicture;
	///Showing parameters.
	friend std::ostream& operator<<(std::ostream &out, fileHeader &fileHeader);	
};

/**
 * Structure which represents part of metadata with picture settings.
 */
struct imgHeader{
private:
	///Header Size - Must be at least 40
	unsigned int biSize;
	///Image width in pixels
	unsigned int biWidth;	
	///Image height in pixels
	unsigned int biHeight;	
	///Must be 1
	unsigned short biPlanes;	
	///Bits per pixel - 1, 4, 8, 16, 24, or 32
	unsigned short biBitCount;
	///Compression type (0 = uncompressed)
	unsigned int biCompression;	
	///Image Size - may be zero for uncompressed images
	unsigned int biSizeImage;	
	///Preferred resolution in pixels per meter
	unsigned int biXPelsPerMeter;	
	///Preferred resolution in pixels per meter
	unsigned int biYPelsPerMeter;	
	///Number Color Map entries that are actually used
	unsigned int biClrUsed;			
	///Number of significant colors
	unsigned int biClrImportant;	
	
	///Main .bmp format class.
	friend class BMPpicture;
	///Showing parameters.
	friend std::ostream& operator<<(std::ostream &out, imgHeader &imgHeader);		 
};

/**
 * Structure which represent pixel format.
 */
struct pixelBGR{
protected:
	///Blue channel.
	unsigned char Blue;
	///Green channel.
	unsigned char Green;
	///Red channel.
	unsigned char Red;
	///Alpha channel is not used but exist for future solutions.
	unsigned char Alpha;

public:
	/**
	 * Method which rotate pixel values.
	 */
	inline void pixelNegative() {
		this->Blue ^= 0xFF;
		this->Green ^= 0xFF;
		this->Red ^= 0xFF;
		this->Alpha = this->Alpha;
	}

	///Main .bmp format class.
	friend class BMPpicture;
};

/**
 * Main .bmp format class.
 */
class BMPpicture : public Graphic
{
private:
	///Recognized pixel format size.
	size_t pixelFormatSize;
	///Picture header.
	fileHeader pictureHeader;
	///Picture settings.
	imgHeader pictureSettings;
	///Information if picture has color table.
	bool colorTableExist;
	///Begining position of color table.
	int colorTableBegining;
	///Positions to pixel indexed colours (pos 0 = pix 0, pos 1 = pix 1 etc...).
	vector<std::fstream::pos_type> colorTable;				
	
	///Extracting picture metadata.
	void getMetadata() noexcept(true) override;
	///Checking if color table exist.
	bool isColorTableExist() noexcept(true);
	///Color table extracting.
	void getColorTable() noexcept(true);
	///Color indexes extracting.
	void getColorIndexes() noexcept(true);
	///Pixel format reading.
	void calculatePixelFormat() noexcept(false);
	
	///Preparing path for result operation picture.
	string rebuildPathForNewFile(string&& _operationName);
	///Basic operations before result picture creation.
	bool makeFile(string _operation) noexcept(false);

	///Negative operation algorithm.
	virtual std::ofstream& negative() override;

public:
	///Constructor.
	BMPpicture(string _path);
	///Destructor.
	virtual ~BMPpicture() noexcept(true) override;	
};

#endif
