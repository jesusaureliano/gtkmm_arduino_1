#include "monitor.h"

double map(double x, double dm, double dM, double rm, double rM) { return rm + (x - dm) / (dM - dm) * (rM - rm); }

Monitor::Monitor(Axes a, std::vector<Traza>& ts)
    : Plot(a, ts)
{
}

Monitor::~Monitor() {}

void Monitor::draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{

    height *= 0.15;

    cr->set_source_rgb(0, 0, 0);
    cr->set_line_width(2);
    cr->rectangle(0, 0, width, height);

    cr->stroke();

    for(Traza tr : trazas)
        tr.draw(cr, axes, width, height);

    m_rect.x = offset * 0.8 * width;
    m_rect.y = 0;
    m_rect.ancho = 0.2 * width;
    m_rect.alto = height;

    cr->set_source_rgba(0, 0.8, 0, 0.1);
    cr->rectangle(m_rect.x, m_rect.y, m_rect.ancho, m_rect.alto);
    cr->fill();

    if(!desplazando) {
        cr->get_matrix(m);
        m.invert();
        px0 = 0;
        py0 = 0;
        m.transform_point(px0, py0);

        px1 = 0.8 * width;
        py1 = 0;
        m.transform_point(px1, py1);

        if(trazas[0].hay_puntos()) {
            if(is_adding_points) {
                offset = (trazas[0].last_point().first - 20) / 80; // 100 - ancho del rect
                offset = std::min(std::max(0.0, offset), 1.0);
            }
        } else {
            offset = 0;
        }
    }

    cr->translate(m_rect.x, m_rect.y);
    cr->get_matrix(matriz);
}

void Monitor::adding_points(bool a) { is_adding_points = a; }

bool Monitor::is_in_rect(double x, double y)
{

    Cairo::Matrix mat(matriz);
    mat.invert();
    mat.transform_point(x, y);

    bool res = x >= 0 && x <= 140 && y >= 0 && y <= 58;

    if(res) {
        desplazando = true;
        x0 = x;
        y0 = y;
    }

    return res;
}

Axes Monitor::desplazar_rect(double x, double y)
{
    Axes res;
    Cairo::Matrix mat(m);
    // mat.invert();
    mat.transform_point(x, y);

    if(desplazando) {
        offset = (x - x0) / (px1 - px0);
        offset = std::max(0., std::min(1.0, offset));

        res.xmin = axes.xmin + offset * 0.8 * (axes.xmax - axes.xmin);
        res.xmax = res.xmin + 20;
        res.ymin = axes.ymin;
        res.ymax = axes.ymax;
    }

    return res;
}

void Monitor::fin_desplazar() { desplazando = false; }
