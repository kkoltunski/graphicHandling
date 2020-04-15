27.03.2020 v0.1
- software is able to work with .bmp pictures which are 24bit's (per pixel - other options will be developed in future 1bit / 8 bit / 16bit / 32bit);
- only available operation is negative (rest will be developed);
- there is also PNG format and software have to be developed about DEFLATE / INFLATE decode (will be done after .bmp file will be complete);
- future updates will contain information about changes.

29.3.2020 v0.2
-BMPpicture class is able to manage files with color tables (1/4/8/16/32 bpp);
-function BMPpicture::getColorIndex() is used for get positions of each index in color table;
-function BMPpicture::negative() was rebuilded for other options (8/16/32 bpp) as well (maybe not so elegant but simpliest and shortest what i came up with);
-function BMPpicture::makeHeader() was rebuilded to BMPpicture::makeFile() which will manage wholl action acording to new file creatiion operations (will be universal
	function called at begining of every operation);

14.04.2020 v0.3
-Code completely reviewed - added new variables, functions, some classes were rebuilded, some were moved to other files, errors handling were added

15.04.2020 v0.4
-Code were cleaned of stupid, unnecessary comments 
-some changes in names of functions / variables

TO_DO:
-more test
-compression / decompression algorithms for PNG format;
