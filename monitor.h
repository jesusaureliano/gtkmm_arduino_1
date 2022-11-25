#ifndef MONITOR_H
#define MONITOR_H

#include "plot.h"

class Monitor : public Plot
{
protected:
    
    struct _RECT{
        double x, y;
        double ancho, alto;
    } m_rect;
    
    Cairo::Matrix matriz;
    Cairo::Matrix m;
    
    double offset = 0;
    bool desplazando = false;
    bool is_adding_points = false;
    // donde se hace click, relativo al rectangulo verde
    double x0, y0;
    // para el desplazamiento del rectangulo
    double px0, py0, px1, py1;
    
public:
    Monitor(Axes,std::vector<Traza>& ts);
    ~Monitor();
    
    bool is_in_rect(double, double);
    Axes desplazar_rect(double, double);
    void adding_points(bool);
    void fin_desplazar();
    
    virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;

};

#endif // MONITOR_H
