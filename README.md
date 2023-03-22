# BMP-processing
Reading and editing 24-bit BMP-file

Описание консольного приложения

 1) Принимает на вход три аргумента: имя входного файла в формате filein.bmp, имя выходного файла в формате fileout.bmp и указание поворота (число от 0 до 3)
 2) Читает файл filein.bmp
 3) Печатает в консоль данные о файле: суммарный размер в байтах, размер в пикселях по горизонтали и вертикали, среднюю яркость картинки.
 4) В каждой строчке картинки сортирует пиксели по возрастанию яркости
 5) Поворачивает файл 0 - нет поворота, 1 - поворот на 90 градусов влево, 2 - поворот на 180 градусов, 3 - поворот на 90 градусов вправо
 6) Сохраняет новую полученную таким образом картинку в fileout.bmp
 
 In English
 
 Discription of the console application
 
 1) The programm takes three arguments as input: the name of input file, the name of output file and rotation value (a number from 0 to 3).
 2) Reads input file.
 3) Prints information about the file to the console: total size in bytes, size in pixels horizontally and vertically, average image brightness.
 4) In each row sorts the pixels in ascending order of brightness.
 5) Rotates the file: 0 - no rotation, 1 - rotate 90 degrees to the left, rotate 180 degrees, 3 - rotate 90 degrees to the right.
 6) Saves the new image obtained in this way to the output file.
