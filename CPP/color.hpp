#pragma once

#include <iostream>
#include <string>

class Color{
    public:
    int r; // red
    int g; // green
    int b; // blue
    int alpha; // alpha
    
    int hue; // hue
    float s; // saturation
    float l; // lightness
    
    Color(){
        r = 0; g = 0; b = 0; alpha = 0; // RGB

        hue = 0; s = 0.0; l = 0.0; // HSL
    };

    Color(const std::string &);
    

    void ConvertToHSLFromRGB(); // Conversion to HSL

    friend std::ostream& operator<<(std::ostream&, const Color&); // operand overload for data printing
};