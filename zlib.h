#ifndef ZLIB_H
#define ZLIB_H

#include <iostream>
#include <algorithm>

struct dataHeaderBits{
	public:
		unsigned char BFINAL : 1,				//0s bit - should be set only for last data block
			 		  BTYPE : 2;				//1-2nd bits - inform about how data is compressed
			 
	friend class zlib;
};

//zlib header schem representation
//	|CMF|FLG|
struct zlib{						//zlib header structure which comes from fisrt IDAT chunk and Deflate algorithm						
	private:
		unsigned short CMF : 4,		//0s byte CMF, 0-3rd bits	Compression method
					   CINFO : 4,	//0s byte CMF, 4-7th bits	compression info
					   FCHECK : 5,	//1th byte FLG, 0-4th bits (abs. 8-12 thbits)	flag
					   FDICT : 1,	//1th byte FLG, 5th bit (abs. 13th bit)		flag
					   FLEVEL : 2;	//1th byte FLG, 6-7th bits (abs 14-15th bit)	flag
	
		dataHeaderBits DHB;			//have to be checked for each IDAT?
		
		int calcWindow(){			//f. for future decoding
			return std::pow(2, (CINFO + 8));
		}
		
		friend class PNGpicture;	
};



#endif
