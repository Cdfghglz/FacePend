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
Pendulum<T>::System::System(const Pendulum* pendulum) : pendulum(pendulum) { };

template<typename T>
void Pendulum<T>::System::operator()(state_type &x, state_type &dxdt, double t){
    dxdt[0] = x[1];
    dxdt[1] = - pendulum->g_/pendulum->length_ * sin(x[0]);
}

template<typename T>
void Pendulum<T>::step() {

    Pendulum::System syst(this);
    solverRk4_.do_step(syst, x_ , 0.0, this->dt_);

}
