#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <stdio.h> 
#include <string.h> 

#include <string>       // std::string
#include <sstream>      // std::stringstream

using namespace std;

char seps[] = ",";
char *token, *next_token;


std::vector<std::string> splitString(std::string input) {
    //clean up "[" and "]" at front and ende
    input.erase(0, 1);
    input.erase(input.size() - 1, input.size());

    //parse string and divide by ","
    std::stringstream ss;
    ss << input;
    std::string segment;
    std::vector<std::string> seglist;
    while (std::getline(ss, segment, ','))
    {
        seglist.push_back(segment);
    }

    //return vector of string values
    return seglist;
}

glm::mat4 makeMat4(std::string input) {
    glm::mat4 result = glm::mat4(1.0);
    std::vector<std::string> objects = splitString(input);

    //seperate into vectors of 4
    for (int i = 0; i < objects.size() / 4; i+=4) {
        result[i/4] = glm::vec4(
            std::stof(objects[i]), 
            std::stof(objects[i + 1]), 
            std::stof(objects[i + 2]),
            std::stof(objects[i + 3]));
    }

    return result;
}