#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <vector>
#include <functional>
#include <cmath>
#include <random>

#include "Utils.h"

using std::vector;

template<typename F>
struct ActivationPair
{
    std::function<F(F)> fun;
    std::function<F(F)> dx;
};

template<typename F>
struct LayerConfig
{
    int size;
    ActivationPair<F> act;
};

template<typename F>
class Layer
{
public:
    Layer(int size,int prevLayerSize)
    {
        mWeights = vector<vector<F>>(size,vector<F>(prevLayerSize+1,1));
        mOutputs = vector<F>(size,0);
        mDeltas = vector<F>(size,0);
    }

    void calculateOutput(const vector<F>& inputs)
    {
        for(int n = 0; n < mWeights.size();n++)
        {
            // add bias to sum first
            F sum = mWeights[n][0];
            for(int i = 1; i < mWeights[n].size();i++)
            {
                sum += inputs[i-1] * mWeights[n][i];
            }
            auto activeOut =mActivation(sum);
            mOutputs[n] = activeOut;
        }
    }
    
    //calculate delta if layer is output
    void calculateOutputDeltas(F target)
    {
        for(int n = 0; n < mOutputs.size();n++)
        {
            //mDeltas[n] = (mOutputs[n] - target );
            if(n == target)
            {
                F diff = (mOutputs[n] -1);
                mDeltas[n] = mActivationDx(mOutputs[n]) * diff * diff;
            }
            else
            {
                F diff = (mOutputs[n] - 0);
                mDeltas[n] = -mActivationDx(mOutputs[n]) * diff * diff;
            }
        }
    }

    void calculateDeltas(
        const vector<F>& forwardDeltas,
        const vector<vector<F>>& forwardWeights)
    {
        for(int n = 0; n < mWeights.size();n++)
        {
            F weightSum = 0;

            for(int fn = 0; fn < forwardWeights.size(); fn++)
            {
                //                              +1 to account for bias
                weightSum += forwardWeights[fn][n+1] * forwardDeltas[fn];
            }

            mDeltas[n] = mActivationDx(mOutputs[n]) * weightSum;
        }
    }

    void changeInputWeights(F lr, const vector<F>& inputs)
    {
        for(int n = 0; n < mWeights.size();n++)
        {
            mWeights[n][0] -= lr * mDeltas[n];
            for(int w = 1; w < mWeights[n].size();w++)
            {
                mWeights[n][w] += lr * mDeltas[n] * inputs[n];
            }
        }
    }

    void changeWeights(F lr)
    {
        for(int n = 0; n < mWeights.size();n++)
        {
            mWeights[n][0] -= lr * mDeltas[n];
            for(int w = 1; w < mWeights[n].size();w++)
            {
                mWeights[n][w] += lr * mDeltas[n] * mOutputs[n];
            }
        }
    }

    void printLayerWeights(const std::string msg = "")
    {
        for(auto neuron : mWeights)
        {
            utils.printVector(neuron,msg);
        }
    }

    void setActivation(std::function<F(F)> act)
    {
        mActivation = act;
    }

    void setActivationDx(std::function<F(F)> dx)
    {
        mActivationDx = dx;
    }

    vector<F>& getOutputs()
    {   return mOutputs; }

    vector<F>& getDeltas()
    {   return mDeltas; }

    vector<vector<F>>& getWeights()
    {   return mWeights; }

    void randomInit()
    {
        for(int n = 0; n < mWeights.size();n++)
        {
            for(int w = 0; w < mWeights[n].size();w++)
            {
                mWeights[n][w] = static_cast<F>(rand() / static_cast<F>(RAND_MAX));
            }
        }
    }

private:
    vector<F> mDeltas;
    vector<F> mOutputs;
    vector<vector<F>> mWeights;
    std::function<F(F)> mActivation;
    std::function<F(F)> mActivationDx;

    Utils utils;   
};// end layer

#endif
