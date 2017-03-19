#ifndef QT_PENDULUM_COMMON_H
#define QT_PENDULUM_COMMON_H

template <typename T>
struct Point{
    T x, y;
};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif //QT_PENDULUM_COMMON_H
