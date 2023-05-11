#ifndef BMPprocessing_h
#define BMPprocessing_h

#include <stdio.h>
#include <stdint.h>

uint8_t *readImage(const char *fileIn, uint32_t *width, uint32_t *height, float ***brightness, uint32_t *unpaddedRowSize, uint16_t *bitsPerPixel);
uint8_t *sortPixels(uint32_t width, uint32_t height, uint8_t *pixels, float **brightness, uint32_t unpaddedRowSize);
uint8_t *rotateImage(uint32_t *width, uint32_t *height, uint8_t *pixels, uint16_t rotation, uint32_t unpaddedRowSize);
void writeImage(const char *fileOut, uint32_t width, uint32_t height, uint8_t *pixels, uint16_t bitsPerPixel);

#endif /* BMPprocessing_h */
