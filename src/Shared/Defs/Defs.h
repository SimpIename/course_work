#ifndef DEFS_H
#define DEFS_H

#include <limits.h>
#include <stdint.h>
#include <float.h>
#include <atomic>

/// Макрос запрета операции копирования класса

#   define DISABLE_COPY(Class) \
        Class& operator=(const Class&) = delete; \
        Class           (const Class&) = delete \

/// Макрос для исключения предупреждений
/// компиляторов о неиспользуемых переменных

#   define UNUSED(param) (void)param

/// Макросы платформ

#   if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#       define OS_WIN64
#       define OS_WIN32
#   elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#       define OS_WIN32
#   elif defined(__linux__) || defined(__linux)
#       define OS_LINUX
#   elif (defined(__APPLE__) && defined(__GNUC__))
#       define OS_MACOS
#  endif

#   if defined(OS_WIN32) || defined(OS_WIN64)
#       define OS_WIN
#   endif

/// Макрос экпорта функции или класса из библиотеки

#   ifdef _MSC_VER
#        define LIBRARY_EXPORT __declspec(dllexport)
#        define LIBRARY_LOCAL
#   elif defined(__GNUC__)
#       ifdef OS_WIN32
#           define LIBRARY_EXPORT __declspec(dllexport)
#           define LIBRARY_LOCAL
#        else
#           define LIBRARY_EXPORT __attribute__((visibility("default")))
#           define LIBRARY_LOCAL __attribute__((visibility("hidden")))
#       endif
#   endif


/// Макрос стандартной функции для регистрации логгера в библиотеке

#define REGISTER_LOGGER                                                    \
    extern "C" LIBRARY_EXPORT                                              \
    void registerLogger(std::shared_ptr<spdlog::logger> p_global_logger) { \
        spdlog::register_logger(p_global_logger);                          \
    }

/// Тип задающий позицию элемента в массиве

    using Apos = uint32_t;
#   define APOS_MAX UINT32_MAX

/// Расширенный вариант типа Apos

using Exapos = uint64_t;

/// Тип вещественного числа

typedef double Real;

#   define REAL_MIN -DBL_MAX
#   define REAL_MAX DBL_MAX
#   define REAL_EPSILON DBL_EPSILON

/// Тип вещественного числа для геометрических операций

typedef double GReal;

/// Число Пи

#   define PI 3.1415926535897932384626433832795028841971

namespace mat
{
/// Перевод из градусов в радианы

    template<typename _AngleType>
    inline constexpr _AngleType degreesToRadians(_AngleType p_angle)
    {
        return p_angle * PI / 180.;
    }

/// Перевод из радиан в градусы

    template<typename _AngleType>
    inline constexpr _AngleType radiansToDegrees(_AngleType p_angle)
    {
        return p_angle * 180. / PI;
    }

/// Возведение в квадрат

    template<typename _Type>
    inline constexpr auto sqr(_Type p_value) -> decltype(p_value * p_value)
    {
        return p_value * p_value;
    }

/// Минимум для двух элементов

    template<typename _Type>
    inline constexpr _Type min(_Type p_value1, _Type p_value2)
    {
        return p_value1 < p_value2 ? p_value1 : p_value2;
    }

/// Максимум для двух элементов

    template<typename _Type>
    inline constexpr _Type max(_Type p_value1, _Type p_value2)
    {
        return p_value1 > p_value2 ? p_value1 : p_value2;
    }

/// Минимум для произвольного числа элементов

    template<typename _Type, typename... _Types>
    inline constexpr _Type min(_Type p_value, _Types... p_values)
    {
        return min(p_value, min(p_values...));
    }

/// Максимум для произвольного числа элементов

    template<typename _Type, typename... _Types>
    inline constexpr _Type max(_Type p_value, _Types... p_values)
    {
        return max(p_value, max(p_values...));
    }

/// Абсолютное значение

    template<typename _Type>
    inline constexpr _Type abs(_Type p_value)
    {
        return p_value >= 0 ? p_value : -p_value;
    }

/// Сравнение двух значения типа Real с учетом погрешности

    inline constexpr bool compare(Real p_value1, Real p_value2)
    {
        return (abs(p_value1 - p_value2) * 1000000000000. <= min(abs(p_value1), abs(p_value2)));
    }
}

namespace misc {
struct ThreadSafeCounter {
    ThreadSafeCounter() = delete;
    using value_type = unsigned long long;

    static value_type getNext()
    {
        static std::atomic<value_type> last_id{ 0 };
        return ++last_id;
    }
};
}

#endif // DEFS_H
