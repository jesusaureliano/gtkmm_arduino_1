#include "parametrosdlg.h"

#include "mainwindow.h"

#include <cstdlib>
#include <iostream>

ParametrosDlg::ParametrosDlg(const Glib::ustring& title, Gtk::Window& parent, bool modal, bool use_header_bar)
    : Gtk::Dialog(title, parent, modal, use_header_bar)
    , refMainWin((MainWindow&)parent)
    , str_params("")
    , m_lblControlador("CONTROLADOR :")
    , m_lblProceso("PROCESO :")
    , m_lblNumerador("Numerador :")
    , m_lblNum_Proc("Numerador :")
    , m_lblDenominador("Denominador :")
    , m_lblDen_Proc("Denominador :")
    , m_lblReferencia("Referencia :")
    , m_lblMuestreo("T :")
    , m_lblAmplitud("Amplitud :")
    , m_lblPeriodo("Período :")
{
    set_title("Parametros");

    // Create the tree model
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_Combo.set_model(m_refTreeModel);

    auto iter = m_refTreeModel->append();
    auto row = *iter;
    row[m_Columns.m_col_id] = 1;
    row[m_Columns.m_col_name] = "escalón";
    m_Combo.set_active(iter);

    iter = m_refTreeModel->append();
    auto childrow = *iter;
    childrow[m_Columns.m_col_id] = 2;
    childrow[m_Columns.m_col_name] = "rampa";

    iter = m_refTreeModel->append();
    childrow = *iter;
    childrow[m_Columns.m_col_id] = 3;
    childrow[m_Columns.m_col_name] = "senoidal";

    // m_Combo.pack_start(m_Columns.m_col_id);
    m_Combo.pack_start(m_Columns.m_col_name);

    // Connect signal handler:

    Gtk::Grid* grid = Gtk::manage(new Gtk::Grid());

    m_ptrNumerador = Gtk::manage(new Gtk::Entry());
    m_ptrDenominador = Gtk::manage(new Gtk::Entry());
    m_ptrNum_Proc = Gtk::manage(new Gtk::Entry());
    m_ptrDen_Proc = Gtk::manage(new Gtk::Entry());

    m_ptrMuestreo = Gtk::manage(new Gtk::Entry());
    m_ptrMuestreo->set_sensitive(false);

    m_ptrAmplitud = Gtk::manage(new Gtk::Entry());
    m_ptrPeriodo = Gtk::manage(new Gtk::Entry());

    m_btnDesdeArduino = Gtk::manage(new Gtk::Button("Desde Arduino"));
    m_btnDesdeArduino->set_expand();
    m_btnHaciaArduino = Gtk::manage(new Gtk::Button("Hacia Arduino"));
    m_btnHaciaArduino->set_expand();

    m_btnHaciaArduino->signal_clicked().connect(sigc::mem_fun(*this, &ParametrosDlg::on_hacia_arduino));
    m_btnDesdeArduino->signal_clicked().connect(sigc::mem_fun(*this, &ParametrosDlg::on_desde_arduino));

    m_chkLazoCerrado = Gtk::manage(new Gtk::CheckButton("Lazo Cerrado"));
    m_chkPlantaSimulada = Gtk::manage(new Gtk::CheckButton("Planta Simulada"));
    m_chkDiscreto = Gtk::manage(new Gtk::CheckButton("Controlador Discreto"));
    m_chkEstaCorriendo = Gtk::manage(new Gtk::CheckButton("Esta Corriendo"));

    m_chkPlantaSimulada->signal_toggled().connect(sigc::mem_fun(*this, &ParametrosDlg::on_toggled_planta_simulada));
    m_chkDiscreto->signal_toggled().connect(sigc::mem_fun(*this, &ParametrosDlg::on_toggled_discreto));
    m_chkEstaCorriendo->signal_toggled().connect(sigc::mem_fun(*this, &ParametrosDlg::on_toggled_esta_corriendo));
    m_chkLazoCerrado->signal_toggled().connect(sigc::mem_fun(*this, &ParametrosDlg::on_update_str_params));

    // Actualiaciones al modificar valores
    m_ptrNumerador->signal_changed().connect(sigc::mem_fun(*this, &ParametrosDlg::on_update_str_params));
    m_ptrDenominador->signal_changed().connect(sigc::mem_fun(*this, &ParametrosDlg::on_update_str_params));
    m_ptrMuestreo->signal_changed().connect(sigc::mem_fun(*this, &ParametrosDlg::on_update_str_params));
    m_ptrAmplitud->signal_changed().connect(sigc::mem_fun(*this, &ParametrosDlg::on_update_str_params));
    m_ptrPeriodo->signal_changed().connect(sigc::mem_fun(*this, &ParametrosDlg::on_update_str_params));
    m_Combo.signal_changed().connect(sigc::mem_fun(*this, &ParametrosDlg::on_update_str_params));


    m_lblControlador.set_halign(Gtk::Align::START);
    m_lblProceso.set_halign(Gtk::Align::START);
    m_lblNum_Proc.set_halign(Gtk::Align::START);
    m_lblDen_Proc.set_halign(Gtk::Align::START);
    m_lblNumerador.set_halign(Gtk::Align::START);
    m_lblDenominador.set_halign(Gtk::Align::START);
    m_lblReferencia.set_halign(Gtk::Align::END);
    m_lblMuestreo.set_halign(Gtk::Align::START);
    m_lblAmplitud.set_halign(Gtk::Align::START);
    m_lblPeriodo.set_halign(Gtk::Align::START);

    grid->attach(*m_chkPlantaSimulada, 1, 1);
    grid->attach(m_lblProceso, 1, 2);
    grid->attach(m_lblNum_Proc, 1, 3);
    grid->attach(*m_ptrNum_Proc, 1, 4);
    grid->attach(m_lblDen_Proc, 1, 5);
    grid->attach(*m_ptrDen_Proc, 1, 6);
    grid->attach(*m_chkLazoCerrado, 1, 7);
    grid->attach(*m_chkEstaCorriendo, 1, 8);
    
    grid->attach(m_lblReferencia, 2, 1);
    grid->attach(m_lblControlador, 2, 2);
    grid->attach(m_lblNumerador, 2, 3);
    grid->attach(*m_ptrNumerador, 2, 4);
    grid->attach(m_lblDenominador, 2, 5);
    grid->attach(*m_ptrDenominador, 2, 6);
    grid->attach(*m_chkDiscreto, 2, 7);

    grid->attach(m_Combo, 3, 1);
    grid->attach(m_lblMuestreo, 3, 2);
    grid->attach(*m_ptrMuestreo, 3, 3);
    grid->attach(m_lblAmplitud, 3, 4);
    grid->attach(*m_ptrAmplitud, 3, 5);
    grid->attach(m_lblPeriodo, 3, 6);
    grid->attach(*m_ptrPeriodo, 3, 7);


    grid->attach(*m_btnHaciaArduino, 2, 8);
    grid->attach(*m_btnDesdeArduino, 3, 8);


    grid->set_margin(20);
    grid->set_row_spacing(10);
    grid->set_column_spacing(30);

    set_child(*grid);
}

ParametrosDlg::~ParametrosDlg() {}

std::string ParametrosDlg::get_parameters()
{

    auto row = *m_Combo.get_active();

    str_params = m_ptrNumerador->get_text() + " " +
                 m_ptrDenominador->get_text() // Num Den en la misma cadena b0 b1 b2 a1 a2
                 + ":" + m_ptrNum_Proc->get_text() + " " + m_ptrDen_Proc->get_text() // Igual para la planta
                 + ":" + m_ptrAmplitud->get_text() + ":" + m_ptrPeriodo->get_text() + ":" +
                 std::to_string(row[m_Columns.m_col_id]) // Referencia
                 + ":" + m_ptrMuestreo->get_text() + ":" + std::to_string(m_chkLazoCerrado->get_active()) + ":" +
                 std::to_string(m_chkDiscreto->get_active()) 
                 + ":" + std::to_string(m_chkPlantaSimulada->get_active())
                 + ":" + std::to_string(m_chkEstaCorriendo->get_active());

    return str_params;
}

void ParametrosDlg::on_update_str_params() { refMainWin.set_str_params(get_parameters()); }

void ParametrosDlg::on_hacia_arduino()
{

    std::string str_params = get_parameters();
    refMainWin.set_str_params(str_params);
    refMainWin.escribir_serial(str_params);
}

void ParametrosDlg::set_parametros(std::string str)
{
    char* ptr;
    char* strsarray[9];
    int8_t index = 0;

    ptr = strtok((char*)str.c_str(), ":");
    while(ptr != NULL || index < 9) {
        strsarray[index] = ptr;
        index++;
        ptr = strtok(NULL, ":");
    }
    
    std::string str1, str2;
    
    auto f_split = [&](std::string source){
            std::size_t pos = 0;
            for(int i=0; i<3; i++){
                pos = source.find(" ",pos+1);
            }
            str1 = source.substr(0,pos);
            str2 = source.substr(pos+1);
        };
        
    // el resultado viene en str1 y str2 (se pasan como referencias)
    f_split(strsarray[0]);    
    m_ptrNumerador->set_text(str1.c_str());
    m_ptrDenominador->set_text(str2.c_str());
    
    f_split(strsarray[1]);    
    m_ptrNum_Proc->set_text(str1.c_str());
    m_ptrDen_Proc->set_text(str2.c_str());
    
    m_ptrAmplitud->set_text(strsarray[2]);
    m_ptrPeriodo->set_text(strsarray[3]);

    // referencia
    unsigned int indice_señal = atoi(strsarray[4]);
    m_Combo.set_active(indice_señal - 1);
    
    m_ptrMuestreo->set_text(strsarray[5]);
    
    if(!strcmp(strsarray[6], "1")) {
        m_chkLazoCerrado->set_active();
    } else {
        m_chkLazoCerrado->set_active(false);
    }

    if(!strcmp(strsarray[7], "1")) {
        m_chkDiscreto->set_active();
    } else {
        m_chkDiscreto->set_active(false);
    }

    if(!strcmp(strsarray[8], "1")) {
        m_chkPlantaSimulada->set_active();
    } else {
        m_chkPlantaSimulada->set_active(false);
    }

}

void ParametrosDlg::on_desde_arduino() { refMainWin.escribir_serial("recuperar"); }

void ParametrosDlg::on_toggled_discreto()
{
    if(m_chkDiscreto->get_active()) {
        m_ptrMuestreo->set_sensitive();
    } else {
        m_ptrMuestreo->set_sensitive(false);
        m_ptrMuestreo->set_text("-1");
    }

    on_update_str_params();
}

void ParametrosDlg::on_toggled_esta_corriendo()
{
    on_update_str_params();
    on_hacia_arduino();
}

void ParametrosDlg::on_toggled_planta_simulada()
{
    // Las siguientes cadenas se utilizan para respaldar los valores de los campos
    // Numerador y Denominador de la planta cuando el usuario pulsa el checkbox repetidamente.
    static std::string str_bkup_num = m_ptrNum_Proc->get_text();
    static std::string str_bkup_den = m_ptrDen_Proc->get_text();

    if(m_chkPlantaSimulada->get_active()) {
        m_ptrNum_Proc->set_text(str_bkup_num);
        m_ptrNum_Proc->set_sensitive();
        m_ptrDen_Proc->set_text(str_bkup_den);
        m_ptrDen_Proc->set_sensitive();
    } else {
        m_ptrNum_Proc->set_text("0 0 0");
        m_ptrNum_Proc->set_sensitive(false);
        m_ptrDen_Proc->set_text("0 0");
        m_ptrDen_Proc->set_sensitive(false);
    }

    on_update_str_params();
}
