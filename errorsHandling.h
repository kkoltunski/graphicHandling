/*****************************************************************//**
 * \file   errorsHandling.h
 * \brief  Exceptions structures.
 * 
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

#ifndef ERRORSHANDLING_H
#define ERRORSHANDLING_H

using std::string;

/**
 * Exception which is thrown while file opening is failed.
 */
struct openingFileError : public std::exception
{	
	/**
	 * Constructor which construct error message.
	 * \param [in] _fileBeingOpened Name of file.
	 */
	openingFileError(const string _fileBeingOpened) {
		MSG = "File " + _fileBeingOpened + " can not be opened.";
	}
	
	/**
	 * Method to show message.
	 * \return Error message.
	 */
	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};

/**
 * Exception which is thrown while extension format is not correct.
 */
struct badFileExtension : public std::exception
{	
	/**
	 * Constructor which construct error message.
	 * \param [in] _extensionFormat Expected extension.
	 */
	badFileExtension(const string _extensionFormat) {
		MSG = "Expected " + _extensionFormat + "  file format was not found.";
	}

	/**
	* Method to show message.
	* \return Error message.
	*/
	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};

/**
 * Exception which is thrown while readed signature is not equal to standard signature.
 */
struct unequalSignature : public std::exception
{
	/**
	 * Constructor which construct error message.
	 * \param [in] _readedSignature Readed signature.
	 * \param [in] _expectedSignature Standard signature.
	 */
	unequalSignature(const string _readedSignature, const string _expectedSignature) {
		MSG = "Alleged format signature as number is " + _expectedSignature + ".\nReaded signature as number is ";
		MSG += _readedSignature + ".\nFile may be corrupted.";
	}

	/**
	* Method to show message.
	* \return Error message.
	*/
	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};

/**
 * Exception which is thrown while readed pixel format is unknow.
 */
struct unknowPixelFormat : public std::exception
{
	/**
	 * Constructor which construct error message.
	 * \param [in] _pixelSize Single pixel size in picture (in bytes).
	 */
	unknowPixelFormat(const string _pixelSize) {
		MSG = "Examined size = " + _pixelSize + " is unsupported.";
	}

	/**
	 * Method to show message.
	* \return Error message.
	*/
	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};

/**
 * Exception which is thrown while expected chunk is not found.
 */
struct chunkIsNotExist : public std::exception
{
	/**
	 * Constructor which construct error message.
	 * \param [in] _chunkName Expected chunk name.
	 */
	chunkIsNotExist(string &&_chunkName) {
		MSG = "Chunk " + _chunkName + "is not exist. Do you want to see chunk overview?\n";
	}

	/**
	* Method to show message.
	* \return Error message.
	*/
	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};
#endif