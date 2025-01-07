#include <cmath>
#include "../include/functions.h"

extern "C" {
    float Pi(int K) {
        float pi = 0.0;
        for (int i = 0; i < K; i++) {
            pi += pow(-1, i) / (2 * i + 1);
        }
        return pi * 4;
    }

    float Square(float A, float B) {
        return A * B; // Площадь прямоугольника
    }
}
