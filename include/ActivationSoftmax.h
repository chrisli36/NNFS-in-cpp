#ifndef ACTIVATIONSOFTMAX_H
#define ACTIVATIONSOFTMAX_H

#include <iostream>
#include <Eigen/Dense>

class ActivationSoftmax {

    public:
        ActivationSoftmax();
        Eigen::MatrixXd getOutput() const;
        Eigen::MatrixXd getDinputs() const;
        void forward(Eigen::MatrixXd inputs);
        void backward(Eigen::MatrixXd dvalues);
        
        Eigen::MatrixXd output;

    private:
        Eigen::MatrixXd dinputs;

};

#endif