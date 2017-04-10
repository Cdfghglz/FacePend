#include "headers/pendulum.h"
#include <iostream>

template <typename T>
Pendulum<T>::Pendulum() { };

template <typename T>
Pendulum<T>::Pendulum(int len, double mass) : length_(len), mass_(mass) { };

template<typename T>
void Pendulum<T>::reset(double theta_init) {
    this->x_[0] = theta_init;
    this->x_[1] = 0;
}

template <typename T>
void Pendulum<T>::setLen(int len) {
    this->length_ = len;
}

template<typename T>
Point<T> Pendulum<T>::getEndPosition(){

    Point<T> pos;
    pos.x = (T)(sin(x_[0])*length_);
    pos.y = (T)(-cos(x_[0])*length_);

    pos.x += center_.x;
    pos.y += center_.y;

    return pos;
}

template<typename T>
void Pendulum<T>::setCenter(Point<T> center){
    center_ = center;
}

template<typename T>
void Pendulum<T>::setAccel(Point<double> centerAccel) {
    x_[1] = 0;
}

template<typename T>
void Pendulum<T>::setCenter(Point<T> center, Point<double> centerAccel) {
    setCenter(center);
    centerAccel_ = centerAccel;
}

template<typename T>
Pendulum<T>::System::System(const Pendulum* pendulum) : pendulum(pendulum) { };

template<typename T>
void Pendulum<T>::System::operator()(state_type &x, state_type &dxdt, double t){

    Point<double> L = {pendulum->length_ * sin(x[0]), -pendulum->length_ * cos(x[0])};
    Point<double> G = {0, -pendulum->g_/pendulum->length_ * sin(x[0])};

    dxdt[0] = x[1];
    dxdt[1] = G.y  - sgn(x[1]) * 0.5 +
            + 100 * L.y * pendulum->centerAccel_.x +
            + 100 * L.x * pendulum->centerAccel_.y ;
}

template<typename T>
void Pendulum<T>::step() {

    Pendulum::System syst(this);
    solverRk4_.do_step(syst, x_ , 0.0, this->dt_);

}
