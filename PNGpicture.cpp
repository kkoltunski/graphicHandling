#include "PNGpicture.h"

template<typename T>
void bigToLittle(T &_valToSwap) {					//Endians handshake
	T result{ 0x0000 };
	unsigned short ct = (sizeof(T));

	if (ct > 1) {
		for (int i = 0; i < ct; ++i) {
			result <<= 8;
			result += (_valToSwap & 0xFF);
			_valToSwap >>= 8;
		}
		_valToSwap = result;
	}
}

PNGpicture::PNGpicture(string _path) : Graphic{_path, ".png"}{
	signature = 727905341920923785;					//137 80 78 71 13 10 26 10 added as byte's

	if (checkFileExtension()) {
		if (validateSignature(8)) {
			partitioning();
			getSettings();
			extractZlibHeader();

			std::cout << "PNG PICTURE SETTINGS";
			std::cout << picSettings;
			std::cout << "\nzLib Header info\n";
			std::cout << zlibHeader;
		}
	}
}

PNGpicture::~PNGpicture() noexcept(true) {
	for(int i = 0; i < fileChunks.size(); ++i) delete fileChunks[i];
}

void PNGpicture::partitioning(){
	if(!pic.fail() && (pic.tellg() == 8)){
		for(;pic.tellg() != EOF;){
			auto *pToChunk = new Chunk("temp");					//memory allocation for new chunk

			pToChunk->begOfChunk = pic.tellg();					//Position of chunk begining
			
			pic.read((char*)&pToChunk->dataLength, 4);
			bigToLittle(pToChunk->dataLength);

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

void PNGpicture::getSettings() noexcept(true) {
	pic.seekg(fileChunks[0]->chunkDataPos, std::ios_base::beg);	//chunk [0] is IHDR always
	
//IHDR INFO	
	pic.read((char*)&picSettings.width, 4);
	bigToLittle(picSettings.width);

	pic.read((char*)&picSettings.height, 4);
	bigToLittle(picSettings.height);

	pic.read((char*)&picSettings.bitDepth, 1);
	pic.read((char*)&picSettings.colorType, 1);
	pic.read((char*)&picSettings.compressionMeth, 1);
	pic.read((char*)&picSettings.filterMeth, 1);
	pic.read((char*)&picSettings.intelaceMeth, 1);
}

void PNGpicture::extractZlibHeader() throw (chunkIsNotExist) {
	try {
		unsigned int pos{0};

		for (; pos <= fileChunks.size(); ++pos) {			//Find first IDAT block (it contains zlib header)
			if (fileChunks[pos]->chunkType != "IDAT")	continue;
			else	break;
		}
		if (pos == fileChunks.size()) throw (chunkIsNotExist("IDAT"));

		pic.seekg(fileChunks[pos]->chunkDataPos, std::ios_base::beg);	//Set position
		pic.read((char*)&zlibHeader, 3);				//and get zlib Header + data header bits
	}
	catch (chunkIsNotExist& Exception) {
		cout << Exception.what() << endl;
	}
	
}

std::ofstream& PNGpicture::negative(){
	return operationResult;
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

//Operator used to show IHDR chunk of PNG image
std::ostream& operator<<(std::ostream& outStream, IHDR& var) {
	outStream //<< "\nFile path : " << var.filePath
		<< "\nIHDR.width = " << var.width
		<< "\nIHDR.height = " << var.height
		<< "\nIHDR.bitDepth = " << static_cast<int>(var.bitDepth)
		<< "\nIHDR.colorType = " << static_cast<int>(var.colorType)
		<< "\nIHDR.compressionMeth = " << static_cast<int>(var.compressionMeth)
		<< "\nIHDR.filterMeth = " << static_cast<int>(var.filterMeth)
		<< "\nIHDR.intelaceMeth = " << static_cast<int>(var.intelaceMeth) << endl;

	return outStream;
}

//Operator used to show zlib header of IDAT chunk
std::ostream& operator<<(std::ostream& outStream, zlibHeader& zlibHeader) {
	outStream //<< "\nFile path : " << var.filePath
		<< "zlibHeader.CMF = " << zlibHeader.CMF << std::endl
		<< "zlibHeader.CINFO = " << zlibHeader.CINFO << std::endl
		<< "zlibHeader.FCHECK = " << zlibHeader.FCHECK << std::endl
		<< "zlibHeader.FDICT = " << zlibHeader.FDICT << std::endl
		<< "zlibHeader.FLEVEL = " << zlibHeader.FLEVEL << std::endl
		<< "zlibHeader Window = " << zlibHeader.calcWindow() << std::endl
		<< "zlibHeader.DHB.BFINAL = " << std::boolalpha << (bool)zlibHeader.DHB.BFINAL << std::endl
		<< "zlibHeader.DHB.BTYPE = " << (int)zlibHeader.DHB.BTYPE << std::endl;

	return outStream;
}
