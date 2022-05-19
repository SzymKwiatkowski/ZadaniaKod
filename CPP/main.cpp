#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>

#include "color.hpp"

std::map<std::string, int> modes = {
    {"mix", 0},
    {"lowest", 1},
    {"highest", 2},
    {"mix-saturate", 3}
};

Color mixMode(const std::vector<Color> &colors){ // Color of average values of colors
    Color colorOut = Color();
    int sizeColors = colors.size();
    for(auto color : colors){
        colorOut.r += color.r/sizeColors;
        colorOut.g += color.g/sizeColors;
        colorOut.b += color.b/sizeColors;
        colorOut.alpha += color.alpha/sizeColors;
    }
    colorOut.ConvertToHSLFromRGB();
    return colorOut;
}

Color lowestMode(const std::vector<Color> &colors){ // Color of lowest from all of colors
    Color colorOut("ffffffff");
    for(auto color : colors){
        colorOut.r = color.r < colorOut.r ? color.r : colorOut.r;
        colorOut.g = color.g < colorOut.g ? color.g : colorOut.g;
        colorOut.b = color.b < colorOut.b ? color.b : colorOut.b;
        colorOut.alpha = color.alpha < colorOut.alpha ? color.alpha : colorOut.alpha;
    }
    colorOut.ConvertToHSLFromRGB();
    return colorOut;
}

Color highestMode(const std::vector<Color> &colors){ // Color of highest from all of colors
    Color colorOut("00000000");
    for(auto color : colors){
        colorOut.r = color.r > colorOut.r ? color.r : colorOut.r;
        colorOut.g = color.g > colorOut.g ? color.g : colorOut.g;
        colorOut.b = color.b > colorOut.b ? color.b : colorOut.b;
        colorOut.alpha = color.alpha > colorOut.alpha ? color.alpha : colorOut.alpha;
    }
    colorOut.ConvertToHSLFromRGB();
    return colorOut;
}

Color mix_saturateMode(const std::vector<Color> &colors){ // Last color have to have average saturation of all colors
    float newSat = 0;
    int sizeColors = colors.size();
    for (auto color : colors){
       newSat += color.s/sizeColors;
    }
    Color colorOut = colors[sizeColors-1];
    colorOut.s = newSat;

    return colorOut;
}

void readData(std::string filename, std::vector<Color>& colorsData) {
    std::fstream dataFile;
    dataFile.open(filename, std::ios::in); // open file
    if (!dataFile.is_open()) {
        printf("Cannot open file");
    } else {
        std::string line;
        while(getline(dataFile, line)){  
            colorsData.emplace_back(line); // append line to data vector
        }
      dataFile.close();   
    }
}

int main(int argc, char *argv[]) {
    std::vector<Color> colorsData;
    enum Mode {mix, lowest, highest, mix_saturate};
    Mode mode = mix;
    std::string filename = "./colors.txt";
    readData(filename, colorsData);

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if(arg == "-m" || arg == "--mode") {
            if ((i + 1) < argc){
                std::string nextArg = argv[i + 1];
                if (nextArg == "mix"){
                    mode = mix;
                    i++;
                } else if (nextArg == "lowest"){
                    mode = lowest;
                    i++;
                } else if (nextArg == "highest"){
                    mode = highest;
                    i++;
                } else if (nextArg == "mix-saturate"){
                    mode = mix_saturate;
                    i++;
                } else{
                    std::cerr << "Check mode you selected \nIgnoring it\n";
                    mode = mix;
                    i++;
                }
            }
        } else{
            colorsData.emplace_back(arg);
        }
    }

    if (colorsData.empty()){
        std::cerr << "No data in file!\n";
        return 1;
    }

    switch (mode){
    case mix:
        std::cout << mixMode(colorsData);
        break;

    case lowest:
        std::cout << lowestMode(colorsData);
        break;

    case highest:
        std::cout << highestMode(colorsData);
        break;

    case mix_saturate:
        std::cout << mix_saturateMode(colorsData);
        break;

    default:
        std::cout << mixMode(colorsData);
        break;
    }

   return 0;
}