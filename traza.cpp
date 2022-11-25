#include "traza.h"

#include <iostream>
#include <map>

double mapa(double x, double dm, double dM, double rm, double rM) { return rm + (x - dm) / (dM - dm) * (rM - rm); }

Traza::Traza(std::vector<std::pair<double, double>> pts) { puntos = pts; }

Traza::Traza() {}

Traza::~Traza() {}

void Traza::set_puntos(std::vector<std::pair<double, double>> pts)
{
    if(!puntos.empty())
        puntos.clear();
    puntos = pts;
}

void Traza::add_punto(double x, double y) { 
    puntos.emplace_back(std::pair<double, double>(x, y)); 
    /*if(puntos.size()>1000){
        puntos.erase(puntos.begin(),puntos.end()-1000);
    }*/
}

std::pair<double, double> Traza::last_point() { return puntos.back(); }

bool Traza::hay_puntos() { return !puntos.empty(); }

void Traza::clear() { puntos.erase(puntos.begin(), puntos.end()); }

void Traza::set_width_color(double lw, double r, double g, double b)
{
    ancho = lw;
    red = r;
    green = g;
    blue = b;
}

void Traza::draw(const Cairo::RefPtr<Cairo::Context>& cr, Axes axes, int width, int height)
{
    double x, y;

    double xm = axes.xmin;
    double xM = axes.xmax;
    double ym = axes.ymin;
    double yM = axes.ymax;

    if(puntos.empty())
        return;

    std::pair<double, double> pt, pt0, pt1;
    std::map<double, double> datos;

    for(std::vector<std::pair<double, double>>::iterator it = puntos.begin(); it < puntos.end(); it++) {
        pt0 = *it;
        pt1 = *(it+1);
        if(std::abs(pt0.first - pt1.first) < 0.1) {
            datos[pt0.first] = pt0.second;
            datos[pt1.first] = pt1.second;
        }
    }

    cr->save();

    cr->set_line_width(ancho);
    cr->set_source_rgb(red, green, blue);

    cr->rectangle(0, 0, width, height);
    cr->clip();

    std::map<double, double>::iterator it_m = datos.begin();
    pt = *it_m;
    x = mapa(pt.first, xm, xM, 0, width);
    y = mapa(pt.second, ym, yM, 0, height);
    cr->move_to(x, y);

    for(auto pta : datos) {
        x = mapa(pta.first, xm, xM, 0, width);
        y = mapa(pta.second, ym, yM, 0, height);
        cr->line_to(x, y);
    }

    cr->set_source_rgb(red, green, blue);
    cr->stroke();

    cr->restore();
}
