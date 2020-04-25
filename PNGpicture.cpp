/*****************************************************************//**
 * \file   PNGpicture.cpp
 * \brief  Methods definitions of PNGpicture class.
 * 
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

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

/**
 * Is defining \ref formatSignature, checking if orginal file is correct and extracting picture metadata which is shown at the end.
 * \param [in] _path Path to orginal file.
 */
PNGpicture::PNGpicture(string _path) : Graphic{_path, ".png"}{
	formatSignature = static_cast<Tulli>(pngSignature);					//137 80 78 71 13 10 26 10 added as byte's

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

/**
 * Deleting stored chunks.
 */
PNGpicture::~PNGpicture() noexcept(true) {
	for(int i = 0; i < fileChunks.size(); ++i) delete fileChunks[i];
}

/**
 * Method used to extract each chunk information and stored its address.
 */
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

/**
 * Method used to extract metadata from orginal picture.
 */
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

/**
 * Method searching for first "IDAT" chunk and extracting zlibHeader from it
 * \throw chunkIsNotExist Exception is thrown when "IDAT" chunk is not found.
 */
void PNGpicture::extractZlibHeader() noexcept(false) {
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

/**
 * Method used as algorithm to reverse picture colors to negative.
 * \return Reference to stream opened as result picture.
 */
std::ofstream& PNGpicture::negative(){
	return operationResultPicture;
}

/**
 * Help method used to show extracted chunks informations.
 */
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

/**
 * Method used to show chunk data in chunk specified as _chunkIndex. 
 * \param [in] _chunkIndex Index to chunk.
 */
void PNGpicture::showChunkData(int _chunkIndex){
	Chunk chunkTemp = *fileChunks[_chunkIndex];
	
	originalPicture.seekg(chunkTemp.dataFieldBegining, std::ios_base::beg);
	for(int i = chunkTemp.dataFieldBegining; i < (static_cast<int>(chunkTemp.dataFieldBegining) + chunkTemp.dataFieldLength); ++i){
		int temp = originalPicture.get();
		std::cout << "IDAT byte[" << i << "] = " /*<< std::hex*/ << temp << std::endl;
	}
}

/**
 * Overloaded operator to show picture IHDR informations.
 * \param [in&] outStream Reference to stream used as output.
 * \param [in&] var Reference to imgHeader informations.
 * \return Reference to used stream.
 */
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

/**
 * Overloaded operator to show picture zlibHeader informations.
 * \param [in&] outStream Reference to stream used as output.
 * \param [in&] var Reference to imgHeader informations.
 * \return Reference to used stream.
 */
std::ostream& operator<<(std::ostream& outStream, zlibHeader& var) {
	outStream << "zlibHeader.CMF = " << var.CMF << std::endl
		<< "zlibHeader.CINFO = " << var.CINFO << std::endl
		<< "zlibHeader.FCHECK = " << var.FCHECK << std::endl
		<< "zlibHeader.FDICT = " << var.FDICT << std::endl
		<< "zlibHeader.FLEVEL = " << var.FLEVEL << std::endl
		<< "zlibHeader Window = " << var.calcWindow() << std::endl
		<< "zlibHeader.DHB.BFINAL = " << std::boolalpha << static_cast<bool>(var.DHB.BFINAL) << std::endl
		<< "zlibHeader.DHB.BTYPE = " << static_cast<int>(var.DHB.BTYPE) << std::endl;

	return outStream;
}
