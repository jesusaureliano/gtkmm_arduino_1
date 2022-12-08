#ifndef AREADIBUJAR_H
#define AREADIBUJAR_H

#include <gtkmm.h>
#include <vector>

#include "traza.h"
#include "serial.h"
#include "plot.h"
#include "monitor.h"

typedef std::pair<double,double> pair_t;

class AreaDibujar: public Gtk::DrawingArea
{
    double intervalo = 3;
    bool clicked = false;
    bool clicked_monitor = false;
    
    Cairo::Matrix m_matriz;
    
    double x0,y0,x1,y1;
    
    Axes axes;
    std::vector<Traza> trazas;
    
    Plot plot;
    
protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
    
    void on_mouse_pressed(int n_press, double x, double y);
    void on_mouse_released(int n_press, double x, double y);
    void on_mouse_motion(double x, double y);
    
    
public:
    AreaDibujar();
    ~AreaDibujar();

    inline void pop_axes(){ plot.pop_axes();};

    Monitor monitor;
    void add_to_trazas(std::vector<double>);
    void limpiar_trazas();
    
};

#endif // AREADIBUJAR_H
