#ifndef PARAMETROSDLG_H
#define PARAMETROSDLG_H

#include <gtkmm.h>

class MainWindow;

class ParametrosDlg : public Gtk::Dialog
{
    MainWindow& refMainWin;
    std::string str_params;
    
    Gtk::Label m_lblNumEntradas;
    Gtk::Entry* m_ptrNumEntradas;

    Gtk::Label m_lblControlador;

    Gtk::Label m_lblNumerador;
    Gtk::Entry* m_ptrNumerador;

    Gtk::Label m_lblDenominador;
    Gtk::Entry* m_ptrDenominador;


    Gtk::Label m_lblReferencia;
    Gtk::ComboBox m_Combo;

    Gtk::Label m_lblMuestreo;
    Gtk::Entry* m_ptrMuestreo;
    
    Gtk::Label m_lblAmplitud;
    Gtk::Entry* m_ptrAmplitud;
    
    Gtk::Label m_lblPeriodo;
    Gtk::Entry* m_ptrPeriodo;
    
    
    Gtk::Button *m_btnDesdeArduino;
    Gtk::Button *m_btnHaciaArduino;
    
    Gtk::CheckButton *m_chkLazo;
    Gtk::CheckButton *m_chkControlador;
    Gtk::CheckButton *m_chkDiscreto;
    
protected:
    // Signal handlers
    void on_hacia_arduino();
    void on_desde_arduino();
    void on_toggled_discreto();
    void on_update_str_params();
    // Tree model columns
    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:
        ModelColumns()
        {
            add(m_col_id);
            add(m_col_name);
        }

        Gtk::TreeModelColumn<int> m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    };

    ModelColumns m_Columns;

    // Child widgets
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

public:
    ParametrosDlg(const Glib::ustring& title, Gtk::Window& parent, bool modal = false, bool use_header_bar = false);
    ~ParametrosDlg();
    
    void set_parametros(std::string);
    std::string get_parameters();
};

#endif // PARAMETROSDLG_H
