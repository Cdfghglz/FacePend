#include "headers/pendulum.h"
#include <iostream>

template <typename T>
Pendulum<T>::Pendulum() { };
template <typename T>
Pendulum<T>::Pendulum(int len, double mass) : length_(len), mass_(mass) { };

template <typename T>
void Pendulum<T>::reset(double theta_init) {
    this->theta_ = theta_init;
    this->theta_d_ = 0;
}

template <typename T>
void Pendulum<T>::setLen(int len) {
    this->length_ = len;
}

template <typename T>
Point<T> Pendulum<T>::getPosition(){

    Point<T> pos;
    pos.x = (T)(-sin(theta_)*length_);
    pos.y = (T)(cos(theta_)*length_);

    pos.x += center_.x;
    pos.y += center_.y;

    return pos;
}

template <typename T>
void Pendulum<T>::setCenter(Point<T> center){
    center_ = center;
}

template <typename T>
double solver(Pendulum<T>* pend, double args[], double var, double steps){
    //todo: implement generalization accepting any system of equations from DynamicalSystem class, Pendulum will be
    //subclass of...
    double k1, k2, k3, k4;

    double k1a[] = {args[0], args[1]};
    k1 = pend->eq1(k1a);
    double k2a[] = {args[0] + 0.5 * steps * k1, args[1]};
    k2 = pend->eq1(k2a);
    double k3a[] = {args[0] + 0.5 * steps * k2, args[1]};
    k3 = pend->eq1(k3a);
    double k4a[] = {args[0] + 0.5 * steps * k1, args[1]};
    k4 = pend->eq1(k4a);

    return var + (steps/6) * (k1 + 2 * k2 + 2 * k3 + k4);
}

template <typename T>
void Pendulum<T>::increment(double crtl_torque) {

    double  ku1, ku2, ku3, ku4, unext, theta_d, theta;


    double args[] = {theta_d_, theta_};

    theta = theta_;
    theta_d = theta_d_;

//Numerical integration of equation 2
    ku1 = udot(theta, theta_d);
    ku2 = udot(theta + 0.5 * steps_ * ku1, theta_d);
    ku3 = udot(theta + 0.5 * steps_ * ku2, theta_d);
    ku4 = udot(theta + steps_ * ku3, theta_d);
    unext = theta_d + (steps_ / 6) * (ku1 + 2 * ku2 + 2 * ku3 + ku4);

// solve eq1
    theta_ = solver(this, args, args[1], steps_);
    theta_d_ = unext;

}

//simplified equations 1
template <typename T>
double Pendulum<T>::eq1(double arg[])
{
    return arg[0];
}
//simplified equations 2
template <typename T>
double Pendulum<T>::eq2(double arg[])
{
    return -(g_ / length_) * sin(arg[0]) - d_damping_*arg[1] - sgn(arg[1])*s_damping_;
}

//simplified equations 2
template <typename T>
double Pendulum<T>::udot(double theta, double theta_d)
{
    return -(g_ / length_) * sin(theta) - d_damping_*theta_d - sgn(theta_d)*s_damping_;
}

