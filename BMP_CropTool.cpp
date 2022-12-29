#include "BMP_CropTool.h"
#include <iostream>
#include <fstream>

void BMP_CropTool::Crop_Image(BMP_Format* bmp)
{
	//initialize goals
	int leftmost = -1;
	int rightmost = -1;
	int bottommost = -1;
	int topmost = -1;

	//find boundaries
	{
		//condense info
		int height = bmp->bmp_imageinfo.height; 
		int width = bmp->bmp_imageinfo.width;	
		int old_padding = 4 - width % 4;		
												
		//possible padding correction
		if (old_padding == 4) { old_padding = 0; }

		//set initial trigger
		bool bIsInitialHit = true;
		bool bTrigger_RowChange_AccordingToPadding = false;
		int skip_counter = 0;

		//start runners
		int current_row = 0;
		int current_col = 0;

		//loop pixels
		for (auto& pixel : bmp->pixel_data) {

			//if rowchange triggered
			if (bTrigger_RowChange_AccordingToPadding) {

				//maybe skip some filler pixel data
				if (skip_counter > 0) {

					//decrement counter
					skip_counter--;

					//skip this filler pixel
					continue;
				}

				/* No more skipping needed.  Change Row */

				//reset
				{
					//increment row
					current_row++;

					//reset col
					current_col = 0;

					//reset trigger
					bTrigger_RowChange_AccordingToPadding = false;
				}
			}

			//if pixel is not magenta
			if (pixel != 0x00) {

				//initial hit
				if (bIsInitialHit) {

					//set initial values
					leftmost = current_col;
					rightmost = current_col;
					bottommost = current_row;
					topmost = current_row;

					//toggle trigger
					bIsInitialHit = false;
				}
				//NOT initial
				else {

					//if more to the left
					if (current_col < leftmost) {
						leftmost = current_col;
					}

					//if more to the right
					if (current_col > rightmost) {
						rightmost = current_col;
					}

					//after initial, always set topmost
					topmost = current_row;
				}
			}

			//increment col runner
			current_col++;

			//last row check
			if (current_col >= width) {

				//trigger inevitable rowchange
				bTrigger_RowChange_AccordingToPadding = true;

				//add padding to skip_counter
				skip_counter += old_padding;
			}
		}
	}

	//condense
	int old_height = bmp->bmp_imageinfo.height;
	int old_width = bmp->bmp_imageinfo.width;
	int old_total = old_height * old_width;

	//find new width and height
	int new_width = rightmost - leftmost + 1;
	int new_height = topmost - bottommost + 1;

	//find possible padding
	int new_padding = 4 - new_width % 4;
	int old_padding = 4 - old_width % 4;

	//possible padding correction
	if (old_padding == 4) { old_padding = 0; }
	if (new_padding == 4) { new_padding = 0; }

	//create new vector to store new data
	std::vector<uint8_t> new_pixel_data;

	//resize, so it doesn't have to be done more than once
	new_pixel_data.resize((new_width + new_padding) * new_height);

	/* I will loop the original data less than once, placing relevant items into new array */

	//find different skippers, taking into account old padding
	int bottom_skip = (old_width + old_padding) * bottommost;
	int left_skip = leftmost;
	int right_skip = old_width - rightmost - 1;

	//create index runners
	int i = 0;	//index
	int ni = 0;	//new_index

	//We can skip the width * bottommost first elements
	i += bottom_skip;	//old data

	/* Now, we can loop the old data, but we need to skip the left and the right portions */

	//loop as many times as new height
	for (int r = 1; r <= new_height; r++) {

		//skip the left portion
		i += left_skip;	//old data

		/* Now, we can loop old data for the new data vector */
		{
			//loop columns of new_width
			for (int c = 1; c <= new_width; c++) {

				//set into new data vector
				new_pixel_data[ni] = bmp->pixel_data[i];

				//increment both indexes
				i++;	//old data
				ni++;
			}

			/* We've added all the relevant pixel data for this row */

			//possibly add padding
			for (int j = 1; j <= new_padding; j++) {
				
				//add magenta pixel
				new_pixel_data[ni] = 0x00;

				//increment both indexes
				ni++;
			}

			//possible increment old data index
			i += old_padding; //old data
		}

		//skip the right portion
		i += right_skip;	//old data
	}

	/* We are done, because the top portion is skipped entirely */

	//replace old data with new data on bmp
	bmp->pixel_data = new_pixel_data;

	//edit metadata
	BMP_CropTool::Edit_Metadata(bmp, new_width, new_height);
}

void BMP_CropTool::Edit_Metadata(BMP_Format* bmp, int new_width, int new_height)
{
	//change filesize
	{
		//add filesize
		int filesize = sizeof(bmp->bmp_fileheader) 
			+ sizeof(bmp->bmp_imageinfo) 
			+ sizeof(uint8_t) * bmp->colortable_data.size()
			+ sizeof(uint8_t) * bmp->pixel_data.size();

		//set filesize
		bmp->bmp_fileheader.file_size = filesize;
	}

	//change width and height
	bmp->bmp_imageinfo.width = new_width;
	bmp->bmp_imageinfo.height = new_height;
	bmp->bmp_imageinfo.size_image = new_width * new_height;
}

void BMP_CropTool::Output_Image(BMP_Format* bmp)
{
	std::cout << "--outputting--" << std::endl;

	//get old string name
	std::string old_name = std::string(bmp->original_filename);

	//remove .bmp
	old_name.erase(old_name.length() - 4, 4);

	//build new string name
	std::string string_name = old_name + "_cropped.bmp";

	//convert name
	const char* new_name = string_name.c_str();

	//make new file with new name
	std::ofstream output(new_name, std::ios_base::binary);

	//write out relevant sections
	output.write((const char*)&bmp->bmp_fileheader, sizeof(bmp->bmp_fileheader));
	output.write((const char*)&bmp->bmp_imageinfo, sizeof(bmp->bmp_imageinfo));
	output.write((const char*)bmp->colortable_data.data(), bmp->colortable_data.size());
	output.write((const char*)bmp->pixel_data.data(), bmp->pixel_data.size());
}
