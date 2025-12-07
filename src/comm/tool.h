#pragma once

#include <concepts>
#include <random>

namespace Tool
{


template<std::integral T>
T random_range(T l, T r)
{
    static thread_local std::mt19937 gen{ std::random_device{}() };
    std::uniform_int_distribution<T> dis(l, r);
    return dis(gen);
}

template<std::floating_point T>
T random_range(T l, T r)
{
    static thread_local std::mt19937 gen{ std::random_device{}() };
    std::uniform_real_distribution<T> dis(l, r);
    return dis(gen);
}

template<typename T>
concept IsVec2 = requires(T t)
{
    {t.x};
    {t.y};
};

template<typename T>
concept IsVec3 = IsVec2<T> && requires(T t)
{
    {t.z};
};


template<IsVec2 T>
T random_range(const T& l, const T& r)
{
    T v;
    v.x = random_range(l.x, r.x);
    v.y = random_range(l.y, r.y);
    return v;
}


template<IsVec3 T>
T random_range(const T& l, const T& r)
{
    T v;
    v.x = random_range(l.x, r.x);
    v.y = random_range(l.y, r.y);
    v.z = random_range(l.z, r.z);
    return v;
}


}