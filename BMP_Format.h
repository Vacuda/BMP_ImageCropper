#pragma once
#include <vector>


#pragma pack(push, 1)			//this prevents the compiler from packing
class BMP_FileHeader {

public:

	uint16_t file_type{ 0x4D42 };          // File type always BM which is 0x4D42
	uint32_t file_size{ 0 };               // Size of the file (in bytes)
	uint16_t reserved1{ 0 };               // Reserved, always 0
	uint16_t reserved2{ 0 };               // Reserved, always 0
	uint32_t offset_data{ 0 };             // Start position of pixel data (bytes from the beginning of the file)
};

class BMP_ImageInfo {

public: 

	uint32_t size{ 0 };                      // Size of this header (in bytes)
	uint32_t width{ 0 };                     // width of bitmap in pixels
	uint32_t height{ 0 };                    // height of bitmap in pixels
	uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
	uint16_t bit_count{ 0 };                 // No. of bits per pixel
	uint32_t compression{ 0 };               // 0
	uint32_t size_image{ 0 };                // width x height
	int32_t x_pixels_per_meter{ 0 };
	int32_t y_pixels_per_meter{ 0 };
	uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
	uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required

};

class BMP_Format
{
	public:

	/* CONSTRUCTOR */
		BMP_Format(const char* bitmap_name);

	/* METHODS */
		void ReadAndLoad_Data(const char* bitmap_name);

	/* MEMBERS */

		const char* original_filename;
		bool bFileNotFound = false;

	/* OUTPUT MATERIAL */

		BMP_FileHeader bmp_fileheader;
		BMP_ImageInfo bmp_imageinfo;
		std::vector<uint8_t> colortable_data;
		std::vector<uint8_t> pixel_data;
};



