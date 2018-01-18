#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace Config {

    const std::string dataDir = "C:\\4611_A3_data";

    const std::string textureFile = dataDir + "\\earth-1k.bmp";

    const std::string quakeFile = dataDir + "\\earthquakes.txt";

    const float timeWindow = 365*24*3600;

}

#endif
