#include "PNGpicture.h"

template<typename T>
void littleEndianToBigEndian(T& _valToSwap) {
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
	formatSignature = 727905341920923785;					//137 80 78 71 13 10 26 10 added as byte's

	if (checkFileExtensionOnFilePath()) {
		if (isFileSignatureValid(8)) {
			filePartitioning();
			getMetadata();
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

void PNGpicture::filePartitioning(){
	if(!originalPicture.fail() && (originalPicture.tellg() == 8)){
		for(;originalPicture.tellg() != EOF;){
			auto *pToChunk = new Chunk("temp");

			pToChunk->begining = originalPicture.tellg();
			
			originalPicture.read((char*)&pToChunk->dataFieldLength, 4);
			littleEndianToBigEndian(pToChunk->dataFieldLength);

			originalPicture.read((char*)&pToChunk->type[0], 4);
			pToChunk->criticalChunk = (pToChunk->type[0] > 96 ? false : true);
			
			pToChunk->dataFieldBegining = originalPicture.tellg();
			
			originalPicture.seekg(pToChunk->dataFieldLength + 4, std::ios_base::cur);//jump to end of the chunk (=begining next chunk)
			
			pToChunk->chunkLenght = (originalPicture.tellg() - pToChunk->begining);
			
			fileChunks.push_back(pToChunk);

			if(pToChunk->type != "IEND")	continue;		//IEND is last chunk of file
			else	break;		 
		}	
										
		originalPicture.clear();
	}
}

void PNGpicture::getMetadata() noexcept(true) {
	originalPicture.seekg(fileChunks[0]->dataFieldBegining, std::ios_base::beg);	//chunk [0] is IHDR always
	
//IHDR INFO	
	originalPicture.read((char*)&picSettings.width, 4);
	littleEndianToBigEndian(picSettings.width);

	originalPicture.read((char*)&picSettings.height, 4);
	littleEndianToBigEndian(picSettings.height);

	originalPicture.read((char*)&picSettings.bitDepth, 1);
	originalPicture.read((char*)&picSettings.colorType, 1);
	originalPicture.read((char*)&picSettings.compressionMeth, 1);
	originalPicture.read((char*)&picSettings.filterMeth, 1);
	originalPicture.read((char*)&picSettings.intelaceMeth, 1);
}

void PNGpicture::extractZlibHeader() throw (chunkIsNotExist) {
	try {
		unsigned int pos{0};

		for (; pos <= fileChunks.size(); ++pos) {
			if (fileChunks[pos]->type != "IDAT")	continue;
			else	break;
		}
		if (pos == fileChunks.size()) throw (chunkIsNotExist("IDAT"));

		originalPicture.seekg(fileChunks[pos]->dataFieldBegining, std::ios_base::beg);
		originalPicture.read((char*)&zlibHeader, 3);
	}
	catch (chunkIsNotExist& Exception) {
		cout << Exception.what() << endl;
	}
	
}

std::ofstream& PNGpicture::negative(){
	return operationResultPicture;
}

//HELP FUNCTION
void PNGpicture::showChunks(){
	for(int i = 0; i < fileChunks.size(); ++i){
		std::cout << "\nfileChunks[" << i << "]->begOfChunk = " << fileChunks[i]->begining << std::endl;
		std::cout << "fileChunks[" << i << "]->chunkLenght = " << fileChunks[i]->chunkLenght << std::endl;
		std::cout << "fileChunks[" << i << "]->criticalChunk = " << fileChunks[i]->criticalChunk << std::endl;
		std::cout << "fileChunks[" << i << "]->dataLength = " << fileChunks[i]->dataFieldLength << std::endl;
		std::cout << "fileChunks[" << i << "]->chunkType = " << fileChunks[i]->type << std::endl;
		std::cout << "fileChunks[" << i << "]->chunkDataPos = " << fileChunks[i]->dataFieldBegining << std::endl;
	}
}

void PNGpicture::showChunkData(int inList){
	Chunk chunkTemp = *fileChunks[inList];
	
	originalPicture.seekg(chunkTemp.dataFieldBegining, std::ios_base::beg);
	for(int i = chunkTemp.dataFieldBegining; i < (static_cast<int>(chunkTemp.dataFieldBegining) + chunkTemp.dataFieldLength); ++i){
		int temp = originalPicture.get();
		std::cout << "IDAT byte[" << i << "] = " /*<< std::hex*/ << temp << std::endl;
	}
}

std::ostream& operator<<(std::ostream& outStream, IHDR& var) {
	outStream << "\nIHDR.width = " << var.width
		<< "\nIHDR.height = " << var.height
		<< "\nIHDR.bitDepth = " << static_cast<int>(var.bitDepth)
		<< "\nIHDR.colorType = " << static_cast<int>(var.colorType)
		<< "\nIHDR.compressionMeth = " << static_cast<int>(var.compressionMeth)
		<< "\nIHDR.filterMeth = " << static_cast<int>(var.filterMeth)
		<< "\nIHDR.intelaceMeth = " << static_cast<int>(var.intelaceMeth) << endl;

	return outStream;
}

std::ostream& operator<<(std::ostream& outStream, zlibHeader& zlibHeader) {
	outStream << "zlibHeader.CMF = " << zlibHeader.CMF << std::endl
		<< "zlibHeader.CINFO = " << zlibHeader.CINFO << std::endl
		<< "zlibHeader.FCHECK = " << zlibHeader.FCHECK << std::endl
		<< "zlibHeader.FDICT = " << zlibHeader.FDICT << std::endl
		<< "zlibHeader.FLEVEL = " << zlibHeader.FLEVEL << std::endl
		<< "zlibHeader Window = " << zlibHeader.calcWindow() << std::endl
		<< "zlibHeader.DHB.BFINAL = " << std::boolalpha << (bool)zlibHeader.DHB.BFINAL << std::endl
		<< "zlibHeader.DHB.BTYPE = " << (int)zlibHeader.DHB.BTYPE << std::endl;

	return outStream;
}
