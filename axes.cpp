#include "axes.h"

Axes::Axes(double xm, double xM, double ym, double yM)
{
    xmin = xm;
    ymin = ym;
    
    xmax = xM;
    ymax = yM;
}

Axes::Axes(){
    xmin = ymin = xmax = ymax = 0;
}

Axes::~Axes()
{
}

