#ifndef QT_PENDULUM_PENDULUM_H
#define QT_PENDULUM_PENDULUM_H

#include <cmath>
#include "headers/common.h"

#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

template <typename T>
class Pendulum {

protected:
	const double g_ = 9.81;
    const double dt_ = 0.1;
	// todo: reimplement
    const double steps_ = 0.05;
    double viscDamping_ = 0.5;
    double s_damping_ = 0.003;
	Point<T> center_ = {0, 0};
	Point<double> centerAccel_ = {0.0, 0.0};
    int length_;
    double mass_;
	typedef boost::array<double, 2> state_type;
	state_type x_;
	boost::numeric::odeint::runge_kutta4<state_type> solverRk4_;

public:

    Pendulum();
    Pendulum(int len, double mass);

	void setLen(int len);
	void setCenter(Point<T> center);
	void setCenter(Point<T> center, Point<double> centerAccel);
	void setAccel(Point<double> centerAccel);
	Point<T> getEndPosition();

    void reset(double theta_init);
	void step();

	friend class System;

    class System{
	private:
		const Pendulum* pendulum;
    public:
		friend class Pendulum;
		System(const Pendulum* pendulum);
		void operator()(state_type &x , state_type &dxdt , double t);
    };

};

#endif //QT_PENDULUM_PENDULUM_H
