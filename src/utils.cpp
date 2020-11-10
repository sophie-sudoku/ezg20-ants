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
    std::stringstream ss;
    ss << input;
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(ss, segment, ','))
    {
        seglist.push_back(segment);
    }

    //for (int i = 0; i < seglist.size(); i++) {cout << seglist[i] << " ";}
    return seglist;
}

glm::mat4 makeMat4(std::string input)
{
    glm::mat4 result;
    std::vector<std::string> objects = splitString(input);
    int i = std::stoi(objects[0]);
    cout << i;
    return result;
}