// Написать прграмму на языке C, которая выполняет следующие действия:
// 1) Принимает на вход три аргумента: имя входного файла в формате filein.bmp, имя выходного файла в формате fileout.bmp и указание поворота (число от 0 до 3)
// 2) Читает файл filein.bmp
// 3) Печатает в консоль данные о файле: суммарный размер в байтах, размер в пикселях по горизонтали и вертикали, среднюю яркость картинки.
// 4) В каждой строчке картинки сортирует пиксели по возрастанию яркости
// 5) Поворачивает файл 0 - нет поворота, 1 - поворот на 90 градусов влево, 2 - поворот на 180 градусов, 3 - поворот на 90 градусов вправо
// 6) Сохраняет новую полученную таким образом картинку в fileout.bmp

// Внимание! Данная реализация работает только с 24-битными BMP-файлами. Для других форматов требуется доработка

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "BMPprocessing.h"

int main(int argc, const char * argv[])
{
    printf("Welcome to BMP viewer version 1.0\n");
    if (argc!=4)
    {
        printf ("Invalid command line arguments. Usage: %s <full path/filein.bmp> <full path/fileout.bmp> <rotation>\n", argv[0]);
        return 0;
    }
    const char *fileIn = argv[1];
    const char *fileOut = argv[2];
    uint16_t rotation = atoi(argv[3]);
    if (rotation!=0 && rotation!=1 && rotation!=2 && rotation!=3)
    {
        printf("Incorrect rotation value. Rotation value can only be 0, 1, 2 or 3\n");
        return 0;
    }
    uint32_t width, height, unpaddedRowSize;
    float **brightness;
    uint16_t bitsPerPixel;
    uint8_t *pixels = readImage(fileIn, &width, &height, &brightness, &unpaddedRowSize, &bitsPerPixel);
    pixels = sortPixels(width, height, pixels, brightness, unpaddedRowSize);
    if (rotation == 0)
        writeImage(fileOut, width, height, pixels, bitsPerPixel);
    else if (rotation == 1 || rotation == 2 || rotation == 3)
    {
        pixels = rotateImage(&width, &height, pixels, rotation, unpaddedRowSize);
        writeImage(fileOut, width, height, pixels, bitsPerPixel);
    }
    free(pixels);
    return 0;
}
