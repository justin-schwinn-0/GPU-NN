#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <limits>

#include "Logger.h"

using std::vector;

template<class F> class DataFrame
{
public:

    DataFrame() 
    {
    }
    DataFrame( 
        vector<vector<F>> entries,
        std::shared_ptr<vector<std::string>> labels
        ):
        mEntries(entries),
        mLabels(labels),
        mColumnCount(labels->size())
    {
    }

    void setLabels(std::shared_ptr<vector<std::string>> labels)
    { 
        mLabels = labels;
        mColumnCount = labels->size();
    }

    int getColumnCount()
    {
        return mColumnCount;
    }
    
    //appends data element to ith vector
    void addRow(vector<F> row)
    {
        mEntries.emplace_back(row);
    }

    const vector<F>& getRow(int i) const
    {
        return mEntries.at(i);
    }

    int size() const
    {
        return mEntries.size();
    }

    std::shared_ptr<vector<std::string>> getLabels()
    {
        return mLabels;
    }

    void print()
    {
        std::cout << mEntries.size() << " entries" << std::endl;

        for(auto vec : mEntries)
        {
            for(auto e : vec)
            {
                std::cout << e << " ";
            }
            std::cout << std::endl;
        }
    }

    std::shared_ptr<DataFrame<F>> getSlice(uint32_t start, uint32_t end)
    {
        if( !inRange(start) || !inRange(end) ||
            !(start < end))   
        {
            LOG << "invalid slice parameters end: " << end 
                << " start: " << start << LOG_ENDL;
            return nullptr;
        }

        vector<vector<F>> newDf;

        for(int i = start; i < end; i++)
        {
            newDf.emplace_back(getRow(i));
        }

        return std::make_shared<DataFrame<F>>(newDf,mLabels);

    }

    std::shared_ptr<DataFrame<F>> splitData(int targetCol)
    {
        vector<vector<F>> newDf(mEntries.size(),vector<F>(mColumnCount-1,0));
        for(int col = 0; col < mColumnCount; col++)
        {
            if(col != targetCol)
            {
                for(int row = 0; row < mEntries.size(); row++)
                {
                    newDf[row][col] = mEntries[row][col];
                }
            }
        }

        return std::make_shared<DataFrame<F>>(newDf,mLabels);
    }

    std::shared_ptr<vector<F>> splitTargets(int targetCol)
    {
        vector<F> targets;
        for(int col = 0; col < mColumnCount; col++)
        {
            if(col == targetCol)
            {
                for(int row = 0; row < mEntries.size(); row++)
                {
                    targets.push_back(mEntries[row][col]);
                }
            }
        }

        return std::make_shared<vector<F>>(targets);
    }   

    bool inRange(int val)
    {
        return val >= 0 && val < mEntries.size();
    }

    F getMaxInCol(int i)
    {
        double max = std::numeric_limits<double>::lowest();

        for(auto entry : mEntries)
        {
           if(entry[i] > max)
               max = entry[i];
        }

        return static_cast<F>(max);
    }

    F getMinInCol(int i)
    {
        double min = std::numeric_limits<double>::max();

        for(auto entry : mEntries)
        {
           if(entry[i] < min)
               min = entry[i];
        }

        return static_cast<F>(min);
    }

    void basicNormalizeCol(int i)
    {
        F min = getMinInCol(i);
        F max = getMaxInCol(i);

        for(int row = 0; row < mEntries.size();row++)
        {
            F ele = mEntries[row][i];
            if(false&&min == 0)
            {
                mEntries[row][i] = (ele + 1)/max;
            }
            else
            {
                mEntries[row][i] = (ele - min)/max;
            }
        }
    }

    void normalize(int targetCol)
    {
        for(int i = 0; i < mColumnCount;i++)
        {
            if(targetCol != i)
            {
                basicNormalizeCol(i);
            }
        }
    }
    
private:
    // row - col
    vector<vector<F>> mEntries;
    
    std::shared_ptr<vector<std::string>> mLabels;

    int mColumnCount;
};

#endif
