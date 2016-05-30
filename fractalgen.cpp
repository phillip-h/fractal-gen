#include <cstdlib>
#include <iostream>

#include "complex.hpp"

#include "lodepng.h"

using std::cout;
using std::vector;

unsigned eval(const complex comp, unsigned max,
              const complex arg)
{
    complex c = comp;
    for (unsigned i = 0; i < max; i++)
    {
        if (c.abs() > 2)
            return i;
        c = c * c + arg;
    }
    return max;
}

void set_pixel(unsigned char r, unsigned char g, unsigned char b,
               unsigned x, unsigned y, vector<unsigned char> &vec,
               unsigned height)
{
    unsigned index = (x + y * height) * 4;
    vec.at(index + 0) = r;
    vec.at(index + 1) = g;
    vec.at(index + 2) = b;
    vec.at(index + 3) = 0xff;
}

int main(int argc, char** argv)
{
    if (argc < 4) {
        cout << "too few arguments!\n";
        cout << "usage: mandelbrot <width> <height> <scale>\n";
        cout << "                  ";
        cout << "[cr + ci] [color depth] [offx] [offy]\n";
        return -1;
    }
    
    unsigned width = atoi(argv[1]);
    unsigned height = atoi(argv[2]);
    double size = atof(argv[3]); 
    unsigned color_depth = 255;
    double offx = -0.5;
    double offy = 0.0;
    bool mandel = true;
    complex c_arg(0, 0);
    auto file = "fractal.png";

    if (argc > 4 && std::string(argv[4]) != "mandelbrot") {
        mandel = false;
        offx = 0;
        offy = 0;
        std::string str(argv[4]);
        unsigned pos = 0;
        for (unsigned i = 0; i < str.size(); i++)
        {
            if (str.at(i) == '+') {
                pos = i + 1;
                break;
            }
        }
        if (pos == 0) {
            cout << "invalid complex value!\n";
            return -1;
        }
        double real = atof(str.substr(0, pos - 1).c_str());
        double imag = atof(str.substr(pos, str.size() - pos).c_str());
        c_arg = complex(real, imag);
    }

    if (argc > 5)
        color_depth = atof(argv[5]);
    if (argc > 6)
        offx = atof(argv[6]);
    if (argc > 7)
        offy = atof(argv[7]);
    if (argc > 8)
        cout << "warning -- extra arguments, ignoring them.\n";

    vector<unsigned char> image(width * height * 4);

    auto lerp = [&] (double x0, double y0,
                    double x1, double y1,
                    double t)
    {
        double m = (y1 - y0) / (x1 - x0);
        return y0 + t * m;
    };

    const auto A_R = 67;
    const auto A_G = 10;
    const auto A_B = 42;

    const auto B_R = 232;
    const auto B_G = 56;
    const auto B_B = 56;

    const auto D_R = 21;
    const auto D_G = 21;
    const auto D_B = 21;

    auto comp_color_r = [&] (unsigned val)
    {
        if (val == color_depth)
            return D_R;
        double rat = (double) val / color_depth;
        double dc = lerp(0, A_R, 1, B_R, rat);
        return (int) dc;
    };

    auto comp_color_g = [&] (unsigned val)
    {
        if (val == color_depth)
            return D_G;
        double rat = (double) val / color_depth;
        double dc = lerp(0, A_G, 1, B_G, rat);
        return (int) dc;
    };

    auto comp_color_b = [&] (unsigned val)
    {
        if (val == color_depth)
            return D_B;
        double rat = (double) val / color_depth;
        double dc = lerp(0, A_B, 1, B_B, rat);
        return (int) dc;
    };

    double r;
    double i;
    complex c;
    unsigned color;
    for (unsigned x = 0; x < width; x++)
    {
        for (unsigned y = 0; y < height; y++)
        {
            r = offx - size / 2 + size * x / width;
            i = offy - size / 2 + size * y / height;
            c = complex(r, i);
            if (mandel)
                color = eval(c, color_depth, c);
            else
                color = eval(c, color_depth, c_arg);
            set_pixel(comp_color_r(color),
                      comp_color_g(color),
                      comp_color_b(color), 
                      x, y, image, height);
        }
    }

    unsigned error = lodepng::encode(file, image, width, height);
    if (error)
        cout << "error -- " << lodepng_error_text(error) << std::endl;
    
    return 0;
}
