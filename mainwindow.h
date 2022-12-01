#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "areadibujar.h"
#include "parametrosdlg.h"
#include <thread>
#include <gtkmm.h>

class MainWindow : public Gtk::Window
{
    // Signal handler:
    bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);

    void on_puerto_dialog();
    void on_puerto_desconectar();

    void on_parameters_dialog();

    std::string filename;
protected:
    // Child widgets
    std::string str_params;
    Gtk::Box m_Box;
    Gtk::Grid grid;
    
    void on_file_dialog(Gtk::FileChooser::Action);
    
    // Signal handlers:
    void on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog);

    void on_btnIniciar_clicked();
    void on_btnConectar_clicked();
    void on_archivo_nuevo();
    void on_archivo_guardar();
    void on_archivo_guardarc();
    void on_archivo_abrir();

    bool delete_dlg(Gtk::Dialog * p);
    // Child widgets:
    Gtk::Button m_btnIniciar;
    Gtk::Button m_btnConectar;

    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup;
    Glib::RefPtr<Gio::SimpleAction> m_refActionRain;

    Gtk::Label m_labelPuerto;
    Gtk::Entry m_entryPuerto;

    Gtk::Statusbar m_statusbar;

public:
    MainWindow(const Glib::RefPtr<Gtk::Application>& app);
    ~MainWindow();

    void set_str_params(std::string str);

    AreaDibujar canvas;
};

#endif // MAINWINDOW_H
