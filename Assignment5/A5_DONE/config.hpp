#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <glm/glm.hpp>

namespace Config {

    //const std::string dataDir = "/home/narain/4611/assignments/a5-artrender/data";
    //const std::string codeDir = "/home/narain/4611/assignments/a5-artrender/startercode";

	const std::string dataDir = "C:\\4611_A5_data";
	const std::string codeDir = "C:\\Data\\University of MN\\3rd Year\\2017 Spring\\CSCI 4611\\Assignment5\\startercode\\startercode";

    // Shaders
    const std::string phongVert = codeDir + "\\phong.vert";
    const std::string phongFrag = codeDir + "\\phong.frag";
    const std::string silhouetteVert = codeDir + "\\silhouette.vert";
    const std::string silhouetteFrag = codeDir + "\\silhouette.frag";

    // Mesh and ramps
    const std::string mesh = dataDir + "\\cow.obj";
	//const std::string mesh = dataDir + "\\sphere.obj";
    //const std::string diffuseRamp = dataDir + "\\standardDiffuse.bmp";
    //const std::string specularRamp = dataDir + "\\standardSpecular.bmp";
	const std::string diffuseRamp = dataDir + "\\toonDiffuse.bmp";
	const std::string specularRamp = dataDir + "\\toonSpecular.bmp";

    // Shader parameters.
    // TODO: Pass them into your shaders using uniform variables.

    // Lighting
    glm::vec4 Ia(0.3, 0.3, 0.3, 1);
    glm::vec4 Id(0.7, 0.7, 0.7, 1);
    glm::vec4 Is(1.0, 1.0, 1.0, 1);
    // Material parameters
    glm::vec4 ka(1.0, 0.4, 0.4, 1);
    glm::vec4 kd(1.0, 0.4, 0.4, 1);
    glm::vec4 ks(0.6, 0.6, 0.6, 1);
    float s = 50;

    // Outline parameters
    float thickness = 0.01;

}

#endif
