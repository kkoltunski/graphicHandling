#ifndef ERRORSHANDLING_H
#define ERRORSHANDLING_H

#include <iostream>
#include <exception>
#include <string>

using std::string;

struct openingFileError : public std::exception
{
	openingFileError(const string _fileBeingOpened) {
		MSG = "File " + _fileBeingOpened + " can not be opened.";
	}
	
	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};

struct badFileExtension : public std::exception
{
	badFileExtension(const string _extensionFormat) {
		MSG = "Expected " + _extensionFormat + "  file format was not found.";
	}

	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};

struct unequalSignature : public std::exception
{
	unequalSignature(const string _readedSignature, const string _expectedSignature) {
		MSG = "Alleged format signature as number is " + _expectedSignature + ".\nReaded signature as number is ";
		MSG += _readedSignature + ".\nFile may be corrupted.";
	}

	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};

struct unknowPixelFormat : public std::exception
{
	unknowPixelFormat(const string _pixelSize) {
		MSG = "Examined size = " + _pixelSize + " is unsupported.";
	}

	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};

struct chunkIsNotExist : public std::exception
{
	chunkIsNotExist(string &&_chunkName) {
		MSG = "Chunk " + _chunkName + "is not exist. Do you want to see chunk overview?\n";
	}

	virtual const char* what() const noexcept override
	{
		return MSG.c_str();
	}

private:
	string MSG;
};
#endif