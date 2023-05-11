#include "BMPprocessing.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FILE_SIZE 0x02
#define DATA_OFFSET_OFFSET 0x0A
#define WIDTH_OFFSET 0x12
#define HEIGHT_OFFSET 0x16
#define BITS_PER_PIXEL_OFFSET 0x1C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

uint8_t *readImage(const char *fileIn, uint32_t *width, uint32_t *height, float ***brightness, uint32_t *unpaddedRowSize, uint16_t *bitsPerPixel)
{
    FILE *imageFile = fopen(fileIn, "rb");
    if (imageFile == NULL)
    {
        perror("Error occured while opening the file");
        return 0;
    }
    uint32_t fileSize;
    fseek(imageFile, FILE_SIZE, SEEK_SET); // read file size
    fread(&fileSize, 4, 1, imageFile);
    printf("File size = %u bytes\n", fileSize);
    uint32_t dataOffset;
    fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET); // read data offset
    fread(&dataOffset, 4, 1, imageFile);
    fseek(imageFile, WIDTH_OFFSET, SEEK_SET); // read width
    fread(width, 4, 1, imageFile);
    printf("Width = %u pixels\n", *width);
    fseek(imageFile, HEIGHT_OFFSET, SEEK_SET); // read height
    fread(height, 4, 1, imageFile);
    printf("Height = %u pixels\n", *height);
    fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET); // read bits per pixel
    fread(bitsPerPixel, 2, 1, imageFile);
    // each row is padded to be a multiple of 4 bytes
    uint32_t paddedRowSize;
    if ((*width)*(*bitsPerPixel) % 32 == 0)
       paddedRowSize = (((*width)*(*bitsPerPixel)) / 32) * 4;
    else
       paddedRowSize = (((*width)*(*bitsPerPixel) + 31) / 32) * 4;
    *unpaddedRowSize = (*width)*(uint32_t)(*bitsPerPixel) / 8;
    uint8_t *pixels = (uint8_t*)malloc((*unpaddedRowSize)*(*height));
    uint8_t *startPixels = pixels;
    *brightness = (float **)malloc(sizeof(float*) * (*height)); // 2D array to store brightness
    for (uint32_t i = 0; i < *height; i++)
        (*brightness)[i] = (float*)malloc(sizeof(float) * (*unpaddedRowSize));
    uint16_t p = 1, k = 0;
    float sumBrightness = 0, averageBrightness = 0;;
    for (uint32_t i = 0; i < *height; i++)
    {
        for (uint32_t j = 0; j < *unpaddedRowSize; j+=3)
        {
            fseek(imageFile, dataOffset+j, SEEK_SET);
            fread(pixels, 1, 3, imageFile);
            (*brightness)[i][k] = ((0.0722*(float)(*pixels)/255) + (0.7152*(float)(*(pixels+1))/255) + (0.2126*(float)(*(pixels+2))/255))*100;
            sumBrightness += (*brightness)[i][k];
            averageBrightness = sumBrightness / p;
            pixels += 3;
            p++;
            k++;
        }
        k = 0;
        dataOffset = dataOffset + (paddedRowSize-(*unpaddedRowSize));
    }
    printf("Average brightness = %.2f%%\n", averageBrightness);
    fclose(imageFile);
    return startPixels;
}
uint8_t *sortPixels(uint32_t width, uint32_t height, uint8_t *pixels, float **brightness, uint32_t unpaddedRowSize)
{
    uint8_t *startPixels = pixels;
    uint8_t noSwap = 1, temp;
    for (uint32_t i = 0; i < height; i++)
    {
        pixels = startPixels + unpaddedRowSize*i;
        for (uint32_t m = width - 1; m >= 0; m--)
        {
            noSwap = 1;
            for (uint32_t j = 0, k = 0; j < m; j++, k += 3)
            {
                if (brightness[i][j] > brightness[i][j+1])
                {
                    temp = pixels[k];
                    pixels[k] = pixels[k+3];
                    pixels[k+3] = temp;
                    temp = pixels[k+1];
                    pixels[k+1] = pixels[k+4];
                    pixels[k+4] = temp;
                    temp = pixels[k+2];
                    pixels[k+2] = pixels[k+5];
                    pixels[k+5] = temp;
                    temp = brightness[i][j];
                    brightness[i][j] = brightness[i][j+1];
                    brightness[i][j+1] = temp;
                    noSwap = 0;
                }
            }
            if (noSwap)
                break;
        }
    }
    for (uint32_t i = 0; i < height; i++)
            free(brightness[i]);
    free(brightness);
    return startPixels;
}
uint8_t *rotateImage(uint32_t *width, uint32_t *height, uint8_t *pixels, uint16_t rotation, uint32_t unpaddedRowSize)
{
    uint8_t *rotatedPixels = (uint8_t*)malloc(unpaddedRowSize*(*height));
    switch(rotation)
    {
        case 1:
        {
            uint32_t oldWidth = *width;
            uint32_t oldHeight = *height;
            uint32_t temp = *width;
            *width = *height;
            *height = temp;
            uint32_t index = 0;
            for (uint32_t y = 0; y < *height; y++)
            {
                for (uint32_t x = 0; x < *width; x++)
                {
                    uint32_t oldIndex = ((oldHeight-1-x) * oldWidth + y) * 3;
                    rotatedPixels[index] = pixels[oldIndex];
                    rotatedPixels[index+1] = pixels[oldIndex+1];
                    rotatedPixels[index+2] = pixels[oldIndex+2];
                    index += 3;
                }
            }
            free(pixels);
            return rotatedPixels;
        }
        case 2:
        {
            for (int y = 0; y < *height; y++)
            {
                for (int x = 0; x < *width; x++)
                {
                    uint32_t new_x = (*width) - x - 1;
                    uint32_t new_y = (*height) - y - 1;
                    rotatedPixels[(new_x + new_y * (*width)) * 3] = pixels[(x + y * (*width)) * 3];
                    rotatedPixels[(new_x + new_y * (*width)) * 3 + 1] = pixels[(x + y * (*width)) * 3 + 1];
                    rotatedPixels[(new_x + new_y * (*width)) * 3 + 2] = pixels[(x + y * (*width)) * 3 + 2];
                }
            }
            free(pixels);
            return rotatedPixels;
        }
        case 3:
        {
            uint32_t oldWidth = *width;
            uint32_t temp = *width;
            *width = *height;
            *height = temp;
            uint32_t index = 0;
            for (uint32_t y = 0; y < *height; y++)
            {
                for (uint32_t x = 0; x < *width; x++)
                {
                    uint32_t oldIndex = ((x+1) * oldWidth - (y+1)) * 3;
                    rotatedPixels[index] = pixels[oldIndex];
                    rotatedPixels[index+1] = pixels[oldIndex+1];
                    rotatedPixels[index+2] = pixels[oldIndex+2];
                    index += 3;
                }
            }
            free(pixels);
            return rotatedPixels;
        }
    }
    free(pixels);
    return rotatedPixels;
}
void writeImage(const char *fileOut, uint32_t width, uint32_t height, uint8_t *pixels, uint16_t bitsPerPixel)
{
    FILE *outputFile = fopen(fileOut, "wb");
    if (outputFile == NULL)
    {
        perror("Error occured while opening the file");
        return;
    }
    const char *BM = "BM"; // BMP header
    fwrite(&BM[0], 1, 1, outputFile);
    fwrite(&BM[1], 1, 1, outputFile);
    typedef struct
    {
        uint32_t fileSize;
        uint32_t reserved;
        uint32_t dataOffset;
    } BMPFileHeader;
    uint32_t paddedRowSize;
    if ((width*bitsPerPixel) % 32 == 0)
        paddedRowSize = ((width*bitsPerPixel) / 32) * 4;
    else
        paddedRowSize = (((width*bitsPerPixel) + 31) / 32) * 4;
    uint32_t fileSize = HEADER_SIZE + INFO_HEADER_SIZE + paddedRowSize*height;
    uint32_t dataOffset = HEADER_SIZE+INFO_HEADER_SIZE;
    BMPFileHeader fileHeader = {fileSize, 0, dataOffset};
    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, outputFile);
    typedef struct // DIB header
    {
        uint32_t headerSize;
        uint32_t imageWidth;
        uint32_t imageHeight;
        uint16_t planes;
        uint16_t bitsPerPixel;
        uint32_t compression;
        uint32_t imageSize;
        uint32_t xPixelsPerMeter;
        uint32_t yPixelsPerMeter;
        uint32_t colorsUsed;
        uint32_t colorsImportant;
    } BMPImageHeader;
    uint32_t imageSize = width*height*(bitsPerPixel/8);
    uint32_t resolutionX = 2834; //72 dpi
    uint32_t resolutionY = 2834; //72 dpi
    BMPImageHeader imageHeader = {INFO_HEADER_SIZE, width, height, 1, bitsPerPixel, 0, imageSize, resolutionX, resolutionY, 0, 0};
    fwrite(&imageHeader, sizeof(BMPImageHeader), 1, outputFile);
    uint32_t unpaddedRowSize = width*(bitsPerPixel/8);
    for (uint32_t i = 0; i < height; i++)
    {
            uint32_t pixelOffset = i*unpaddedRowSize;
            fwrite(&pixels[pixelOffset], 1, paddedRowSize, outputFile);
    }
    fclose(outputFile);
}
