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

    Point() {};

    Point(T x0, T y0) {
        x = x0;
        y = y0;
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

    Point<float> toFloat() {
        Point<float> ret;
        ret.x = (float) this->x;
        ret.y = (float) this->y;
        return ret;
    };

	Point<double> toDouble() {
        Point<double> ret;
        ret.x = (double) this->x;
        ret.y = (double) this->y;
        return ret;
    };
};

//template<typename T>
//Point<float> Point<T>::toFloat() {
//    this->x = (float)this->x;
//    this->y = (float)this->y;
//}

// slow, use CBuf instead
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

template <typename T>
class CBuf {
private:
    const unsigned long size;
    unsigned long itr = 0;
    std::vector<T> buf;

public:
    CBuf() : size(0) {};
    CBuf(const unsigned long size) : size(size) {
        std::vector<T> b(size);
        buf = b;
    };
    T operator[] (const int pos) {
        return buf[(itr + pos) % size];
    };
    void push(T item) {
        buf.at((++itr + size - 1) % size) = item;
    };
};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif //QT_PENDULUM_COMMON_H
