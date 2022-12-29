#pragma once

#include "BMP_Format.h"

class BMP_CropTool
{
	public:

		static void Crop_Image(BMP_Format* bmp);
		static void Output_Image(BMP_Format* bmp);
		static void Edit_Metadata(BMP_Format* bmp, int new_width, int new_height);
};

