*****************
BMP_ImageCropper
*****************

This commandline tool takes in an 8-bit .bmp file and outputs a new .bmp file, cropping off any transparent edges, 
reducing the image to its smallest bounding rectangle.


//store bmp info

It reads and stores the .bmp file information into the BMP_Format class. The BMP format consists of a fileheader, 
image info, colortable data, and pixel data.  Each pixel is represented by a byte, written in hexadecimal.  Each row 
of pixels is padded to be a multiple of 4 bytes in size.

//analyze info

It analyzes the information to determine each edge of the image with the first untransparent pixel.  This takes into 
account possible padding due to the 4 byte padding restriction.

//produce new image

Then, it uses that information to build a new .bmp file.  This file may need padding added to it. It copies the 
relevant data from the original file into the resulting image.  It also correctly modifies the metadata to produce a 
valid .bmp file.

