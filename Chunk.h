#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <string>
#include <fstream>

using std::vector;
using std::string;

//chunk schem representation
//	|LENGTH|CHUNK_TYPE|CHUNK_DATA = optional|CRC|
class Chunk{
	private:
		std::fstream::pos_type begOfChunk;				//1st length byte
		unsigned int chunkLenght;						//bytes in chunk
		bool criticalChunk;								//0 ancillary , 1 critical
		
		unsigned int dataLength;						//length of data field in chunk
		string chunkType;								//chunk Name
		std::fstream::pos_type chunkDataPos;			//begining of data field in chunk
		//char chunkCRC [4];							
	
		Chunk(const char name[]) : chunkType{name}{
		}
		
		friend class PNGpicture;
};

#endif
