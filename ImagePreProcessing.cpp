/// @file pa09a.cpp
/// @author Kiana Partovi 
/// <kiana.partovi@csn.edu>
/// @date 10/30/2021
/// @note I pledge my word of honor that I have complied with the
/// CSN Academic Integrity Policy while completing this assignment.
/// @brief This program will read an image file into memory and will copy it
/// to a output image file. This is part 1.
/// @note Time: 4 hours

#include <fstream>
#include <iostream>

// define a pixel struct

struct Pixel {
    unsigned char red;    // red   intensity [0,256)  // range of a char
    unsigned char green;  // green intensity [0,256)
    unsigned char blue;   // blue  intensity [0,256)
};

// define an image struct

struct Image {
    unsigned int width;
    unsigned int height;
    unsigned int max_colors;
    Pixel data[1024][1024];  // all of the pixels used to represent the image
                        // the largest possible picture our array can handle
};

// function prototypes

// reads an RGB color triplet into a Pixel from an input file
void read_pixel(std::ifstream& infile, Pixel& pixel);
// writes a Pixel's RGB components to an ouput file stream
void write_pixel(std::ofstream& outfile, const Pixel& pixel);
// reads a P3 PPM image from the input file stream
bool read_image(std::ifstream& infile, Image& image);
// writes a P3 PPM image to the output file stream.
void write_image(std::ofstream& outfile, const Image& image);

int main(int argc, const char *argv[]) {  // main function

    if (argc == 2) {  // indexes for first and second arguments

        Image image;  // variable declaration for image

        std::ifstream infile;  // opens the file
        std::ofstream outfile;  // outputs file

        infile.open(argv[1]);  // opens the file given the second argument
                               // of the command line arguments
        outfile.open("output.ppm");  // use output.ppm as the output file name

        // if the image file cannot be opened, then the program reports this
        if (infile.good() && outfile.good()) {
            // call the read function
            if (read_image(infile,image)) {

                // calling the write image function
                write_image(outfile, image);

        }
        } else {
            std::cout << "The image file cannot be opened." << std::endl;
        }
        infile.close();   // close infile
        outfile.close();  // close outfile
    }

    return 0;
}

// other functions

/// Reads an RGB color triplet into a Pixel from an input file.
/// @param infile The file we are opening up and reading from.
/// @param pixel Is referencing the Pixel structure.

void read_pixel(std::ifstream& infile, Pixel& pixel) {
    int red;    // integer for red
    int green;  // integer for green
    int blue;   // integer for blue

    // read into pixel, read one pixel, store into the image strucutre later on
    infile >> red >> green >> blue;  // reading in the data
    // static casting to get correct values
    pixel.red = static_cast<unsigned char>(red);
    pixel.green = static_cast<unsigned char>(green);
    pixel.blue = static_cast<unsigned char>(blue);
}

/// Writes a Pixel's RGB components to an output file stream.
/// Each value is followed by whitespace.
/// @param outfile The file we are readint out to.
/// @param pixel Is referencing the Pixel structure.

void write_pixel(std::ofstream& outfile, const Pixel& pixel) {

    // writes the pixel colors converts values
    outfile << static_cast<int>(pixel.red) << ' '
            << static_cast<int>(pixel.green) << ' '
            << static_cast<int>(pixel.blue) << ' ';

}

/// Reads a P3 PPM image from the input file stream.
/// @returns true if image is read successfully and is of correct size.
/// @param infile The file we are opening up and reading from.
/// @param image Is referencing the Image structure.

bool read_image(std::ifstream& infile, Image& image) {

    bool check = false;  // initializing to false

    // using pixel and image structures to read into and assign data
    // needs the P3 and newline to work
    if (infile.get() == 'P' && infile.get() == '3' && infile.get() == '\n') {
        // if there is a hash ignore up to newline
        while (infile.peek() == '#') {
            infile.ignore(100, '\n');
        }

        // reading in the width and height
        infile >> image.width >> image.height;

        // checking for whitespace and comments, if there is any ignore
        while (isspace(infile.peek())) {
            infile.ignore();
        }

        if (infile.peek() == '#') {
            infile.ignore(100, '\n');
        }

        infile >> image.max_colors;  // reading in the max colors

        // checking for whitespace and comments, if there is any ignore
        while (isspace(infile.peek())) {
            infile.ignore();
        }

        if (infile.peek() == '#') {
            infile.ignore(100, '\n');
        }

        //  1. image width is less than or equal to 1024
        //  2. height is less than or equal to 1024
        //  3. color is less than or equal to 255
        check = (image.width <= 1024 && image.height <= 1024
                 && image.max_colors <= 255);

        // if check = true, iterate through whole array by height and width 2D
        // array use read pixel function to read the infile for first paramater
        // and second paramater will be image.data and then the height
        // by the width. [h][w]
        if (check == true) {  // check is true
            // neccessary to acces the array
            for (unsigned int  h = 0; h < image.height; ++h) {
                for (unsigned int w = 0;  w < image.width; ++w) {
                    read_pixel(infile, image.data[h][w]);  // call the function
                    // set input width and height to new image height and width
                    // the data of height and width
                }
            }
        }
    }
        else {  // otherwise
            image.width = 0;  // everything is equal to 0
            image.height = 0;
            image.max_colors = 0;
            std::cout << "Error.";  // error
        }
        return check;  // returning function
    }
/// Writes a P3 PPM image to the output file stream.
/// The header is formatted as specified, and the author's name is
/// inserted as a comment after the magic number.
/// @param outfile The file we are readint out to.
/// @param image Is referencing the Image structure.

void write_image(std::ofstream& outfile, const Image& image) {
    // insert name and email address as a comment immediately following
    // the magic number in the output file
    outfile << "P3\n"
            << "# Kiana Partovi <kiana.partovi@csn.edu>\n"
            << "# Carlos Velasquez <8001217815@student.csn.edu>\n"
            << image.width << ' ' << image.height << std::endl
            << image.max_colors << std::endl;
    // step through the array
    for (unsigned int  h = 0; h < image.height; ++h) {
        for (unsigned int w = 0;  w < image.width; ++w) {
            // writing the pixels and calling the pixel array which is i.d
            write_pixel(outfile, image.data[h][w]);
        }
        outfile << '\n';  // to the outfile new line
    }
}
