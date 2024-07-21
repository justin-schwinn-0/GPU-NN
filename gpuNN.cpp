#include "Logger.h"

#include "CsvReader.h"
#include "DataFrame.h"
#include "NeuralNet.h"
#include "Layer.h"

#include "Utils.h"

#include <cmath>

template<typename T>
T relu(T val)
{
    if(val > 0)
        return val;
    return 0;
}

template<typename T>
T reluDx(T val)
{
    if(val > 0)
        return 1;
    return 0;
}

template<typename T>
T sigmoid(T val)
{
    return (1 / (1 + exp(-val)));
}

// sigVal is the return value from sigmoid(val);
template<typename T>
T sigmoidDx(T sigVal)
{
    return (sigVal * (1 - sigVal));
}

// sigVal is the return value from sigmoid(val);
template<typename T>
T tanh(T val)
{
    return (exp(val) - exp(-val))/(exp(val) + exp(-val));
}

template<typename T>
T tanhDx(T tanhVal)
{
    return 1 - (tanhVal * tanhVal);
}

template<typename T>
T identity(T val)
{
    return val;
}

// sigVal is the return value from sigmoid(val);
template<typename T>
T identityDx(T sigVal)
{
    return static_cast<T>(1);
}

typedef float dataType;

struct dataConfig
{
    std::string filename;
    int inputs;
    int targetCol;
    std::vector<LayerConfig<dataType>>& nnConfig;
    dataType learningRate;
};


int main()
{
    //  first, read the CSV
    //      if possible, find a way to read it that lets us
    //      get direct access to the raw pointer arrays to send them to the GPU easily
    //      alternatively, find a way to send that data in a well encapsulated way
    //
    //  second, set up the NN as a single threaded process (copied directly from the python file)
    //      
    //      Momentum after?
    //
    //      epochs
    //
    //  third multithread the NN
    //
    //  Fourth, run the NN on GPU!    
    



    ActivationPair<dataType> reluPair = {relu<dataType>,reluDx<dataType>};
    ActivationPair<dataType> sigmoidPair = {sigmoid<dataType>,sigmoidDx<dataType>};
    ActivationPair<dataType> tanhPair = {tanh<dataType>,tanhDx<dataType>};
    ActivationPair<dataType> identityPair = {identity<dataType>,identityDx<dataType>};
    
    Utils utils;

    std::vector<LayerConfig<dataType>> heartTest =
    {
        {2,tanhPair}
    };
    std::vector<LayerConfig<dataType>> cardioTest =
    {
        {3,tanhPair},
        {2,tanhPair}
    };
    std::vector<LayerConfig<dataType>> mnistTest =
    {
        {10,sigmoidPair}
    };

    dataConfig cardioConfig = 
    {
        "datasets/cardio_train.csv",
        11,
        11,
        cardioTest,
        0.06
    };

    dataConfig heartConfig = 
    {
        "datasets/heart.csv",
        13,
        13,
        heartTest,
        0.01
    };

    dataConfig mnistConfig = 
    {
        "datasets/mnist_test.csv",
        28*28,
        0,
        mnistTest,
        0.1
    };

    dataConfig& usedConfig = heartConfig;



    auto reader = std::make_unique<CsvReader>(usedConfig.filename);
    auto df = reader->populate<dataType>();
    df->normalize(usedConfig.targetCol);

    auto nn = std::make_shared<NeuralNet<dataType>>(
            usedConfig.inputs,
            usedConfig.nnConfig,
            usedConfig.learningRate);

    nn->randomInit();

    //auto usedDf = df->getSlice(0,100);
    auto usedDf = df;

    auto data = usedDf->splitData(usedConfig.targetCol);
    auto targets = usedDf->splitTargets(usedConfig.targetCol);

    if(&usedConfig != &mnistConfig)
    {
        nn->printNet();
    }

    nn->train(*data,*targets);
    nn->evaluate(*data,*targets);

    if(&usedConfig != &mnistConfig)
    {
        nn->printNet();
    }


    return 0;
}
