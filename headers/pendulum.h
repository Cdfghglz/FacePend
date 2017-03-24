#ifndef QT_PENDULUM_PENDULUM_H
#define QT_PENDULUM_PENDULUM_H

#include <cmath>
#include "headers/common.h"

template <typename T>
class Pendulum {

protected:
    const double g_ = 9.81;
    const double steps_ = 0.01;
    double d_damping_ = 0.02;
    double s_damping_ = 0.009;
    int length_;
    double mass_;
    double theta_ = 0;
    double theta_d_ = 0;
	Point<T> center_ = {0, 0};
    Point<double> state_ = {0, 0};

public:
    Pendulum();
    Pendulum(int len, double mass);

    void setLen(int theta_init);
    void reset(double theta_init);
    void increment(double crtl_torque);
	Point<T> getPosition();
	void setCenter(Point<T> center);

    //todo: make the dynamical equations memebers of a more generic class e.g. DynamicalSystem, solvable by a generic
    //solver
    //todo: put set of equations with arguments into a contaier of functions to be fed to a generic solver
    double eq1(double arg[]);
    double eq2(double arg[]);

    double udot(double theta, double theta_d);
};

template <typename T>
double solver(Pendulum<T>* pend, double args[], double var, double steps);

#endif //QT_PENDULUM_PENDULUM_H
