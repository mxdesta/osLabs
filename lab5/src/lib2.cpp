#include <cmath>
#include "../include/functions.h"

extern "C" {
    float Pi(int K) {
    double pi = 1.0; 
    for (int i = 1; i <= K; i++) {
        pi *= (2.0 * i / (2.0 * i - 1)) * (2.0 * i / (2.0 * i + 1));
    }
    return static_cast<float>(pi * 2);
    }

    float Square(float A, float B) {
        return 0.5 * A * B; // Площадь прямоугольного треугольника
    }
}
