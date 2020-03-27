#include "PNGpicture.h"

extern std::ostream& operator<<(std::ostream &out, IHDR &var);

PNGpicture::PNGpicture(string _path) : Graphic{_path}{
	signature = 727905341920923785;					//137 80 78 71 13 10 26 10 added as byte's
	checkFile();
}

PNGpicture::~PNGpicture(){
	for(int i = 0; i < fileChunks.size(); ++i){
		delete fileChunks[i];
	}
}

bool PNGpicture::checkFile(){
	if(pic.is_open()){
		if(validateSignature(8)){						//Check if PNG file (PNG signature)
			chunkingF();
			getSettings();
			extractZlibHeader();
			
			std::cout << "PNG PICTURE SETTINGS";
			std::cout << picSettings;
			std::cout << "\nzLib Header info\n";
			std::cout << "zlibHeader.CMF = " << zlibHeader.CMF << std::endl;
			std::cout << "zlibHeader.CINFO = " << zlibHeader.CINFO << std::endl;
			std::cout << "zlibHeader.FCHECK = " << zlibHeader.FCHECK << std::endl;
			std::cout << "zlibHeader.FDICT = " << zlibHeader.FDICT << std::endl;
			std::cout << "zlibHeader.FLEVEL = " << zlibHeader.FLEVEL << std::endl;
			std::cout << "zlibHeader Window = " << zlibHeader.calcWindow() << std::endl;
			std::cout << "zlibHeader.DHB.BFINAL = " << std::boolalpha << (bool)zlibHeader.DHB.BFINAL << std::endl;
			std::cout << "zlibHeader.DHB.BTYPE = " << (int)zlibHeader.DHB.BTYPE << std::endl;
			
		}
		else{
			std::cout << "signature nok" << std::endl;
		}
	}
	else{
		std::cout << "Wrong file path" << std::endl;
		pic.clear();
		filePath = "";	
	}
}

void PNGpicture::chunkingF(){	
	if(!pic.fail() && (pic.tellg() == 8)){
		for(;pic.tellg() != EOF;){
			auto *pToChunk = new Chunk("temp");					//memory allocation for new chunk

			pToChunk->begOfChunk = pic.tellg();					//Position of chunk begining
			
			auto *pToIngridient = (char*)&pToChunk->dataLength;	//Length of DATA field
			for(int i = 3; i >= 0; --i){						//BigEndian => Little endian handshake
				pic.read(pToIngridient + i, 1);
			}
			
			pic.read((char*)&pToChunk->chunkType[0], 4);		//chunk type
			pToChunk->criticalChunk = (pToChunk->chunkType[0] > 96 ? false : true);
			
			pToChunk->chunkDataPos = pic.tellg();				//begining of DATA area
			
			pic.seekg(pToChunk->dataLength + 4, std::ios_base::cur);//jump to end of the chunk (=begining next chunk)
			
			pToChunk->chunkLenght = (pic.tellg() - pToChunk->begOfChunk);	//absolute length of chunk
			
			fileChunks.push_back(pToChunk);						//Ready chunk pushback to vector with chunks address
			
			if(pToChunk->chunkType != "IEND")	continue;		//IEND is last chunk of file
			else	break;		 
		}	
										
		pic.clear();											//after above for() there will be an error in stream
	}
}

void PNGpicture::getSettings(){
	pic.seekg(fileChunks[0]->chunkDataPos, std::ios_base::beg);	//chunk [0] is IHDR always
	
//IHDR INFO	
	auto *pToDim = (char*)&picSettings.width;		//BigEndian => Little endian handshake
	for(int i = 3; i >= 0; --i){
		pic.read(pToDim + i, 1);
	}
	
	pToDim = (char*)&picSettings.height;			//BigEndian => Little endian handshake
	for(int i = 3; i >= 0; --i){
		pic.read(pToDim + i, 1);
	}
	//pic.read((char*)&picSettings.width, 4);		//Done by code above - byte swap necessary
	//pic.read((char*)&picSettings.height, 4);		//Done by code above - byte swap necessary
	pic.read((char*)&picSettings.bitDepth, 1);
	pic.read((char*)&picSettings.colorType, 1);
	pic.read((char*)&picSettings.compressionMeth, 1);
	pic.read((char*)&picSettings.filterMeth, 1);
	pic.read((char*)&picSettings.intelaceMeth, 1);
}

void PNGpicture::extractZlibHeader(){
	unsigned int pos {};
	for(; pos <= fileChunks.size(); ++pos){			//Find first IDAT block (it contains zlib header)
		if(fileChunks[pos]->chunkType != "IDAT")	continue;		
		else	break;
		
		//if(pos == fileChunks.size()) throw std::out_of_range("IDAT chunk not found");
	}
	
	pic.seekg(fileChunks[pos]->chunkDataPos, std::ios_base::beg);	//Set position
	pic.read((char*)&zlibHeader, 3);				//and get zlib Header + data header bits
	
}

//HELP FUNCTION
void PNGpicture::showChunks(){
	for(int i = 0; i < fileChunks.size(); ++i){
		std::cout << "\nfileChunks[" << i << "]->begOfChunk = " << fileChunks[i]->begOfChunk << std::endl;
		std::cout << "fileChunks[" << i << "]->chunkLenght = " << fileChunks[i]->chunkLenght << std::endl;
		std::cout << "fileChunks[" << i << "]->criticalChunk = " << fileChunks[i]->criticalChunk << std::endl;
		std::cout << "fileChunks[" << i << "]->dataLength = " << fileChunks[i]->dataLength << std::endl;
		std::cout << "fileChunks[" << i << "]->chunkType = " << fileChunks[i]->chunkType << std::endl;
		std::cout << "fileChunks[" << i << "]->chunkDataPos = " << fileChunks[i]->chunkDataPos << std::endl;
	}
}

void PNGpicture::showChunkData(int inList){
	Chunk chunkTemp = *fileChunks[inList];
	
	pic.seekg(chunkTemp.chunkDataPos, std::ios_base::beg);
	for(int i = chunkTemp.chunkDataPos; i < (static_cast<int>(chunkTemp.chunkDataPos) + chunkTemp.dataLength); ++i){
		int temp = pic.get();
		std::cout << "IDAT byte[" << i << "] = " /*<< std::hex*/ << temp << std::endl;
	}
}
