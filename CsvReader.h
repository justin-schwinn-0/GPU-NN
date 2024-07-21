#ifndef CSV_READER_H
#define CSV_READER_H

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include "Logger.h"

#include "DataFrame.h"

class CsvReader
{

public:

    CsvReader(const std::string& file);

    // populate the dataframe
    template<class F>
    std::shared_ptr<DataFrame<F>> populate()
    {
        std::ifstream file(mFile);
        std::string line;

        auto df = std::make_shared<DataFrame<F>>();

        if(std::getline(file,line))
        {
            auto labels = split(line);
            df->setLabels(labels);
        }
        else
        {
            LOG << "it broke" << LOG_ENDL;
        }

        int dataCount;
        while(std::getline(file,line))
        {
            auto splits = split(line);
            std::vector<F> addedRow;

            for(int i = 0; i < df->getColumnCount(); i++)
            {
                auto dataPoint = static_cast<F>(std::stod(splits->at(i)));
                addedRow.push_back(dataPoint);
            }

            df->addRow(addedRow);
            dataCount++;
        }

        LOG << "added " << std::to_string(dataCount) << " rows" << LOG_ENDL;

        return df;
    }
private:
    // splits a string on the given delimiter
    std::shared_ptr<std::vector<std::string>> split(std::string str,const std::string delimiter = ",");


    std::string mFile;

};
#endif
