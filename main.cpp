#include "mainwindow.h"

#include <gtkmm.h>

int main(int argc, char* argv[])
{
    Glib::set_init_to_users_preferred_locale(false);
    auto app = Gtk::Application::create("org.gtkmm.examples.base");

    return app->make_window_and_run<MainWindow>(argc, argv, app);
}