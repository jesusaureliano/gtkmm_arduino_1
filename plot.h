#ifndef PLOT_H
#define PLOT_H

#include "axes.h"
#include "traza.h"

#include <gtkmm.h>
#include <vector>

class Plot
{

protected:
    int ancho, alto;

    double m_t_span;

    Gtk::DrawingArea* drawingarea;

    std::vector<Traza>& trazas;

    void f2l(double&, double&);

    void ticks(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

public:
    Plot(Axes, std::vector<Traza>&);
    ~Plot();

    Axes axes;
    std::stack<Axes> pila_axes;

    virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
    void pausar(bool);

    void pop_axes();
    void updateAxes(double, double, double, double);
    Axes& get_axes_ref();
    void add_traza(Traza);

    inline void setAxes(Axes a)
    {
        axes = a;
        m_t_span = std::abs(a.xmax-a.xmin);
    }
};

#endif // PLOT_H
