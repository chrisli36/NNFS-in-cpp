#include "../../include/Optimizers/RMSProp.h"

using Eigen::MatrixXd;
using Eigen::RowVectorXd;

RMSProp::RMSProp(double lr, double dr, double e, double r) {
    learningRate = lr;
    currentLearningRate = lr;
    decayRate = dr;
    iteration = 0;
    epsilon = e;
    rho = r;
}

double RMSProp::getLearningRate() {
    return currentLearningRate;
}

void RMSProp::decay() {
    currentLearningRate = learningRate * (1 / (1 + decayRate * iteration));
}

void RMSProp::updateParameters(Dense* layer) {
    MatrixXd newWeightCache = (rho * layer->getWeightCache()->array()) + ((1 - rho) * layer->getDweights()->array().square());
    RowVectorXd newBiasCache = (rho * layer->getBiasCache()->array()) + ((1 - rho) * layer->getDbiases()->array().square());

    layer->setWeightCache(&newWeightCache);
    layer->setBiasCache(&newBiasCache);

    MatrixXd weightUpdate = (-currentLearningRate * *layer->getDweights()).array()
        / (MatrixXd::Constant(layer->getWeights()->rows(), layer->getWeights()->cols(), epsilon).array() + layer->getWeightCache()->array().sqrt());
    RowVectorXd biasUpdate = (-currentLearningRate * *layer->getDbiases()).array()
        / (RowVectorXd::Constant(layer->getBiases()->rows(), layer->getBiases()->cols(), epsilon).array() + layer->getBiasCache()->array().sqrt());

    layer->updateWeights(&weightUpdate);
    layer->updateBiases(&biasUpdate);
}

void RMSProp::incrementIteration() {
    iteration++;
}