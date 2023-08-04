#include "../include/DenseLayer.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::RowVectorXd;

DenseLayer::DenseLayer(int numInputs, int numNeurons){
    input = nullptr;
    output = nullptr;

    weights = new MatrixXd(numInputs, numNeurons);
    *weights = MatrixXd::Random(numInputs, numNeurons);

    biases = new RowVectorXd(1, numNeurons);
    *biases = RowVectorXd::Zero(1, numNeurons);

    dinputs = nullptr;
    dweights = new MatrixXd(numInputs, numNeurons);
    dbiases = new RowVectorXd(1, numNeurons);

    weightMomentum = new MatrixXd(numInputs, numNeurons);
    *weightMomentum = MatrixXd::Zero(numInputs, numNeurons);
    biasMomentum = new RowVectorXd(1, numNeurons);
    *biasMomentum = RowVectorXd::Zero(1, numNeurons);

    weightCache = new MatrixXd(numInputs, numNeurons);
    *weightCache = MatrixXd::Zero(numInputs, numNeurons);
    biasCache = new RowVectorXd(1, numNeurons);
    *biasCache = RowVectorXd::Zero(1, numNeurons);
}

ostream& operator<<(ostream& os, const DenseLayer& layer) {
    os << "Weights:\n" << layer.getWeights() << std::endl << "Biases:\n" << layer.getBiases() << endl;
    return os;
}

MatrixXd* DenseLayer::getWeights() const {
    return weights;
}

void DenseLayer::setWeights(MatrixXd* newWeights) {
    *weights = *newWeights;
}

void DenseLayer::updateWeights(MatrixXd* weightsUpdate) {
    *weights += *weightsUpdate;
}

RowVectorXd* DenseLayer::getBiases() const {
    return biases;
}

void DenseLayer::setBiases(RowVectorXd* newBiases) {
    *biases = *newBiases;
}

void DenseLayer::updateBiases(RowVectorXd* biasesUpdate) {
    *biases += *biasesUpdate;
}


void DenseLayer::forward(MatrixXd* in) {
    input = in;
    if(!output) output = new MatrixXd(input->rows(), weights->cols());
    *output = ((*input) * (*weights)).rowwise() + (*biases);
}

MatrixXd* DenseLayer::getOutput() const {
    return output;
}

void DenseLayer::backward(MatrixXd* dvalues) {
    *dweights = input->transpose() * (*dvalues); // y don't we noramlize for sample size????

    *dbiases = dvalues->colwise().sum();

    if(!dinputs) dinputs = new MatrixXd(dvalues->rows(), weights->rows());
    *dinputs = (*dvalues) * weights->transpose();
}

MatrixXd* DenseLayer::getDinputs() const {
    return dinputs;
}

MatrixXd* DenseLayer::getDweights() const {
    return dweights;
}

RowVectorXd* DenseLayer::getDbiases() const {
    return dbiases;
}

MatrixXd* DenseLayer::getWeightMomentum() const {
    return weightMomentum;
}

void DenseLayer::setWeightMomentum(MatrixXd* newWeightMomentum) {
    *weightMomentum = *newWeightMomentum;
}

RowVectorXd* DenseLayer::getBiasMomentum() const {
    return biasMomentum;
}

void DenseLayer::setBiasMomentum(RowVectorXd* newBiasMomentum) {
    *biasMomentum = *newBiasMomentum;
}

MatrixXd* DenseLayer::getWeightCache() const {
    return weightCache;
}

void DenseLayer::setWeightCache(MatrixXd* update) {
    *weightCache = *update;
}

void DenseLayer::updateWeightCache(MatrixXd* update) {
    *weightCache += *update;
}

RowVectorXd* DenseLayer::getBiasCache() const {
    return biasCache;
}

void DenseLayer::setBiasCache(RowVectorXd* update) {
    *biasCache = *update;
}

void DenseLayer::updateBiasCache(RowVectorXd* update) {
    *biasCache += *update;
}