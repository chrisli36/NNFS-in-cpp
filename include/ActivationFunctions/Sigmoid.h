#ifndef SIGMOID_H
#define SIGMOID_H

#include <Eigen/Dense>
#include "../ModelWrappers/Layer.h"
#include "../ModelWrappers/Activation.h"

class Sigmoid : public Activation {

    public: 
        Sigmoid();
        std::string getName() const override;

        void forward(Eigen::MatrixXd* in) override;
        Eigen::MatrixXd* getOutput() const override;
        Eigen::MatrixXd getPredictions() const override;

        void backward(Eigen::MatrixXd* dvalues) override;
        Eigen::MatrixXd* getDinputs() const override;
        
    private:
        Eigen::MatrixXd* output;
        Eigen::MatrixXd* dinputs;

};

#endif