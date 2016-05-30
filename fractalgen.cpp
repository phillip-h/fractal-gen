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

unsigned char htoi(std::string str)
{
    if (str.size() != 2)
        return 0;
    unsigned char res = 0;
    unsigned char c = str.at(0);
    if (c >= '0' && c <= '9')
        res += (c - 0x30) * 16;
    else if (c >= 'a' && c <= 'f')
        res += (c - 'a' + 10) * 16;
    else if (c >= 'A' && c <= 'F')
        res += (c - 'A' + 10) * 16;
    else
        cout << "unknown hex char '" << c << "'.\n";

    c = str.at(1);
    if (c >= '0' && c <= '9')
        res += (c - 0x30) * 1;
    else if (c >= 'a' && c <= 'f')
        res += (c - 'a' + 10) * 1;
    else if (c >= 'A' && c <= 'F')
        res += (c - 'A' + 10) * 1;
    else
        cout << "unknown hex char '" << c << "'.\n";

    return res;
}

int main(int argc, char** argv)
{
    if (argc < 4) {
        cout << "too few arguments!\n";
        cout << "usage: fractal-gen <width> <height> <scale>\n";
        cout << "                   ";
        cout << "[color A] [color B] [color C]\n";
        cout << "                   ";
        cout << "[cr+ci] [color depth] [offx] [offy]\n";
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

    auto A_R = 0;
    auto A_G = 0;
    auto A_B = 0;

    auto B_R = 255;
    auto B_G = 255;
    auto B_B = 255;

    auto D_R = 0;
    auto D_G = 0;
    auto D_B = 0;

    if (argc > 4) {
        std::string str(argv[4]);
        if (str.at(0) == '#')
            str = str.substr(1, str.size() - 1);
        if (str.size() != 6) {
            cout << "error -- invalid color!\n";
            cout << "express colors as '#RRGGBB'.\n";
            return -1;
        }

        std::string sub_str = str.substr(0, 2);
        A_R = htoi(sub_str);

        sub_str = str.substr(2, 2);
        A_G = htoi(sub_str);
        
        sub_str = str.substr(4, 2);
        A_B = htoi(sub_str);
    }

    if (argc > 5) {
        std::string str(argv[5]);
        if (str.at(0) == '#')
            str = str.substr(1, str.size() - 1);
        if (str.size() != 6) {
            cout << "error -- invalid color!\n";
            cout << "express colors as '#RRGGBB'.\n";
            return -1;
        }

        std::string sub_str = str.substr(0, 2);
        B_R = htoi(sub_str);

        sub_str = str.substr(2, 2);
        B_G = htoi(sub_str);
        
        sub_str = str.substr(4, 2);
        B_B = htoi(sub_str);
    }
    
    if (argc > 6) {
        std::string str(argv[6]);
        if (str.at(0) == '#')
            str = str.substr(1, str.size() - 1);
        if (str.size() != 6) {
            cout << "error -- invalid color!\n";
            cout << "express colors as '#RRGGBB'.\n";
            return -1;
        }

        std::string sub_str = str.substr(0, 2);
        D_R = htoi(sub_str);

        sub_str = str.substr(2, 2);
        D_G = htoi(sub_str);
        
        sub_str = str.substr(4, 2);
        D_B = htoi(sub_str);
    }

    if (argc > 7 && std::string(argv[7]) != "mandelbrot") {
        mandel = false;
        offx = 0;
        offy = 0;
        std::string str(argv[7]);
        unsigned pos = 0;
        for (unsigned i = 0; i < str.size(); i++)
        {
            if (str.at(i) == '+') {
                pos = i + 1;
                break;
            }
        }
        if (pos == 0) {
            cout << "invalid complex argument!\n";
            return -1;
        }
        double real = atof(str.substr(0, pos - 1).c_str());
        double imag = atof(str.substr(pos, str.size() - pos).c_str());
        c_arg = complex(real, imag);
    }

    if (argc > 8)
        color_depth = atof(argv[8]);
    if (argc > 9)
        offx = atof(argv[9]);
    if (argc > 10)
        offy = atof(argv[10]);
    if (argc > 11)
        cout << "warning -- extra arguments, ignoring them.\n";

    cout << "fractal information:\n\n";
    
    if (mandel) {
        cout << "fractal type -- mandelbrot\n";
    } else {
        cout << "fractal type -- julia\n";
        cout << "complex argument -- " << c_arg.to_str() << "\n";
    }

    cout << "\n";
    cout << "width       -- " << width << "\n";
    cout << "height      -- " << height << "\n";
    cout << "pixels      -- " << (width * height * 4) << "\n";
    cout << "scale       -- " << size << "\n";
    cout << "color A     -- (";
    cout << A_R << ", " << A_G << ", " << A_B << ")\n";
    cout << "color B     -- (";
    cout << B_R << ", " << B_G << ", " << B_B << ")\n";
    cout << "color C     -- (";
    cout << D_R << ", " << D_G << ", " << D_B << ")\n";
    cout << "color depth -- " << color_depth << "\n";
    cout << "offx        -- " << offx << "\n";
    cout << "offy        -- " << offy << "\n";

    vector<unsigned char> image(width * height * 4);

    auto lerp = [&] (double x0, double y0,
                    double x1, double y1,
                    double t)
    {
        double m = (y1 - y0) / (x1 - x0);
        return y0 + t * m;
    };

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
