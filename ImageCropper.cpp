
#include <iostream>
#include "BMP_Format.h"
#include "BMP_CropTool.h"

//declarations
void Display_Intro();
void GetInput_AndCropImage();
void Display_Success();
void Display_NoSuccess();
void Get_BitmapName(std::string& input);


/* MAIN */
int main()
{
    Display_Intro();
    GetInput_AndCropImage();

    return 0;
}

void Display_Intro() 
{
    std::cout << "*^*^*^*^*^*^*^*^*^" << std::endl;
    std::cout << "Image Cropper Tool" << std::endl;
    std::cout << "*^*^*^*^*^*^*^*^*^" << std::endl;
    std::cout << std::endl;
    std::cout << "Write the name of the .bmp file to crop: ";
}

void GetInput_AndCropImage()
{
    //get bitmap_name
    std::string input;
    Get_BitmapName(input);

    //convert
    const char* bitmap_name = input.c_str();

    //create bmp format from bitmap_name
    BMP_Format* bmp = new BMP_Format(bitmap_name);

    //bmp not found
    if (bmp->bFileNotFound) {

        //display outro
        Display_NoSuccess();
    }
    else {

        //crop image
        BMP_CropTool::Crop_Image(bmp);

        //output image
        BMP_CropTool::Output_Image(bmp);

        //display outro
        Display_Success();
    }
}

void Get_BitmapName(std::string& input) 
{
    //get inputted string
    std::cin >> input;

    std::cout << std::endl;
    std::cout << "Thank you. Processing..." << std::endl;
    std::cout << std::endl;
}

void Display_Success()
{
    std::cout << std::endl;
    std::cout << "#######" << std::endl;
    std::cout << "SUCCESS" << std::endl;
    std::cout << "#######" << std::endl;
}

void Display_NoSuccess()
{
    std::cout << std::endl;
    std::cout << "~ ~ ~ ~ ~ ~ ~" << std::endl;
    std::cout << "UNSUCCESSFUL!" << std::endl;
    std::cout << "~ ~ ~ ~ ~ ~ ~" << std::endl;
}

