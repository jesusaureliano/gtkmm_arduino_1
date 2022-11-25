#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "areadibujar.h"


#include <gtkmm.h>

class MainWindow : public Gtk::Window
{
    // Signal handler:
    bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);

    void on_puerto_dialog();
    void on_puerto_desconectar();
    
protected:
    // Child widgets
    Gtk::Box m_Box;
    Gtk::Grid grid;
    Gtk::Box m_BoxVert1;

    // Signal handlers:
    void on_btnClosed_toggled();
    void on_btnIniciar_clicked();
    void on_btnConectar_clicked();

    // Child widgets:
    Gtk::CheckButton m_btnClosed;
    Gtk::Button m_btnIniciar;
    Gtk::Button m_btnConectar;

    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup;
    Glib::RefPtr<Gio::SimpleAction> m_refActionRain;

    Gtk::Label m_labelPuerto;
    Gtk::Entry m_entryPuerto;
    
    Gtk::Statusbar m_statusbar;
    
    Gtk::Dialog * m_puertoDlg;
    
public:
    MainWindow(const Glib::RefPtr<Gtk::Application>& app);
    ~MainWindow();

    AreaDibujar canvas;
};

#endif // MAINWINDOW_H
