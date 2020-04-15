#ifndef PNGPICTURE_H
#define PNGPICTURE_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include "Graphic.h"

using std::vector;
using std::string;
using std::fstream;		

/*chunk scheme representation
|LENGTH|CHUNK_TYPE|CHUNK_DATA = optional|CRC|*/
struct Chunk {
private:
	std::fstream::pos_type begining;
	unsigned int chunkLenght;
	bool criticalChunk;								//0 ancillary , 1 critical

	unsigned int dataFieldLength;
	string type;
	std::fstream::pos_type dataFieldBegining;						

	Chunk(const char _name[]) : type{ _name } {
	}

	friend class PNGpicture;
};

struct IHDR{
private:				
	unsigned int width,
				 height;
	char		 bitDepth, 
				 colorType, 
				 compressionMeth, 
				 filterMeth, 
				 intelaceMeth; 
	
	friend class PNGpicture;
	friend std::ostream& operator<<(std::ostream &out, IHDR &var);
};

/*RFC1950 zlib compressed data format*/
struct dataHeaderBits {
public:
	unsigned char BFINAL : 1,						//0s bit - should be set only for last data block
				  BTYPE : 2;						//1-2nd bits - inform about how data is compressed

	friend struct zlibHeader;
};

/* RFC1950 zlib compressed data format
zlib header scheme representation
|CMF|FLG|*/
struct zlibHeader {									//zlib header structure which comes from fisrt IDAT chunk and Deflate algorithm						
private:
	unsigned short CMF : 4,							//0s byte CMF, 0-3rd bits Compression method
		CINFO : 4,									//0s byte CMF, 4-7th bits compression info
		FCHECK : 5,									//1th byte FLG, 0-4th bits (abs. 8-12 thbits) flag
		FDICT : 1,									//1th byte FLG, 5th bit (abs. 13th bit)	flag
		FLEVEL : 2;									//1th byte FLG, 6-7th bits (abs 14-15th bit) flag

	dataHeaderBits DHB;								//have to be checked for each IDAT?

	inline int calcWindow() {
		return std::pow(2, (CINFO + 8));
	}

	friend class PNGpicture;
	friend std::ostream& operator<<(std::ostream& outStream, zlibHeader& zlibHeader);
};

class PNGpicture : public Graphic
{
private:
	vector<Chunk*> fileChunks;
	IHDR picSettings;
	zlibHeader zlibHeader;

	void getMetadata() noexcept(true) override;
	void filePartitioning();
	void extractZlibHeader() noexcept(false);
	std::ofstream& negative() override;

public:
	PNGpicture(string _path);
	virtual ~PNGpicture() noexcept(true) override;
		
	void showChunks();
	void showChunkData(int inList);
};

#endif
