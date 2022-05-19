#include "color.hpp"
#include <ostream>
#include <regex>
#include <initializer_list>
#include <algorithm>

void Color::ConvertToHSLFromRGB(){
    // https://www.rapidtables.com/convert/color/rgb-to-hsl.html
    float rNorm = r/255.0; // Normalized RED
    float gNorm = g/255.0; // Normalized GREEN
    float bNorm = b/255.0; // Normalized BLUE

    float cMax = std::max({rNorm, gNorm, bNorm});
    float cMin = std::min({rNorm, gNorm, bNorm});
    float delta = cMax - cMin;

    if(delta == 0){
        hue = 0;
    } else if((delta != 0) && (cMax == rNorm)){
        hue = 60 * (int((gNorm-bNorm)/delta) % 6);
    } else if((delta != 0) && (cMax == gNorm)){
        hue = 60 * (((bNorm-rNorm)/delta) + 2);
    } else if((delta != 0) && (cMax == bNorm)){
        hue = 60 * (((rNorm-gNorm)/delta) + 4);
    }

    l = (cMax + cMin)/2;
    if(delta){
        s = delta/(1-std::abs(2*l-1));
    } else{
        s = 0;
    }
}

Color::Color(const std::string &colorData){
    std::regex hexType("^([0-9a-f]{3}|[0-9a-f]{6}|[0-9a-f]{8})$");
    std::regex decimalType("^([0-9]{1,3},){3}[0-9]{1,3}$");

    auto hexMatchIteratorBegin = std::sregex_iterator(colorData.begin(), colorData.end(), hexType);
    auto hexMatchIteratorEnd = std::sregex_iterator();

    auto decimalMatchIteratorBegin = std::sregex_iterator(colorData.begin(), colorData.end(), decimalType);
    auto decimalMatchIteratorEnd = std::sregex_iterator();
    // first regex formatting
    if(std::distance(hexMatchIteratorBegin, hexMatchIteratorEnd)){
        std::smatch match = *hexMatchIteratorBegin;
        std::string matchStr = match.str();
        if(matchStr.size() == 3){
            r = strtoul(matchStr.substr(0, 1).c_str(), NULL, 16);
            b = strtoul(matchStr.substr(1, 1).c_str(), NULL, 16);
            g = strtoul(matchStr.substr(2, 1).c_str(), NULL, 16);
            alpha = 255;
        } else if(matchStr.size() == 6){
            r = strtoul(matchStr.substr(0, 2).c_str(), NULL, 16);
            b = strtoul(matchStr.substr(2, 2).c_str(), NULL, 16);
            g = strtoul(matchStr.substr(4, 2).c_str(), NULL, 16);
            alpha = 255;
        } else if(matchStr.size() == 8){
            r = strtoul(matchStr.substr(0, 2).c_str(), NULL, 16);
            b = strtoul(matchStr.substr(2, 2).c_str(), NULL, 16);
            g = strtoul(matchStr.substr(4, 2).c_str(), NULL, 16);
            alpha = strtoul(matchStr.substr(6, 2).c_str(), NULL, 16);
        }
    }
    // Second regex formatting
    if(std::distance(decimalMatchIteratorBegin, decimalMatchIteratorEnd)){
        std::smatch match = *decimalMatchIteratorBegin;
        std::string matchStr = match.str();
        std::vector<int> rgba;
		std::string value;
		std::stringstream matchStream(matchStr);

		while(std::getline(matchStream, value, ',')){
			rgba.push_back(std::stoi(value));
		}

		r = rgba[0];
		g = rgba[1];
		b = rgba[2];
		alpha = rgba[3];
    }

    if(!std::distance(decimalMatchIteratorBegin, decimalMatchIteratorEnd) && !std::distance(hexMatchIteratorBegin, hexMatchIteratorEnd)){
        std::cerr << "Wrong data format\n" << std::endl;
    }

    ConvertToHSLFromRGB();
}

std::ostream& operator<<(std::ostream& outStream, const Color& color){
    outStream << "Red: " << color.r << std::endl;
    outStream << "Green: " << color.g << std::endl;
    outStream << "Blue: " << color.b << std::endl;
    outStream << "Alpha: " << color.alpha << std::endl;
    outStream << "Hex: " << "#" << std::hex << color.r  << color.g 
               << color.b  << color.alpha << std::dec << std::endl;
    outStream << "Hue: " << color.hue << std::endl;
    outStream << "Saturation: " << color.s << std::endl;
    outStream << "Lightness: " << color.l << std::endl;
    return outStream;
}