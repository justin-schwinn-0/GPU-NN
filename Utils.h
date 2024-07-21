#ifndef UTILS_H 
#define UTILS_H

#include "Logger.h"

class Utils
{
public:
    template<class T>
    void print2DVector(T t, const std::string& outerMsg = "",
                            const std::string& innerMsg = "")
    {
        for(auto inner : t)
        {
            std::cout << outerMsg << std::endl;
            for(auto elem : inner)
            {
                std::cout << "\t"<< innerMsg << elem << std::endl;
            }
        }
    }

    template<class T>
    void printVector(T t,const std::string& msg = "")
    {
        std::cout << msg;

        for(auto ele : t)
        {
            std::cout << " "<< ele;
        }
        std::cout << std::endl;
    }

    template<class T>
    void printShape(T t)
    {
        std::cout << t.size() << std::endl;

        for(int i  = 0; i < t.size(); i++)
        {
            std::cout << "\t"<< t[i].size() << std::endl;
        }
    }
    template<class T>
    int maxIndex(const vector<T> vec)
    {
        double max = -std::numeric_limits<double>::max();
        int maxIndex = -1;

        for(int i = 0; i< vec.size(); i++)
        {
            if(vec[i] > max)
            {
                max = vec[i];
                maxIndex = i;
            }
        }

        return maxIndex;
    }
};

#endif
