#ifndef AXES_H
#define AXES_H

class Axes
{
public:
    Axes(double, double, double, double);
    Axes();
    ~Axes();

    double xmin, ymin, xmax, ymax;
    
};

#endif // AXES_H
