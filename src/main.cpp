#include <iostream>
#include <vector>

#include "../include/DataGeneration/lodepng.h"
#include "../include/DataGeneration/ImageGenerator.h"
#include "../include/DataGeneration/Spiral.h"

#include "../include/Layers/DenseLayer.h"
#include "../include/Layers/DropoutLayer.h"

#include "../include/ActivationFunctions/ActivationReLu.h"
#include "../include/ActivationFunctions/ActivationSoftmax.h"

#include "../include/LossFunctions/LossCategoricalCrossEntropy.h"
#include "../include/LossFunctions/ActivationSoftmaxLossCategoricalCrossEntropy.h"

#include "../include/Optimizers/OptimizerSGD.h"
#include "../include/Optimizers/OptimizerAdagrad.h"
#include "../include/Optimizers/OptimizerRMSProp.h"
#include "../include/Optimizers/OptimizerAdam.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::VectorXi;
using Eigen::RowVectorXd;
using namespace std::chrono;

// auto start = high_resolution_clock::now();

// auto stop = high_resolution_clock::now();
// auto duration = duration_cast<microseconds>(stop - start);
// std::cout << duration.count() << std::endl;

int main() {
    srand(1);

    Spiral dataset(100, 3);
    MatrixXd X = dataset.getX();
    VectorXi Y = dataset.getY();

    DenseLayer layer1 = DenseLayer(2, 64, 0.0, 0.0, 5e-4, 5e-4);
    ActivationReLu activation1 = ActivationReLu();
    DropoutLayer dropout1 = DropoutLayer(0.1);
    DenseLayer layer2 = DenseLayer(64, 3);
    ActivationSoftmaxLossCategoricalCrossEntropy activationLoss = ActivationSoftmaxLossCategoricalCrossEntropy();
    OptimizerAdam optimizer = OptimizerAdam(0.02, 1e-5);

    double loss, dataLoss, regularizationLoss;
    MatrixXd softmaxOutputs;
    VectorXd predictions = VectorXd::Zero(300);
    int matchCount;
    Eigen::Index maxRow;
    for(int epoch = 0; epoch < 10001; epoch++){

        // auto start = high_resolution_clock::now();
        // FORWARD PASS
        layer1.forward(&X);
        activation1.forward(layer1.getOutput());
        dropout1.forward(activation1.getOutput());
        layer2.forward(dropout1.getOutput());
        activationLoss.forward(layer2.getOutput());
        dataLoss = activationLoss.calculate(&Y);
        regularizationLoss = activationLoss.getLossFunction()->calculateRegularizationLoss(&layer1)
                            + activationLoss.getLossFunction()->calculateRegularizationLoss(&layer2);
        loss = dataLoss + regularizationLoss;

        // ACCURACY CALCULATION
        softmaxOutputs = *(activationLoss.getOutput());
        for(int rowIndex = 0; rowIndex < predictions.rows(); rowIndex++) {
            softmaxOutputs.row(rowIndex).maxCoeff(&maxRow);
            predictions(rowIndex) = maxRow;
        }
        matchCount = 0;
        for(int rowIndex = 0; rowIndex < predictions.rows(); rowIndex++) {
            if(predictions(rowIndex) == Y(rowIndex)) {
                matchCount++;
            }
        }

        // BACKPROPAGATION
        activationLoss.backward(activationLoss.getOutput(), &Y);
        layer2.backward(activationLoss.getDinputs());
        dropout1.backward(layer2.getDinputs());
        activation1.backward(dropout1.getDinputs());
        layer1.backward(activation1.getDinputs());

        // PARAMETER UPDATE
        optimizer.decay();
        optimizer.updateParameters(&layer1);
        optimizer.updateParameters(&layer2);
        optimizer.incrementIteration();

        // auto stop = high_resolution_clock::now();
        // auto duration = duration_cast<microseconds>(stop - start);

        if((epoch % 100) == 0) {
            cout << "Epoch: " << epoch << "\t";
            cout << "Loss: " << loss << "\t";
            cout << "Data loss: " << dataLoss << "\t";
            cout << "Regularization loss: " << regularizationLoss << "\t";
            cout << "Accuracy: " << (double) matchCount / Y.rows() << endl;
            cout << optimizer.getLearningRate() << endl;
            // std::cout << duration.count() << std::endl;
        }
    }

    //////////////////////////////////////////////////////////////////////
    ///////////////////////////// TEST DATA //////////////////////////////

    Spiral test(100, 3);
    MatrixXd XTest = test.getX();
    VectorXi YTest = test.getY();

    layer1.forward(&XTest);
    activation1.forward(layer1.getOutput());
    layer2.forward(activation1.getOutput());
    activationLoss.forward(layer2.getOutput());
    loss = activationLoss.calculate(&YTest);

    softmaxOutputs = *(activationLoss.getOutput());
    for(int rowIndex = 0; rowIndex < predictions.rows(); rowIndex++) {
        softmaxOutputs.row(rowIndex).maxCoeff(&maxRow);
        predictions(rowIndex) = maxRow;
    }
    matchCount = 0;
    for(int rowIndex = 0; rowIndex < predictions.rows(); rowIndex++) {
        if(predictions(rowIndex) == Y(rowIndex)) {
            matchCount++;
        }
    }
    cout << "Test Data: \t Loss: " << loss << "\t";
    cout << "Accuracy: " << (double) matchCount / Y.rows() << endl;
    cout << optimizer.getLearningRate() << endl;
    
    //////////////////////////////////////////////////////////////////////////
    ///////////////////////////// VISUALIZATION //////////////////////////////
    const int WIDTH = 1000;
    const int HEIGHT = 1000;
    MatrixXd inputGrid = MatrixXd(WIDTH * HEIGHT, 2);
    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            inputGrid((y * HEIGHT) + x, 0) = (x * 2.0 / WIDTH) - 1;
            inputGrid((y * HEIGHT) + x, 1) = (y * 2.0 / HEIGHT) - 1;
        }
    }

    layer1.forward(&inputGrid);
    activation1.forward(layer1.getOutput());
    layer2.forward(activation1.getOutput());
    activationLoss.forward(layer2.getOutput());

    std::vector<unsigned char> pixels(WIDTH * HEIGHT * 4); // RGBA format
    RowVectorXd pix = RowVectorXd(1, 3);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int index = 4 * (y * WIDTH + x);
            pix = activationLoss.getOutput()->row(y * HEIGHT + x);
            // green: 109, 209, 129
            // red: 255, 130, 130
            // blue: 72, 133, 232
            pixels[index + 0] = sqrt(109 * 109 * pix(0) + 255 * 255 * pix(1) + 72 * 72 * pix(2));
            pixels[index + 1] = sqrt(209 * 209 * pix(0) + 130 * 130 * pix(1) + 133 * 133 * pix(2));
            pixels[index + 2] = sqrt(129 * 129 * pix(0) + 130 * 130 * pix(1) + 232 * 232 * pix(2));
            pixels[index + 3] = 255; // Alpha channel (opacity: 255 = fully opaque)
            // pixels[4 * (y * WIDTH + x) + gridPredictions(HEIGHT * y + x, 0)] = 255;
        }
    }
    ImageGenerator gen = ImageGenerator();
    // gen.createImage(pixels, "visualizations/adam/512lr0.02dr1e-5wrdo0.1.png", WIDTH, HEIGHT);

    return 0;
}