#ifndef SPIRAL_H
#define SPIRAL_H

#include <iostream>
#include <Eigen/Dense>

class Spiral {

    public:
        Spiral(int numSamples, int numClasses);
        friend std::ostream& operator<<(std::ostream& os, const Spiral& dataset);
        Eigen::MatrixX2d getX() const;
        Eigen::VectorXd getY() const;
        
    private:
        Eigen::MatrixX2d X;
        Eigen::VectorXd Y;

};

#endif