#include <iostream>
#include <cstdlib>

#include "parametrosdlg.h"
#include "mainwindow.h"

ParametrosDlg::ParametrosDlg(const Glib::ustring& title, Gtk::Window& parent, bool modal, bool use_header_bar)
    : Gtk::Dialog(title,parent,modal,use_header_bar)
    , refMainWin((MainWindow&)parent)
    , str_params("")
    , m_lblNumEntradas("Número de Entradas")
    , m_lblControlador("CONTROLADOR -")
    , m_lblNumerador("Numerador :")
    , m_lblDenominador("Denominador :")
    , m_lblReferencia("Referencia :")
    , m_lblMuestreo("Muestreo :")
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
    
    //m_Combo.pack_start(m_Columns.m_col_id);
    m_Combo.pack_start(m_Columns.m_col_name);
    
    //Connect signal handler:
    
    Gtk::Grid* grid = Gtk::manage(new Gtk::Grid());
    
    m_ptrNumEntradas = Gtk::manage(new Gtk::Entry());
    m_ptrNumerador = Gtk::manage(new Gtk::Entry());
    m_ptrDenominador = Gtk::manage(new Gtk::Entry());
    
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
    
    m_chkLazo = Gtk::manage(new Gtk::CheckButton("Lazo Cerrado"));
    m_chkControlador = Gtk::manage(new Gtk::CheckButton("Con el controlador"));
    m_chkDiscreto = Gtk::manage(new Gtk::CheckButton("Controlador Discreto"));
    m_chkDiscreto->signal_toggled().connect(sigc::mem_fun(*this, &ParametrosDlg::on_toggled_discreto));
    
    //Actualiaciones al modificar valores
    m_ptrNumEntradas->signal_changed().connect(sigc::mem_fun(*this,&ParametrosDlg::on_update_str_params));
    m_ptrNumerador->signal_changed().connect(sigc::mem_fun(*this,&ParametrosDlg::on_update_str_params));
    m_ptrDenominador->signal_changed().connect(sigc::mem_fun(*this,&ParametrosDlg::on_update_str_params));
    m_ptrMuestreo->signal_changed().connect(sigc::mem_fun(*this,&ParametrosDlg::on_update_str_params));
    m_ptrAmplitud->signal_changed().connect(sigc::mem_fun(*this,&ParametrosDlg::on_update_str_params));
    m_ptrPeriodo->signal_changed().connect(sigc::mem_fun(*this,&ParametrosDlg::on_update_str_params));
    m_Combo.signal_changed().connect(sigc::mem_fun(*this,&ParametrosDlg::on_update_str_params));
    m_chkLazo->signal_toggled().connect(sigc::mem_fun(*this,&ParametrosDlg::on_update_str_params));
    m_chkControlador->signal_toggled().connect(sigc::mem_fun(*this,&ParametrosDlg::on_update_str_params));
    
    m_lblNumEntradas.set_halign(Gtk::Align::START);
    m_lblControlador.set_halign(Gtk::Align::START);
    m_lblNumerador.set_halign(Gtk::Align::START);
    m_lblDenominador.set_halign(Gtk::Align::START);
    m_lblReferencia.set_halign(Gtk::Align::START);
    m_lblMuestreo.set_halign(Gtk::Align::START);
    m_lblAmplitud.set_halign(Gtk::Align::START);
    m_lblPeriodo.set_halign(Gtk::Align::START);
    
    
    grid->attach(m_lblNumEntradas,1,1);
    grid->attach(*m_ptrNumEntradas,1,2);
    grid->attach(m_lblControlador,1,3);
    grid->attach(m_lblNumerador,1,4);
    grid->attach(*m_ptrNumerador,1,5);
    grid->attach(m_lblDenominador,1,6);
    grid->attach(*m_ptrDenominador,1,7);
    
    grid->attach(m_lblReferencia,2,2);
    grid->attach(m_Combo,2,3);
    grid->attach(m_lblAmplitud, 2,4);
    grid->attach(*m_ptrAmplitud,2,5);
    grid->attach(m_lblPeriodo, 2,6);
    grid->attach(*m_ptrPeriodo,2,7);
    
    grid->attach(m_lblMuestreo, 3, 1);
    grid->attach(*m_ptrMuestreo,3, 2);
    
    grid->attach(*m_btnHaciaArduino,3,3);
    grid->attach(*m_btnDesdeArduino,3,4);
    
    grid->attach(*m_chkLazo,1,8);
    grid->attach(*m_chkDiscreto,2,8);
    grid->attach(*m_chkControlador,1,9);
    
    
    grid->set_margin(20);
    grid->set_row_spacing(10);
    grid->set_column_spacing(30);
    
    set_child(*grid);
    
}

ParametrosDlg::~ParametrosDlg()
{
}


std::string ParametrosDlg::get_parameters(){
    
    auto row = *m_Combo.get_active();
    
    str_params = m_ptrNumerador->get_text()
        + ":" + m_ptrDenominador->get_text()
        + ":" + m_ptrAmplitud->get_text()
        + ":" + m_ptrPeriodo->get_text()
        + ":" + std::to_string(m_chkLazo->get_active()) //(m_chkLazo->get_active()?"cerrado":"abierto")
        + ":" + std::to_string(row[m_Columns.m_col_id])
        + ":" + std::to_string(m_chkControlador->get_active())//(m_chkControlador->get_active()?"ccontrol":"scontrol")
        + ":" + m_ptrNumEntradas->get_text()
        + ":" + std::to_string(m_chkDiscreto->get_active())
        + ":" + m_ptrMuestreo->get_text(); 
    
    return str_params;
 
}

void ParametrosDlg::on_update_str_params(){
     refMainWin.set_str_params(get_parameters());    
}

void ParametrosDlg::on_hacia_arduino(){
    
    Glib::ustring str_params = get_parameters();
    refMainWin.set_str_params(str_params);
    
    refMainWin.canvas.escribir_puerto(str_params);
    
}

void ParametrosDlg::set_parametros(std::string str){
    char * ptr;
    char * strsarray[10];
    int8_t index = 0;

    ptr = strtok((char *)str.c_str(),":");
    while(ptr != NULL || index <= 9){
        strsarray[index] = ptr;
        index++;
        ptr = strtok(NULL,":");
    }
           
    
    m_ptrNumerador->set_text(strsarray[0]);
    m_ptrDenominador->set_text(strsarray[1]);
    m_ptrAmplitud->set_text(strsarray[2]);
    m_ptrPeriodo->set_text(strsarray[3]);
    
    if(!strcmp(strsarray[4],"1")){
        m_chkLazo->set_active();
    }else{
        m_chkLazo->set_active(false);
    }
    
    unsigned int indice_señal = atoi(strsarray[5]);
    m_Combo.set_active(indice_señal-1);
    
    if(!strcmp(strsarray[6],"1")){
        m_chkControlador->set_active();
    }else{
        m_chkControlador->set_active(false);
    }
    
    m_ptrNumEntradas->set_text(strsarray[7]);
    
    if(!strcmp(strsarray[8],"1")){
        m_chkDiscreto->set_active();
    }else{
        m_chkDiscreto->set_active(false);
    }
    
    m_ptrMuestreo->set_text(strsarray[9]);
    
}

void ParametrosDlg::on_desde_arduino(){
    std::cout << "Desde Arduino" << std::endl;
}

void ParametrosDlg::on_toggled_discreto(){
    if(m_chkDiscreto->get_active()){
        m_ptrMuestreo->set_sensitive();
    }else{
        m_ptrMuestreo->set_sensitive(false);
        m_ptrMuestreo->set_text("-1");
    }
    
    on_update_str_params();
}




