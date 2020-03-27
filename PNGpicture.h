#ifndef PNGPICTURE_H
#define PNGPICTURE_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include "Graphic.h"
#include "Chunk.h"
#include "zlib.h"

using std::vector;
using std::string;
using std::fstream;		

struct IHDR{
	private:				
		unsigned int width, height;
		char bitDepth, colorType, compressionMeth, filterMeth, intelaceMeth; 
	
	friend class PNGpicture;
	friend std::ostream& operator<<(std::ostream &out, IHDR &var);
};

class PNGpicture : public Graphic
{
	private:
		vector<Chunk*> fileChunks;
		IHDR picSettings;
		
		bool checkFile() override;				//f. for preliminary file check
		void getSettings() override; 			//f. for metadata extraction
		
		void chunkingF();						//f. for chunking file
		void extractZlibHeader();				//f. to extract info necessarry for decoding
	
	public:
		PNGpicture(string _path);
		virtual ~PNGpicture() override;
		
		void showChunks();
		void showChunkData(int inList);
		
	protected:
		zlib zlibHeader;
};

#endif
