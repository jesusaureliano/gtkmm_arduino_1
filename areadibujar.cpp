#include "areadibujar.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

AreaDibujar::AreaDibujar()
    : x0(0)
    , y0(0)
    , x1(0)
    , y1(0)
    , axes(0, 20, -15, 15)
    , plot(axes, trazas)
    , monitor(axes, trazas)
    , serial(nullptr)
{
    set_size_request(800, 450);
    set_draw_func(sigc::mem_fun(*this, &AreaDibujar::on_draw));

    sigc::slot<bool()> mi_slot = sigc::mem_fun(*this, &AreaDibujar::on_timeout);
    Glib::signal_timeout().connect(mi_slot, 100, Glib::PRIORITY_HIGH_IDLE);

    Glib::RefPtr<Gtk::GestureClick> gestureClick = Gtk::GestureClick::create();
    gestureClick->signal_released().connect(sigc::mem_fun(*this, &AreaDibujar::on_mouse_released));
    gestureClick->signal_pressed().connect(sigc::mem_fun(*this, &AreaDibujar::on_mouse_pressed));
    add_controller(gestureClick);

    Glib::RefPtr<Gtk::EventControllerMotion> controllerMotion = Gtk::EventControllerMotion::create();
    controllerMotion->signal_motion().connect(sigc::mem_fun(*this, &AreaDibujar::on_mouse_motion));
    add_controller(controllerMotion);

    Traza tr1, tr2, tr3;

    // 1a traza
    tr1.set_width_color(1, 0, 1, 0.5);
    trazas.push_back(tr1);

    // 2a traza
    tr2.set_width_color(1, 1, 0, 0);
    trazas.push_back(tr2);

    // 3a traza
    tr3.set_width_color(1, 1, 0, 1);
    trazas.push_back(tr3);

    /*
     *  Colocar los ejes
     */

    plot.setAxes(axes);

    axes.xmax = 100;
    monitor.setAxes(axes);
}

AreaDibujar::~AreaDibujar() {}

void AreaDibujar::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
    cr->set_source_rgb(1, 1, 1);
    cr->paint();

    cr->translate(100, height - 100);
    cr->scale(1, -1);

    cr->get_matrix(m_matriz);

    plot.draw(cr, width - 200, height - 200);

    auto is_between = [](double n, double m, double M) { return (m <= n) && (n <= M); };

    if(clicked && is_between(x0, 0, width - 200) && is_between(y0, 0, height - 200)) {
        cr->save();
        cr->rectangle(0, 0, width - 200, height - 200);
        cr->clip();
        cr->set_source_rgba(0.2, 0.2, 0.9, 0.1);
        cr->rectangle(x0, y0, x1 - x0, y1 - y0);
        cr->fill();
        cr->restore();
    }

    cr->save();
    cr->translate(0, height - 190);
    monitor.draw(cr, width - 200, height - 200);
    cr->restore();
}

bool AreaDibujar::on_timeout()
{
    if(serial != nullptr && serial->is_reading()) {
        std::queue<std::vector<double>> datos = serial->leer_datos();
        std::vector<double> dato;

        if(datos.empty())
            return true;

        while(!datos.empty()) {

            dato = datos.front();
            datos.pop();

            add_to_trazas(dato);
        }
        monitor.adding_points(true);
    } else {
        // Es importante para fijar
        monitor.adding_points(false);
    }

    queue_draw();

    /*
     * Si se retorna falso, el handle (la función) se desconecta de las señales timeout
     */
    return true;
}

void AreaDibujar::on_mouse_pressed(int n_press, double x, double y)
{
    if(x >= 100 && x <= 800 && y >= 100 && y <= 485) {

        clicked = true;

        m_matriz.invert();
        m_matriz.transform_point(x, y);

        x0 = x;
        y0 = y;
    } else {
        clicked = false;
    }

    clicked_monitor = monitor.is_in_rect(x, y);
}

void AreaDibujar::on_mouse_motion(double x, double y)
{
    bool redibujar = false;

    if(clicked) {
        m_matriz.invert();
        m_matriz.transform_point(x, y);

        x1 = x;
        y1 = y;

        redibujar = true;
    }

    Axes ejes;

    if(clicked_monitor) {
        ejes = monitor.desplazar_rect(x, y);
        plot.setAxes(ejes);
        redibujar = true;
    }

    if(redibujar)
        queue_draw();
}

void AreaDibujar::on_mouse_released(int n_press, double x, double y)
{
    if(clicked) {

        clicked = false;

        m_matriz.invert();
        m_matriz.transform_point(x, y);

        x1 = x;
        y1 = y;

        if(x1 != x0 && y1 != y0)
            plot.updateAxes(x0, y0, x1, y1);

        x0 = x1 = x;
        y0 = y1 = y;
    }

    monitor.fin_desplazar();
    clicked_monitor = false;
}

void AreaDibujar::crear_serial(std::string name)
{
    if(serial == nullptr) {
        try {
            // pasar una referencia inmediatamente al area de dibujo
            serial = new Serial(name.c_str());
        } catch(std::exception& e) {
            std::cout << "Error al crear el puerto serie : " << e.what() << std::endl;
        }
    }
}

void AreaDibujar::limpiar_trazas()
{
    for(Traza traza : trazas) {
        traza.clear();
    }
}

void AreaDibujar::leer_puerto(bool b)
{
    if(serial != nullptr) {
        serial->leer_puerto(b);
    }
}

void AreaDibujar::borrar_serial()
{
    if(serial != nullptr) {
        delete serial;
        serial = nullptr;
    }
}

void AreaDibujar::add_to_trazas(std::vector<double> v)
{
    static double t_prev = 0;

    if(v[0] >= t_prev) {

        for(std::size_t i = 0; i < trazas.size(); i++) {
            trazas[i].add_punto(v[0], v[i + 1]);
        }

        /////////////////////////////////////////////
        //
        // Ajustar el intervalo de plot

        double plot_interval = 20;
        Axes & axplot = plot.get_axes_ref();
        if(v[0] > plot_interval) {
            axplot.xmin = v[0] - plot_interval;
            axplot.xmax = v[0];
        } else {
            axplot.xmin = 0;
            axplot.xmax = plot_interval;
        }

        /////////////////////////////
        //
        //  Ajustar el intervalo de monitor

        double monitor_interval = 100;
        Axes & axmonitor = monitor.get_axes_ref();
        if(v[0] > monitor_interval) {
            axmonitor.xmin = v[0] - monitor_interval;
            axmonitor.xmax = v[0];
        } else {
            axmonitor.xmin = 0;
            axmonitor.xmax = monitor_interval;
        }
        //monitor.setAxes(axes);

    } else {
        if(v[0] == 0) {
            for(Traza traza : trazas) {
                traza.clear();
            }
        }
    }

    t_prev = v[0];
}
