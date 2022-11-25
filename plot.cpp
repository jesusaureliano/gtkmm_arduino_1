#include "plot.h"

#include <boost/format.hpp>
#include <iostream>
#include <random>

double mapa2(double x, double dm, double dM, double rm, double rM) { return rm + (x - dm) / (dM - dm) * (rM - rm); }

Plot::Plot(Axes a, std::vector<Traza>& ts)
    : ancho(0)
    , alto(0)
    , m_t_span(20)
    , trazas(ts)
    , axes(a)
{
    std::vector<std::pair<double, double>> pts;
}

Plot::~Plot() {}

void Plot::draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
    ancho = width;
    alto = height;

    cr->set_source_rgb(0, 0, 0);
    cr->set_line_width(2);
    cr->rectangle(0, 0, width, height);

    cr->stroke();

    // Draw Ticks
    ticks(cr, width, height);

    for(Traza tr : trazas)
        tr.draw(cr, axes, width, height);

}

void Plot::f2l(double& xr, double& yr)
{
    xr = mapa2(xr, 0, ancho, axes.xmin, axes.xmax);
    yr = mapa2(yr, 0, alto, axes.ymin, axes.ymax);
}

void Plot::updateAxes(double x0, double y0, double x1, double y1)
{
    pila_axes.push(axes);

    f2l(x0, y0);
    f2l(x1, y1);

    if(x0 <= x1) {
        axes.xmin = x0;
        axes.xmax = std::min(x1, axes.xmax);
    } else {
        axes.xmin = std::max(x1, axes.xmin);
        axes.xmax = x0;
    }

    if(y0 <= y1) {
        axes.ymin = y0;
        axes.ymax = std::min(y1, axes.ymax);
    } else {
        axes.ymax = y0;
        axes.ymin = std::max(y1, axes.ymin);
    }
    
}

Axes& Plot::get_axes_ref(){
    return axes;
}


void Plot::pop_axes()
{
    if(!pila_axes.empty()) {
        axes = pila_axes.top();
        pila_axes.pop();
    }
}

void Plot::ticks(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
    cr->save();
    double xf, yf;
    std::ostringstream ss;
    cr->scale(1, -1);
    cr->set_font_size(14);
    Cairo::TextExtents te;

    
    // Verticales

    double dx = (axes.xmax - axes.xmin) / 10;

    for(double x = axes.xmin; x <= axes.xmax; x += dx) {

        xf = mapa2(x, axes.xmin, axes.xmax, 0, width);
        cr->move_to(xf, 0);
        cr->line_to(xf, -10);

        ss << boost::format("%4.2f") % x;
        cr->get_text_extents(ss.str(), te);
        cr->move_to(xf - te.width / 2, 16);

        cr->show_text(ss.str().c_str());
        ss.str("");
    }
    cr->stroke();
    
    std::vector<double> segs{ 3.0, 10.0 };
    cr->set_line_width(0.8);
    cr->set_dash(segs, 10);

    for(double x = axes.xmin; x <= axes.xmax; x += dx) {
        xf = mapa2(x, axes.xmin, axes.xmax, 0, width);
        cr->move_to(xf, 0);
        cr->line_to(xf, -height);
    }

    cr->stroke();

    cr->unset_dash();
    cr->set_line_width(2);

    double dy = (axes.ymax - axes.ymin) / 10;
    double ymedia = (axes.ymax + axes.ymin) / 2;
    
    // horizontales
    for(double y = ymedia - 5 * dy; y <= ymedia + 5 * dy; y += dy) {
        yf = -mapa2(y, axes.ymin, axes.ymax, 0, height);
        cr->move_to(0, yf);
        cr->line_to(10, yf);

        ss << boost::format("%4.2f") % y;
        cr->get_text_extents(ss.str(), te);
        cr->move_to(-te.width - 10, yf + te.height / 2);

        cr->show_text(ss.str().c_str());
        ss.str(""); // para borrar el stream
    }
    cr->stroke();

    cr->set_line_width(0.8);
    cr->set_dash(segs, 10);

    for(double y = ymedia - 5 * dy; y <= ymedia + 5 * dy; y += dy) {
        yf = -mapa2(y, axes.ymin, axes.ymax, 0, height);
        cr->move_to(0, yf);
        cr->line_to(width, yf);
    }

    cr->stroke();

    cr->restore();
}

void Plot::add_traza(Traza t){
    trazas.push_back(t);
}
