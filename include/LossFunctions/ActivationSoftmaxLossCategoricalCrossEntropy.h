#ifndef ACTIVATIONSOFTMAXLOSSCATEGORICALCROSSENTROPY_H
#define ACTIVATIONSOFTMAXLOSSCATEGORICALCROSSENTROPY_H

#include <Eigen/Dense>
#include "../ActivationFunctions/ActivationSoftmax.h"
#include "LossCategoricalCrossEntropy.h"

class ActivationSoftmaxLossCategoricalCrossEntropy {
    
    public:
        ActivationSoftmaxLossCategoricalCrossEntropy();
        ActivationSoftmax* getActivationFunction();
        LossCategoricalCrossEntropy* getLossFunction();

        void forward(Eigen::MatrixXd* inputs);
        Eigen::MatrixXd* getOutput() const;
        double calculate(Eigen::VectorXi* yTrue);
        Eigen::MatrixXd* getDinputs() const;
        void backward(Eigen::MatrixXd* yPredicted, Eigen::VectorXi* yTrue);

    private:
        ActivationSoftmax activation;
        LossCategoricalCrossEntropy loss;
        Eigen::MatrixXd* dinputs;

};

#endif