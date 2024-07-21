#include "CsvReader.h"

CsvReader::CsvReader(const std::string& filePath)
    : mFile(filePath)
{
}

std::shared_ptr<std::vector<std::string>> CsvReader::split(std::string s, const std::string delimiter)
{
    auto splits = std::make_shared<std::vector<std::string>>();
    uint32_t prevPos = 0;

    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) 
    {
        std::string token = s.substr(0, pos);
        splits->push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    //add remainder of split string
    splits->push_back(s);
    return splits;
}
