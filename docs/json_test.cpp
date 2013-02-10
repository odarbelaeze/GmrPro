// requires:
// sudo apt-get install libjsoncpp-dev
// compile with:
// g++ -o json_test json_test.cpp -ljsoncpp

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

int main(int argc, char const *argv[])
{
    Json::Value root;
    Json::Reader reader;

    std::ifstream config_doc ("descriptor_example.json");

    bool parsingSuccessful = reader.parse( config_doc, root, false );
    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "Failed to parse configuration\n"
                   << reader.getFormattedErrorMessages();
    }
    else
    {
        std::cout << root.toStyledString () << std::endl;
    }

    std::cout << root["system"]["dimensions"]["width"].asInt() << std::endl;

    return 0;
}