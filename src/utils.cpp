#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include <glm/glm.hpp>

using namespace std;

char seps[] = ",";
char *token, *next_token;


glm::mat4 makeMat4(char one_line_string[])
{
    vector<string> vec_String_Lines;
    token = strtok_s(one_line_string, seps, &next_token);

    cout << "Extracting and storing data in a vector..\n\n\n";

    while (token != NULL)
    {
        vec_String_Lines.push_back(token);
        cout << token << endl;
        token = strtok_s(NULL, seps, &next_token);
    }
    cout << "Displaying end result in vector line storage..\n\n";

    for (int i = 0; i < vec_String_Lines.size(); ++i)
        cout << vec_String_Lines[i] << "\n";
    cout << "\n\n\n";
    return glm::mat4(1.0);
}