# Image processor

<img src="original.bmp" alt="alt text" style="width: 20cm;">

This task requires you to implement a console application,
allows you to apply various filters to images,
similar to filters in popular graphic editors.

## Supported image format

Input and output graphic files must be in the format [BMP](http://en.wikipedia.org/wiki/BMP_file_format).

The BMP format supports quite a lot of variations, but in this task we will use
24-bit BMP without compression and without color table. Type used `DIB header` - `BITMAPINFOHEADER`.

An example file in the required format is in the Wikipedia article [in the "Example 1" section](https://en.wikipedia.org/wiki/BMP_file_format#Example_1)
and in the folder [examples](examples).

When testing, be sure to ensure that the test image 
was saved in 24-bit BMP.

## Command Line Argument Format

Description of the command line argument format:

`{program name} {path to input file} {path to output file}
[-{filter name 1} [filter parameter 1] [filter parameter 2] ...]
[-{filter name 2} [filter parameter 1] [filter parameter 2] ...] ...`

When run without arguments, the program displays help.

### Example
`./image_processor input.bmp /tmp/output.bmp -crop 800 600 -gs -blur 0.5`

In this example
1. The image is loaded from the file `input.bmp`
2. Cropped to an image starting in the upper left corner and 800x600 pixels in size
3. Converted to grayscale
4. Apply blur with sigma 0.5
5. The resulting image is saved to the file `/tmp/output.bmp`

The list of filters may be empty, then the image must be saved unchanged.
Filters are applied in the order they are listed in the command line arguments.

## Filters

In the formulas we further assume that each color component
represented by a real number from 0 to 1. Pixel colors
represented by triplets `(R, G, B)`. Thus, `(0, 0, 0)` is black, 
`(1, 1, 1)` – white.

If the filter is specified by a matrix, this means that the value of each of the colors is determined by the weighted sum
values ​​of this color in neighboring pixels in accordance with the matrix. In this case, the target pixel
corresponds to the central element of the matrix.

For example, for a filter defined by the matrix

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D1%20&%202%20&%203%20%5C%5C4%20&%205%20&%206%20%5C%5C7%20&%208%20&%209%20%5C%5C%5Cend%7Bbmatrix%7D)

The value of each of the colors of the target pixel `C[x][y]` will be determined by the formula

```
C[x][y] =
  min(1, max(0,
   1*C[x-1][y-1] + 2*C[x][y-1] + 3*C[x+1][y-1] +
   4*C[x-1][y]   + 5*C[x][y]   + 6*C[x+1][y]   +
   7*C[x-1][y+1] + 8*C[x][y+1] + 9*C[x+1][y+1]
))
```

When processing pixels close to the edge of the image, part of the matrix may extend beyond the border of the image.
In this case, the value of the nearest pixel should be used as the value of the pixel that goes beyond the boundary.
image pixel to it. 

### List of basic filters

#### Crop (-crop width height)
Crop the image to the specified width and height. The top left part of the image is used.

If the requested width or height exceeds the dimensions of the original image, the available portion of the image is returned.

#### Grayscale (-gs)

<img src="grayscale.bmp" alt="alt text" style="width: 20cm;">

Converts an image to grayscale using the formula

![encoding](https://latex.codecogs.com/svg.image?R'%20=%20G'%20=%20B'%20=0.299%20R%20&plus;%200%20.587%20G%20&plus;%200%20.%20114%20B)

#### Negative (-neg)

<img src="negative.bmp" alt="alt text" style="width: 20cm;">

Converts an image to a negative using the formula

![encoding](https://latex.codecogs.com/svg.image?R'%20=%201%20-%20R,%20G'%20=%201%20-%20G,%20B'%20=%201%20-%20B)

#### Sharpening (-sharp)

<img src="sharp.bmp" alt="alt text" style="width: 20cm;">

Sharpening. Achieved by using a matrix

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D%20&%20-1%20&%20%20%5C%5C-1%20&%205%20&%20-1%20%5C%5C%20&%20-1%20&%20%20%5C%5C%5Cend%7Bbmatrix%7D)

#### Edge Detection (-edge threshold)

<img src="edge_detection.bmp" alt="alt text" style="width: 20cm;">

Border selection. The image is converted to grayscale and the matrix is ​​applied

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D%20&%20-1%20&%20%20%5C%5C-1%20&%204%20&%20-1%20%5C%5C%20&%20-1%20&%20%20%5C%5C%5Cend%7Bbmatrix%7D)

Pixels with a value exceeding `threshold` are colored white, the rest - black.

#### Gaussian Blur (-blur sigma)

<img src="blur.bmp" alt="alt text" style="width: 20cm;">

[Gaussian blur](https://ru.wikipedia.org/wiki/Размытие_по_Гауссу),
parameter – sigma.

The value of each pixel color `C[x0][y0]` is determined by the formula

![encoding](https://latex.codecogs.com/svg.image?C%5Bx_0%5D%5By_0%5D%20=%20%5Csum_%7Bx=0,y=0%7D%5E%7Bwidth-1,%20height-1%7DC%5Bx%5D%5By%5D%5Cfrac%7B1%7D%7B%5Csqrt%5B%5D%7B2%5Cpi%5Csigma%5E2%7D%7De%5E%7B-%5Cfrac%7B%5Cleft%7Cx_o-x%5Cright%7C%5E2%20&plus;%20%5Cleft%7Cy_o-y%5Cright%7C%5E2%7D%7B2%5Csigma%5E2%7D%7D)

There are various options for implementing and optimizing the calculation of this filter, there is a description [In wikipedia](https://ru.wikipedia.org/wiki/Размытие_по_Гауссу).

#### Pixellate (-custom)

Pixellates image into pixels of defined size. Idea from [link](https://developer.apple.com/documentation/coreimage/cifilter/3228393-pixellate/)

## Implementation

It is prohibited to use third-party libraries for working with images.

Try to make all program components as universal as possible and not tied to the specifics of a specific task.

All exception situations must be handled correctly with an error message that is understandable to the user.
No scenarios, including the use of files with a format that does not comply with the specification, should cause the program to crash.

You will most likely need the following components:
- A class that represents an image and provides work with it
- Classes for reading and writing BMP format
- Filters
- Controller that controls the sequential application of filters

Common parts should be allocated through inheritance.

It is recommended to discuss the detailed design of the program with the teacher at seminars.

