/*****************************************************************//**
 * \file   PNGpicture.h
 * \brief  Header for .png format and all additional structures.
 * 
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

#ifndef PNGPICTURE_H
#define PNGPICTURE_H

#include <stdexcept>
#include "Graphic.h"

using std::vector;
using std::string;
using std::fstream;		

/**
 * Structure which represents part of picture.
 * PNG format pictures are divided into chunks. Most important chunks are critical chunks:
 *	-# IHDR - always first chunk;
 *  -# IDAT - chunk with data;
 *  -# PLTE - color table (optional);
 *  -# IEND - last chunk which mean end of picture;
 * 
 * Chunk scheme representation 
 * |LENGTH|CHUNK_TYPE|CHUNK_DATA = optional|CRC|
 * 
 * \sa Anciliary chunks overview: <a href="linkURL">http://www.libpng.org/pub/png/spec/1.2/PNG-Chunks.html#C.Anc-chunks</a> 
 */
struct Chunk {
private:
	///First chunk byte.
	std::fstream::pos_type begining;
	///Completely chunk length.
	unsigned int chunkLenght;
	///Information if chunk is critical (0 ancillary , 1 critical)
	bool criticalChunk;

	///Length of field data.
	unsigned int dataFieldLength;
	///Type of chunk.
	string type;
	///First data field byte.
	std::fstream::pos_type dataFieldBegining;						

	///Constructor to initialize type of chunk.
	Chunk(const char _name[]) : type{ _name } {
	}

	///Main .png format class.
	friend class PNGpicture;
};

/**
 * Structure which represent IHDR chunk - metadata of picture.
 */
struct IHDR{
private:
	///Pixels width.
	unsigned int width;
	///Pixels heigth.
	unsigned int height;
	///Bit depth (1, 2, 4, 8, 16).
	char bitDepth;
	///Color type (0, 2, 3, 4, 6).
	char colorType;
	///Have to be 0 which means deflate/inflate compression with a sliding window of at most 32768 bytes.
	char compressionMeth;
	///Have to be 0 which means adaptive filtering with five basic filter types.
	char filterMeth;
	///Possible values: 0 (no interlace) or 1 (Adam7 interlace).
	char intelaceMeth;
	
	///Main .png format class.
	friend class PNGpicture;
	///Showing parameters.
	friend std::ostream& operator<<(std::ostream &out, IHDR &var);
};

/**
 * Structure shich is part of zlibHeader.
 */
struct dataHeaderBits {
public:
	///0s bit - should be set only for last data block
	unsigned char BFINAL : 1,	
	///1-2nd bits - inform about how data is compressed
				  BTYPE : 2;						

	///Parent structure.
	friend struct zlibHeader;
};

/**
 * Structure which comes from fisrt IDAT chunk. It has settings which are necessary to doeflate/inflate algorithm.
 * zlib header scheme representation
 * |CMF|FLG|
 *
 * Specified information are available in RFC1950.
 */
struct zlibHeader {														
private:
	///0s byte CMF, 0-3rd bits Compression method
	unsigned short CMF : 4,						
	///0s byte CMF, 4-7th bits compression info
				CINFO : 4,								
	///1th byte FLG, 0-4th bits (abs. 8-12 thbits) flag
				FCHECK : 5,		
	///1th byte FLG, 5th bit (abs. 13th bit)	flag
				FDICT : 1,
	///1th byte FLG, 6-7th bits (abs 14-15th bit) flag
				FLEVEL : 2;									

	///have to be checked for each IDAT?
	dataHeaderBits DHB;

	///Method to calculate window used to decoding process.
	inline int calcWindow() {
		return std::pow(2, (CINFO + 8));
	}

	///Main .png format class.
	friend class PNGpicture;
	///Showing parameters.
	friend std::ostream& operator<<(std::ostream& outStream, zlibHeader& zlibHeader);
};

/**
 * Main .png format class.
 */
class PNGpicture : public Graphic
{
private:
	///Chunk storage.
	vector<Chunk*> fileChunks;
	///Picture metadata.
	IHDR picSettings;
	///Decoding settings.
	zlibHeader zlibHeader;

	///Extracting picture metadata.
	void getMetadata() noexcept(true) override;
	///Chunks extracting.
	void filePartitioning();
	///zLib header reading.
	void extractZlibHeader() noexcept(false);

	///Negative operation algorithm.
	virtual std::ofstream& negative() override;

public:
	///Constructor.
	PNGpicture(string _path);
	///Destructor.
	virtual ~PNGpicture() noexcept(true) override;
		
	///Show extracted chunks.
	void showChunks();
	///Show specified chunk data.
	void showChunkData(int inList);
};

#endif
