#include <iostream>
#include <fstream>
#include <cmath>

// Disable structure padding, ensuring no extra bytes are added between members
#pragma pack(push, 1)

// Bitmap file header structure (14 bytes)
struct BitmapFileHeader {
    char header[2] = { 'B', 'M' };// BMP file header signature
    uint32_t fileSize;            // Size of the file in bytes
    uint16_t reserved1 = 0;       // Reserved, should be 0
    uint16_t reserved2 = 0;       // Reserved, should be 0
    uint32_t dataOffset;          // Offset to the data (pixel) array
};

// Bitmap information header structure (40 bytes)
struct BitmapInfoHeader {
    uint32_t headerSize = 40;     // Size of the info header in bytes
    int32_t width;                // Width of the image in pixels
    int32_t height;               // Height of the image in pixels
    uint16_t planes = 1;          // Number of color planes, should be 1
    uint16_t bitCount = 8;        // Number of bits per pixel (8 for grayscale)
    uint32_t compression = 0;     // Compression method, 0 for no compression
    uint32_t imageSize = 0;       // Size of the image data in bytes (can be 0 for uncompressed)
    int32_t xPixelsPerMeter = 0;  // Horizontal pixels per meter, not significant
    int32_t yPixelsPerMeter = 0;  // Vertical pixels per meter, not significant
    uint32_t colorsUsed = 256;    // Number of colors used in the palette
    uint32_t colorsImportant = 256; // Number of important colors, not significant
};

// Structure to represent RGB color components
struct RgbQuad {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved = 0; // Reserved, typically set to 0
};

// Restore previous alignment setting when these structures are out of scope
#pragma pack(pop)

//Helper function to generate heatmap based on the R,G,B values
void getHeatMapColor(float value, float* red, float* green, float* blue)
{
    value /= 255.0; //normalize color value to [0-1]
    const int NUM_COLORS = 4;
    static float color[NUM_COLORS][3] = { {0,0,1}, {0,1,0}, {1,1,0}, {1,0,0} };
    // A static array of 4 colors: (blue,green,yellow,red) using {r,g,b} for each.

    //Our desired color will be between these two indexes in "color".
    int idx1; 
    int idx2;
    // Fraction between "idx1" and "idx2" where our value is.
    float fractBetween = 0;

    if (value <= 0) { idx1 = idx2 = 0; }    // accounts for an input <=0
    else if (value >= 1) { idx1 = idx2 = NUM_COLORS - 1; }    // accounts for an input >=0
    else
    {
        value = value * (NUM_COLORS - 1);        // Will multiply value by 4.
        idx1 = floor(value);                  // Our desired color will be after this index.
        idx2 = idx1 + 1;                        // ... and before this index (inclusive).
        fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).
    }
    //remap fraction to [0-255]
    *red = 255 * ((color[idx2][0] - color[idx1][0]) * fractBetween + color[idx1][0]);
    *green = 255 * ((color[idx2][1] - color[idx1][1]) * fractBetween + color[idx1][1]);
    *blue = 255 * ((color[idx2][2] - color[idx1][2]) * fractBetween + color[idx1][2]);
}

void jetColor(float x, float* red, float* green, float* blue) {
    float r, g, b;
    x = x / 255.0;
    if (x < 0.0) x = 0.0;
    if (x > 1.0) x = 1.0;

    if (x < 0.125) {
        r = 0.0;
        g = 0.0;
        b = 0.5 + 4 * x;
    }
    else if (x < 0.375) {
        r = 0.0;
        g = 4 * x - 0.5;
        b = 1.0;
    }
    else if (x < 0.625) {
        r = 4 * x - 0.5;
        g = 1.0;
        b = 1.0 - 4 * x + 0.5;
    }
    else if (x < 0.875) {
        r = 1.0;
        g = 1.0 - 4 * x + 0.5;
        b = 0.0;
    }
    else {
        r = 1.0 - 4 * x + 0.5;
        g = 0.0;
        b = 0.0;
    }
    *red = 255 * r;
    *green = 255 * g;
    *blue = 255 * b;
}

// Function to write the bitmap data to a file
void writeBitmap(const char* filename,  uint8_t** const imageData2D, int width, int height) {
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;

    // Calculate padding for each scanline to ensure correct alignment in file
    int padding = (4 - (width % 4)) % 4;

    const int colorCount = 256;

    //Create color palette for 5 color heatmap from 256 level grayscale values
    RgbQuad palette[colorCount];
    for (int i = 0; i < colorCount; ++i) {
        float r, g, b;
        getHeatMapColor(i, &r, &g, &b);
        //jetColor(i, &r, &g, &b);
        palette[i].red = r;
        palette[i].green = g;
        palette[i].blue = b;
    }

    // Fill in the file header details
    fileHeader.fileSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + (colorCount * sizeof(RgbQuad)) + (width+ padding)*height;
    fileHeader.dataOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + (colorCount * sizeof(RgbQuad));

    // Fill in the info header details
    infoHeader.width = width;
    infoHeader.height = height;

    // Open the output file in binary mode
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return;
    }
    // Write the file header and info header
    outFile.write(reinterpret_cast<const char*>(&fileHeader), sizeof(BitmapFileHeader));
    outFile.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BitmapInfoHeader));
    outFile.write(reinterpret_cast<const char*>(palette), colorCount * sizeof(RgbQuad));

    // Write the pixel data (imageData) along with padding if needed
    for (int y = height - 1; y >= 0; --y) {
        outFile.write(reinterpret_cast<const char*>(imageData2D[y]), width);
        for (int p = 0; p < padding; ++p) {
            outFile.put(0);
        }
    }

    // Close the output file
    outFile.close();
}