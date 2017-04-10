#ifndef QT_PENDULUM_COMMON_H
#define QT_PENDULUM_COMMON_H

#include <vector>
#include <iostream>

template <typename T>
struct Point{
    T x, y;

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        std::cout << "P: " << p.x << ", "<< p.y;
    };

    Point<T> operator+ (int val) {
        x += val;
        y += val;
        return *this;
    };

    Point<T> operator+ (Point<T>& p) {
        x += p.x;
        y += p.y;
    };

    Point<T>& operator= (const Point<T>& p) {
        x = p.x;
        y = p.y;
    };
};

// todo: slow, reimplement
template <typename T>
class CircBuf : public std::vector<T> {
public:
    CircBuf() {};
    CircBuf(const CircBuf& circBuf) : std::vector<T>(circBuf) { };
    CircBuf& operator=(const CircBuf& circBuf) {
        std::vector<T>::operator=(circBuf);
    };
    CircBuf(const unsigned long int size) : std::vector<T>(size) { };
    void push(T val) {
		this->erase(this->begin());
        this->push_back(val);
    };
};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif //QT_PENDULUM_COMMON_H
