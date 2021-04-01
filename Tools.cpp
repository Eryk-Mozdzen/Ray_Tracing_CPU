#include "Tools.h"

int randomInt(int minimum, int maximum) {
    return rand()%(maximum-minimum+1) + minimum;
}

double randomFloat(double minimum, double maximum) {
    return ((static_cast<double>(rand())/static_cast<double>(RAND_MAX))*(maximum-minimum) + minimum);
}

int sgn(double number) {
    return (number>0.0001) ? 1 : ((number<0.0001) ? -1 : 0);
}


