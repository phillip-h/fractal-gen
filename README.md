# fractal-gen
A C++ program to generate Mandelbrot and Julia set fractals

# about

`fractal-gen` is a simple program that will output a PNG image of the
fractal specified in the program's arguments. The user can set the size,
scale, colors, color depth, and translation of the generated image, along
with the fractal parameters themselves.

`lodepng` is used for PNG output.

# usage

### syntax

```
fractal-gen <width> <height> <scale> [color A] [color B] [color C]
                                     [cr+ci] [color depth] [offx] [offy]
```

### arguments

`<width>`  -> the integral width of the output image

`<height>` -> the integral height of the output image

`<scale>`  -> a floating-point scale for the output image. The scale can
              be thought of as how zoomed the fractal is, a smaller scale
              means a higher zoom.

`[color A]` -> the first color, provided in `#RRGGBB` format. The `#` may
               be ommited. This color is used as the first point in a
               linear interpolation, it will be used as values depart
               the bounds of the fractal. Defaults to `#000000`

`[color B]` -> the second color, provided in `#RRGGBB` format. The `#` may
               be ommited. This color is used as the second point in a
               linear interpolation, it will be used as values approach
               the bounds of the fractal. Defaults to `#FFFFFF`

`[color C]` -> the third color, provided in `#RRGGBB` format. The `#` may
               be ommited. This color is used for points determined to
               be within the fractal. For non-mandelbrot fractals, it
               may be wise to set it to the same value as color B.
               Defaults to `#000000`

`[cr+ci]`   -> The complex argument used in fractal generation. This is
               what can be used to change the generated fractal. Should
               be given in the for of `real_part + imaginary_part`, or
               `mandelbrot` for the mandelbrot set. Defaults to
               `mandelbrot`.

`[color depth]` -> the color depth used in coloring. A higher value will
                  be more accurate, but may fade the image. A lower value
                  will make the fractal "pop", but causes distortion that
                  detracts from the image accuracy. Experiment to find
                  the ideal balance for each fractal.

`[off x]` -> an integral argument for an X translation to be done on
             the generated image. defaults to `0.5`.

`[off y]` -> an integral argument for a Y translation to be done on the
             generatedf image. Defaults to `0.0`.

# known bugs

* non-square images don't always work


