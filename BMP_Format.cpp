#include <iostream>
#include <fstream>

#include "BMP_Format.h"

//constructor
BMP_Format::BMP_Format(const char* bitmap_name) 
{
	//read and emulate data
	ReadAndLoad_Data(bitmap_name);

	//store original name
	original_filename = bitmap_name;
}

void BMP_Format::ReadAndLoad_Data(const char* bitmap_name) 
{
	//attempt to find information
	std::ifstream input (bitmap_name, std::ios_base::binary );

	//if file found
	if (input) {

		//store metadata
		{
			//read data.  place all info from the first part of the data into bmp_fileheader, to the size of the header(14 bytes)
			input.read((char*)&bmp_fileheader, 14);

			//format check
			if (bmp_fileheader.file_type != 0x4D42) {
				std::cout << "--wrong file format--" << std::endl;
			}
			else {
				std::cout << "--bmp recognized--" << std::endl;
			}

			/* The read() function must keep its place */

			//read data, from the point of above, for another length(40 bytes), store in bmp_imageinfo
			input.read((char*)&bmp_imageinfo, 40);

			//get length of colortable, (all metadata - header - info)
			size_t bitlength_of_colortable = (bmp_fileheader.offset_data - 14 - 40);

			//resize color_data to proper length
			colortable_data.resize(bitlength_of_colortable);

			//read data, from the point of above, for as long as the colortable data is
			input.read((char*)colortable_data.data(), colortable_data.size());
		}

		//store pixel data
		{
			/* BMPs are stored with a multiple of 4 bytes in each row */
			/* If the current width is not a multiple of 4, the image will have padding to increase it to that multiple*/

			//initialize possible padding at end of each row
			int possible_padding = 4 - bmp_imageinfo.width % 4;

			//possible padding correction
			if (possible_padding == 4) { possible_padding = 0; }

			//beg goes back to begining of input, seekg moves up to the point in data referenced by the offset_data property of file
			input.seekg(bmp_fileheader.offset_data, input.beg);

			//resize data vector to be as large as pixel data of bmp
			pixel_data.resize((bmp_imageinfo.width + possible_padding) * bmp_imageinfo.height);

			//read data, place into pixel data portion of file
			input.read((char*)pixel_data.data(), pixel_data.size());
		}

	}
	// NOT found
	else {
		std::cout << "Did Not Read File\n";

		//set flag
		bFileNotFound = true;
	}
}