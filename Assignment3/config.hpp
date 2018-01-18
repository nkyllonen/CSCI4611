#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace Config {

    const std::string dataDir = "/home/narain/4611/assignments/a3-quakevis/data";

    const std::string textureFile = dataDir + "/earth-2k.bmp";

    const std::string quakeFile = dataDir + "/earthquakes.txt";

    const float timeWindow = 365*24*3600;

}

#endif
