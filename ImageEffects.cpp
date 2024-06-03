/// @file pa09b.cpp
/// @author Kiana Partovi
/// <kiana.partovi@csn.edu>
/// @date 12/13/21
/// @note I pledge my word of honor that I have complied with the CSN
/// Academic Integrity Policy while completing this assignment.
/// @brief This program will take a simple image and apply effects to it.
/// @note Time taken to develop, write, test, and debug solution 6 hours

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using Kernel = float[3][3];  // kernel definition
static const Kernel SHARPEN = {  // sharpen struct
    {  0, -1,  0 },
    { -1,  5, -1 },
    {  0, -1,  0 }
};
static const Kernel EMBOSS = {  // emboss struct
    { -2, -1,  0 },
    { -1,  1,  1 },
    {  0,  1,  2 }
};
static const Kernel BLUR = {  // blur struct
    { 1.0F / 9, 1.0F / 9, 1.0F / 9 },
    { 1.0F / 9, 1.0F / 9, 1.0F / 9 },
    { 1.0F / 9, 1.0F / 9, 1.0F / 9 }
};
static const Kernel EDGE = {  // edge struct
    { -1,  0,  1 },
    { -1,  0,  1 },
    { -1,  0,  1 }
};

struct Pixel {  // pixel struct
    unsigned char red;    // red   intensity [0,256)
    unsigned char green;  // green intensity [0,256)
    unsigned char blue;   // blue  intensity [0,256)
};

struct Image {  // image struct
    unsigned int width;
    unsigned int height;
    unsigned int max_colors;
    Pixel data[1024 + 2][1024 + 2];  // array for data of pixel type
};

// reads an RGB color triplet into a Pixel from an input file
void read_pixel(std::ifstream& infile, Pixel& pixel);
// writes a Pixel's RGB components to an ouput file stream
void write_pixel(std::ofstream& outfile, const Pixel& pixel);
// reads a P3 PPM image from the input file stream
bool read_image(std::ifstream& infile, Image& image);
// writes a P3 PPM image to the output file stream.
void write_image(std::ofstream& outfile, const Image& image);
// changes the picture into a black and white image
void grayscale(Image& image);
// chnages every pixel in an image with its complementary color
// by changing each color to its negative
void negative(Image& image);
// will reverse the image in the horizontal direction
void flop(Image& image);
// will flip the image in the vertical direction
void flip(Image& image);
// the convolute function will take a kernel, a small matrix, and accomplish
// other small tasks like blurring, sharpenining, embossing, edge detection,
// etc.
void convolute(Image& image, const Kernel& kernel);
// Swaps the pixel a with the pixel b.
void swap_pixels(Pixel &a, Pixel &b);
// Outputs the photo editing options.
void edit_options();
// the max of pixels for a and b
float max(const float& a, const float& b);
// the min of pixels for a and b
float min(const float& a, const float& b);

int main(int argc, const char *argv[]) {

    if (argc == 3) {

        Image image;  // variable declaration for image

        ifstream infile;  // opens the file
        ofstream outfile;  // outputs file

        // arguments to be opened for editing
        string arg1 = argv[1];
        string arg2 = argv[2];
        string edit_args;

        if (arg1[0] == '-') {
            infile.open(argv[2]);  // opens the file given the second argument
                                   // of the command line arguments
            edit_args = argv[1];  // opens the file given the first
                                  // argument of the command line

        }
        else if (arg2[0] == '-') {  // other option for command line arguments
            infile.open(argv[1]);
            edit_args = argv[2];
        }
        outfile.open("output.ppm");  // use output.ppm as the output file name

        // if the image file cannot be opened, then the program reports this
        if (infile.good() && outfile.good()) {
            // call the read function
            if (read_image(infile, image)) {
                // for the edit arguments goes up until the length
                for (string::size_type i = 0; i < edit_args.length(); ++i) {
                    switch (edit_args[i]) {

                        case '-': break;  // type in - case breaks

                        case 'o': edit_options();  // type in o for the options
                                  break;

                        case 'g': grayscale(image);
                                  break;  // type in g, then image will b be
                                          // grayscaled

                        case 'n': negative(image);  // n will negate the image
                                  break;

                        case 'f': flop(image);  // f will flop the image
                                  break;        // in the horizontal direction

                        case 'v': flip(image);  // v will flip the image
                                  break;        // in the vertical direction

                        case 'm': convolute(image, EMBOSS);
                                  break;    // case m calls the convoulte
                                            // function w/ emboss kernel

                        case 's': convolute(image, SHARPEN);
                                  break;  // case s calls the convolute
                                          // function w/ sharpen kernel

                        case 'b': convolute(image, BLUR);
                                  break;  // case b calls the convolute
                                          // function w/ blur kernel

                        case 'e': convolute(image, EDGE);
                                  break;  // case e calls the convolute
                                          // function w/ edge kernel

                        default: break;  // default statement automatically
                                         // breaks


                    }
                }
                // calling the write image function
                write_image(outfile, image);
            }
        }
        else {
            cout << "The image file cannot be opened." << endl;
        }
        infile.close();   // close infile
        outfile.close();  // close outfile
    }

    return 0;
}

/// Reads an RGB color triplet into a Pixel from an input file.
/// @param infile The file we are opening up and reading from.
/// @param pixel Is referencing the Pixel structure.

void read_pixel(ifstream& infile, Pixel& pixel) {
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

void write_pixel(ofstream& outfile, const Pixel& pixel) {

    // writes the pixel colors converts values
    outfile << static_cast<int>(pixel.red) << ' '
            << static_cast<int>(pixel.green) << ' '
            << static_cast<int>(pixel.blue) << ' ';

}

/// Reads a P3 PPM image from the input file stream.
/// @returns true if image is read successfully and is of correct size.
/// @param infile The file we are opening up and reading from.
/// @param image Is referencing the Image structure.

bool read_image(ifstream& infile, Image& image) {

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
            for (unsigned int  h = 1; h < image.height + 1; ++h) {
                for (unsigned int w = 1;  w < image.width + 1; ++w) {
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
        cout << "Error.";  // error
    }
        return check;  // returning function
}

/// Writes a P3 PPM image to the output file stream.
/// The header is formatted as specified, and the author's name is
/// inserted as a comment after the magic number.
/// @param outfile The file we are readint out to.
/// @param image Is referencing the Image structure.

void write_image(ofstream& outfile, const Image& image) {
    // insert name and email address as a comment immediately following
    // the magic number in the output file
    outfile << "P3\n"
            << "# Kiana Partovi <kiana.partovi@csn.edu>\n"
            << "# Carlos Velasquez <8001217815@student.csn.edu>\n"
            << image.width << ' ' << image.height << endl
            << image.max_colors << endl;
    // step through the array
    for (unsigned int  h = 1; h < image.height + 1; ++h) {
        for (unsigned int w = 1;  w < image.width + 1; ++w) {
            // writing the pixels and calling the pixel array which is i.d
            write_pixel(outfile, image.data[h][w]);
        }
        outfile << '\n';  // to the outfile new line
    }
}

// functions for image effects

/// @brief Function swap_element swaps 2 elements in an array or more generally
/// swaps parameter a with parameter b.
/// @param b An integer
/// @param a An integer

void swap_pixels(Pixel& a, Pixel& b) {

    Pixel temp;  // declaring a temp varaible for the pixel structure
    temp = a;    // assigning a to temp
    a = b;       // assigning b to temp
    b = temp;    // swaps temp to now equal b
}

/// The convolute applied to the image will process a small matrix so it can
/// be used for other effects like blurring, sharpening, embossing, edge
/// detection, etc.
/// @param image Is referencing the Image structure.
/// @param kernel Is referencing the Kernel structure.

void convolute(Image& image, const Kernel& kernel) {

    Image temp = {image.width, image.height, image.max_colors};  // copy

    // accessing the array
    for (unsigned int h = 1; h < image.height - 1; ++h) {
        for (unsigned int w = 1; w < image.width - 1; ++w) {
            float sum_red = 0;  // float variable
            float sum_green = 0;  // variable for sum of green pixel
            float sum_blue = 0;  // variable for sum of blue pixel
            for (int k = -1; k <= 1; ++k) {  // get all of the values
                for (int j = -1; j <= 1; ++j) {
                    // equations for the kernel to work with the array
                    const Pixel p = image.data[h - j][w - k];
                    const float ker = kernel[j + 1][k + 1];
                    // for the pixels
                    sum_red   = sum_red + p.red * ker;
                    sum_green = sum_green + p.green * ker;
                    sum_blue  = sum_blue + p.blue * ker;
                }
            }
            temp.data[h][w] = {  // the copies of everything
                static_cast<unsigned char>(min(max(sum_red, 0),
                                           image.max_colors)),
                // calling the max and min functions
                static_cast<unsigned char>(min(max(sum_green, 0),
                                           image.max_colors)),
                static_cast<unsigned char>(min(max(sum_blue, 0),
                                           image.max_colors))
            };
        }
    }

    image = temp;  // copy

}

// functions for image enhancements

/// @brief This function will turn the image into a grayscale image by averaging
/// the values of all three colors for a pixel and replacing them with the
/// average.
/// @param image Is referencing the Image structure.

void grayscale(Image& image) {
    // accessing the array for the image height and width, so we don't go over
    // and can do equations
    for (unsigned int h = 1; h < image.height + 1; ++h) {
        for (unsigned int w = 1; w < image.width + 1; ++w) {
            // averaging the values of all three colors, red, green, blue, for
            // the pixel
            unsigned char average = (image.data[h][w].red   +
                                     image.data[h][w].green +
                                     image.data[h][w].blue) / 3;
            // assigning the colors to the average value to grayscale the image
            image.data[h][w].red   = average;
            image.data[h][w].green = average;
            image.data[h][w].blue  = average;
        }
    }
}

/// @brief This function will replace every pixel in the image with its
/// complementary color by changing the color value to its negative.
/// @param image Is referencing the Image structure.

void negative(Image& image) {
    // accessing the pixels and the next one in array with +1
    for (unsigned int h = 1; h < image.height + 1; ++h) {
        for (unsigned int w = 1; w < image.width + 1; ++w) {
            // changing each color to its negative value
            unsigned char neg_pix_red =
                         static_cast<unsigned char>(255 - image.data[h][w].red);
            // subtracting to find the mamximu color depth
            unsigned char neg_pix_green =
                       static_cast<unsigned char>(255 - image.data[h][w].green);
            unsigned char neg_pix_blue =
                        static_cast<unsigned char>(255 - image.data[h][w].blue);
            // assinging the pixel colors to the calculated color depths
            image.data[h][w].red = neg_pix_red;
            image.data[h][w].green = neg_pix_green;
            image.data[h][w].blue = neg_pix_blue;
        }
    }

}

// functions for image transformations

/// @brief This function will reflect the image along the vertical axis.
/// @param image Is referencing the Image structure.

void flop(Image& image) {
    // accessing the height of the array
    for (unsigned int h = 1; h < image.height + 1; ++h) {
        int temp_width = image.width;  // set the temporary width
                                       // to the image width
        // after accessing the width, will swap the pixel positions,
        // decrementing to flop the image.
        for (unsigned int w = 1; w < image.width / 2; ++w) {
            swap_pixels(image.data[h][w],
                        image.data[h][temp_width]);  // the array
            --temp_width;  // decrementing
        }
    }
}

/// @brief This function will flip an image in the vertical direction, which is
/// a reflection across the horizontal axis.
/// @param image Is referencing the Image structure.

void flip(Image& image) {
    // accessing the width and height of array to swap the pixels
    for (unsigned int w = 1; w < image.width + 1; ++w) {
        // for the width we assign to height to whatever the image height is
        int temp_height = image.height;
        // for the height we swap the pixels in the array
        for (unsigned int h = 1; h < image.height / 2; ++h) {
            swap_pixels(image.data[h][w],
                        image.data[temp_height][w]);  // flipping the pixels
                                // in the opposite direction from the flop
                                // function
            --temp_height;  // decrement height to go through array
        }
    }
}
/// @brief Function edit_options outputs the editing options for a picture
void edit_options() {  // options for user
    // all of the options outputted for the user to pick from
    cout << "Enhancements:\n"  // grayscale and negate are enhancements
         << "g - grayscale -- convert image to grayscale (b&w)\n"
         << "n - negate    -- replace each pixel with its complementary color\n"
         << '\n'
         << "Transformations:\n"  // flip and flop are transformations
         << "f - flop      -- flip image in the horizontal direction\n"
         << "v - flip      -- flip image in the vertical direction\n"
         << '\n'
         << "Effects:\n"  // these effects have to do with the convolute
         << "m - emboss    -- apply embossing effect to image\n"
         << "s - sharpen   -- sharpen the image\n"
         << "b - blur      -- blur (soften) the image\n"
         << "e - edge      -- detect edges in the image\n"
         << endl;


}
/// This function will return the max float for either pixel a or b
/// @param a A float which will be referenced
/// @param b A float which will be referenced
/// @returns the max float
float max(const float& a, const float& b) {
    return b < a ? a : b;  // if else
}

/// This function will return the min float for either pixel a or b
/// @param a A float which will be referenced
/// @param b A float which will be referenced
/// @returns the min float
float min(const float& a, const float& b) {
    return b > a ? a : b;  // if else
}
