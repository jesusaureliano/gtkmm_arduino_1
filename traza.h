#ifndef TRAZA_H
#define TRAZA_H

#include <gtkmm.h>
#include <vector>
#include <tuple>

#include "axes.h"

class Traza
{
    int id;
    double ancho;
    double red, green, blue;
    std::vector<std::pair<double,double>> puntos;
public:
    Traza();
    Traza(std::vector<std::pair<double,double>>);
    ~Traza();
    
    void draw(const Cairo::RefPtr<Cairo::Context>& cr, Axes axes, int width, int height);
    void add_punto(double x, double y);
    std::pair<double,double> last_point();
    bool hay_puntos();
    void clear();
    void set_puntos(std::vector<std::pair<double,double>> pts);
    void set_width_color(double, double, double, double);
};

#endif // TRAZA_H
