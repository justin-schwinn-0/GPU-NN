#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include <vector>
#include <initializer_list>
#include <functional>

#include <cmath>

#include "DataFrame.h"
#include "Utils.h"
#include "Layer.h"

using std::vector;

// template the percision
template<class F>
class NeuralNet
{
public:
    NeuralNet(int inputSize,const std::vector<LayerConfig<F>>& layers,F lr)
    {
        if(layers.size() <= 0)
        {
            LOG << "Layer config has no elements!" << LOG_ENDL;
            return;
        }

        int prevLayerSize = inputSize;
        for(int i = 0; i < layers.size(); i++)
        {
            Layer<F> layer(layers[i].size, prevLayerSize);

            layer.setActivation(layers[i].act.fun);
            layer.setActivationDx(layers[i].act.dx);

            mLayers.push_back(layer);
            prevLayerSize = layers[i].size;
        }

        mLearningRate = lr;
    }

    void backwardsPass(
         const vector<F>& data,
         const F target)
    {
        forwardPass(data);
       
        // DO DELTAS
        mLayers[mLayers.size()-1].calculateOutputDeltas(target);

        for(int l = mLayers.size()-2; l >= 0; l--)
        {
            mLayers[l].calculateDeltas(mLayers[l+1].getDeltas(),mLayers[l+1].getWeights());
        }

        //DO weight change
        mLayers[0].changeInputWeights(mLearningRate,data);

        for(int l = 1; l < mLayers.size();l++)
        {
            mLayers[l].changeWeights(mLearningRate);
        }
    }

    void forwardPass(const vector<F>& data)
    {
        if(mLayers.size() <= 0)
        {
            LOG << "neural net has 0 layers!" << LOG_ENDL;
            return;
        }

        // input layer
        mLayers[0].calculateOutput(data);

        // hidden/output layers
        for(int l = 1; l < mLayers.size(); l++)
        {
            mLayers[l].calculateOutput(mLayers[l-1].getOutputs());
        }

    }

    void train(const DataFrame<F>& data, const vector<F>& targets)
    {
        if(data.size() != targets.size())
        {
            LOG << "data and targets size mismatch!" << data.size() 
                << " != " << targets.size() << LOG_ENDL;
            return;
        }
        auto& outputRow = mLayers[mLayers.size()-1];

        for(int i = 0; i < data.size(); i++)
        {
            backwardsPass(data.getRow(i),targets[i]);
            printNetDeltas();
            utils.printVector(outputRow.getOutputs(),std::to_string(static_cast<int>(targets[i])) + ":: ");
            std::cout << std::endl;
        }
        std::cout << "training complete!" << std::endl;
    }

    void evaluate(const DataFrame<F>& data,const vector<F>& targets)
    {

        int accurate = 0;
        auto& outputRow = mLayers[mLayers.size()-1];
        for(int i = 0; i < data.size(); i++)
        {
            // for each data row, do a forward pass
            // then get the highest output from the output layer
            // if the highest output is the same index as the target value of the data point
            // add 1 to accuracy and total score
            forwardPass(data.getRow(i));
            int chosenOut = utils.maxIndex(outputRow.getOutputs());
            //utils.printVector(outputRow.getOutputs(), std::to_string(chosenOut) + " = " + std::to_string(targets[i]) + " :");
            accurate += (chosenOut == targets[i]);
        }

        std::cout << "accuracy: " << accurate << "/" << data.size() << " " 
            << (static_cast<double>(accurate)/data.size())<< std::endl;
    }

    void randomInit()
    {
        srand (static_cast <unsigned> (time(0)));
        for(int l = 0; l < mLayers.size();l++)
        {
            mLayers[l].randomInit();
        }
    }

    void printNet()
    {
        for(auto l : mLayers)
        {
            std::cout << "Layer: " << std::endl;
            l.printLayerWeights("\t");
        }
    }

    void printNetOutputs()
    {
        for(auto l : mLayers)
        {
            utils.printVector(l.getOutputs(),"outputs:\t");
        }
    }

    void printNetDeltas()
    {
        for(auto l : mLayers)
        {
            utils.printVector(l.getDeltas(),"deltas:\t");
        }
    }
private:
    std::vector<Layer<F>> mLayers;
    F mLearningRate = 0.01;

    Utils utils;   
};

#endif
