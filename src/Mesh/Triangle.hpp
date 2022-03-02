#pragma once

#include <Hinae/Vector2.hpp>
#include <Hinae/Point3.hpp>
#include <Hinae/Point2.hpp>

using namespace Hinae;

template <arithmetic T>
struct Barycentric_Coordinates
{
    T alpha, beta, gamma;

    Barycentric_Coordinates(T f1, T f2, T f3) :
        alpha(f1), beta(f2), gamma(f3) {}
    
    bool is_inside() const
    {
        return alpha >= ZERO<T> && beta >= ZERO<T> && gamma >= ZERO<T>;
    }

    T correct(T z1, T z2, T z3)
    {
        assert(!is_zero(z1) && !is_zero(z2) && !is_zero(z3));
        alpha /= z1;
        beta  /= z2;
        gamma /= z3;

        return reciprocal(alpha + beta + gamma);
    }

    template <typename U>
    U interpolated(const U& t1, const U& t2, const U& t3, T w) const
    {
        assert(!std::isnan(w));
        return (t1 * alpha + t2 * beta + t3 * gamma) * w;
    }

    Point2<T> interpolated(const Point2<T>& t1, const Point2<T>& t2, const Point2<T>& t3, T w) const
    {
        return
        {
            (alpha * t1.x + beta * t2.x + gamma * t3.x) * w,
            (alpha * t1.y + beta * t2.y + gamma * t3.y) * w
        };
    }

    Point3<T> interpolated(const Point3<T>& t1, const Point3<T>& t2, const Point3<T>& t3, T w) const
    {
        return
        {
            (alpha * t1.x + beta * t2.x + gamma * t3.x) * w,
            (alpha * t1.y + beta * t2.y + gamma * t3.y) * w,
            (alpha * t1.z + beta * t2.z + gamma * t3.z) * w
        };
    }
};

template <arithmetic T>
struct Triangle
{
    Point3<T> A, B, C;

    Triangle() = default;
    auto operator <=> (const Triangle<T>&) const = default;

    Triangle(const Point3<T>& p1, const Point3<T>& p2, const Point3<T>& p3) :
        A(p1), B(p2), C(p3) {}

    Barycentric_Coordinates<T> barycentric2D(const Point2<T>& P) const
    {
        const Vector2<T> AB = as<Point2, T>(B) - as<Point2, T>(A);
        const Vector2<T> BC = as<Point2, T>(C) - as<Point2, T>(B);

        const T S_ABC = cross(AB, BC);

        const Vector2<T> AP = P - as<Point2, T>(A); 
        const Vector2<T> BP = P - as<Point2, T>(B); 
        const Vector2<T> CP = P - as<Point2, T>(C); 

        const T beta  = cross(CP, AP) / S_ABC;
        const T gamma = cross(AP, BP) / S_ABC;
        const T alpha = ONE<T> - beta - gamma;
        // const T alpha = cross(BP, CP) / S_ABC;
        return {alpha, beta, gamma};
    }
};

template <arithmetic T>
std::ostream& operator << (std::ostream& os, const Triangle<T>& v)
{
    os << "Triangle:\n" << v.A << v.B << v.C;
    return os;
}
