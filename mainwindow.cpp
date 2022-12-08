#include "mainwindow.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app)
    : serial(nullptr)
    , filename("default.txt")
    , str_params("0 1 0 0 0:0 1 0 1 0:10:14:1:20:1:1:1:0")
    , m_Box(Gtk::Orientation::VERTICAL, 10)
    , m_btnIniciar("Iniciar")
    , m_btnConectar("Conectar")
    , m_labelPuerto("Puerto Serie : ")
    , m_entryPuerto()
    , m_statusbar()
    , canvas()
{
    set_title(std::string("Monitor Serial - ") + filename);
    set_default_size(900, 700);

    set_child(m_Box);

    // Events.
    auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(sigc::mem_fun(*this, &MainWindow::on_window_key_pressed), true);
    add_controller(controller);

    // Define the actions
    m_refActionGroup = Gio::SimpleActionGroup::create();

    m_refActionGroup->add_action("nuevo", sigc::mem_fun(*this, &MainWindow::on_archivo_nuevo));
    m_refActionGroup->add_action("guardar", sigc::mem_fun(*this, &MainWindow::on_archivo_guardar));
    m_refActionGroup->add_action("guardarc", sigc::mem_fun(*this, &MainWindow::on_archivo_guardarc));
    m_refActionGroup->add_action("abrir", sigc::mem_fun(*this, &MainWindow::on_archivo_abrir));

    m_refActionGroup->add_action("conectar", sigc::mem_fun(*this, &MainWindow::on_puerto_dialog));
    m_refActionGroup->add_action("desconectar", sigc::mem_fun(*this, &MainWindow::on_puerto_desconectar));
    m_refActionGroup->add_action("parameters", sigc::mem_fun(*this, &MainWindow::on_parameters_dialog));

    insert_action_group("example", m_refActionGroup);

    m_refBuilder = Gtk::Builder::create();

    // Layout the actions in a menubar and toolbar:
    const char* ui_info = "<interface>"
                          "  <menu id='menubar'>"
                          "    <submenu>"
                          "      <attribute name='label' translatable='yes'>_Archivo</attribute>"
                          "      <section>"
                          "        <item>"
                          "          <attribute name='label' translatable='yes'>_Nuevo</attribute>"
                          "          <attribute name='action'>example.nuevo</attribute>"
                          "          <attribute name='accel'>&lt;Primary&gt;u</attribute>"
                          "        </item>"
                          "        <item>"
                          "          <attribute name='label' translatable='yes'>_Abrir</attribute>"
                          "          <attribute name='action'>example.abrir</attribute>"
                          "          <attribute name='accel'>&lt;Primary&gt;a</attribute>"
                          "        </item>"
                          "        <item>"
                          "          <attribute name='label' translatable='yes'>_Guardar</attribute>"
                          "          <attribute name='action'>example.guardar</attribute>"
                          "          <attribute name='accel'>&lt;Primary&gt;g</attribute>"
                          "        </item>"
                          "        <item>"
                          "          <attribute name='label' translatable='yes'>Guardar _como</attribute>"
                          "          <attribute name='action'>example.guardarc</attribute>"
                          "          <attribute name='accel'>&lt;Primary&gt;g</attribute>"
                          "        </item>"
                          "      </section>"
                          "    </submenu>"
                          "    <submenu>"
                          "      <attribute name='label' translatable='yes'>_Puerto</attribute>"
                          "      <section>"
                          "        <item>"
                          "          <attribute name='label' translatable='yes'>_Conectar...</attribute>"
                          "          <attribute name='action'>example.conectar</attribute>"
                          "          <attribute name='accel'>&lt;Primary&gt;c</attribute>"
                          "        </item>"
                          "        <item>"
                          "          <attribute name='label' translatable='yes'>_Desconectar</attribute>"
                          "          <attribute name='action'>example.desconectar</attribute>"
                          "          <attribute name='accel'>&lt;Primary&gt;d</attribute>"
                          "        </item>"
                          "      </section>"
                          "    </submenu>"
                          "    <submenu>"
                          "          <attribute name='label' translatable='yes'>Controlador</attribute>"
                          "          <section>"
                          "             <item>"
                          "                 <attribute name='label' translatable='yes'>_Parameters</attribute>"
                          "                 <attribute name='action'>example.parameters</attribute>"
                          "                 <attribute name='accel'>&lt;Primary&gt;p</attribute>"
                          "             </item>"
                          "           </section>"
                          "     </submenu> "
                          "  </menu>"
                          "</interface>";

    app->set_accel_for_action("example.conectar", "<Primary>c");
    app->set_accel_for_action("example.desconectar", "<Primary>d");
    app->set_accel_for_action("example.parameters", "<Primary>p");
    app->set_accel_for_action("example.guardar", "<Primary>g");
    app->set_accel_for_action("example.guardar", "<Primary>u");
    app->set_accel_for_action("example.nuevo", "<Primary>n");
    app->set_accel_for_action("example.abrir", "<Primary>a");

    try {
        m_refBuilder->add_from_string(ui_info);
    } catch(const Glib::Error& ex) {
        std::cerr << "Building menu failed: " << ex.what();
    }

    // Get the menubar:
    auto object = m_refBuilder->get_object("menubar");
    auto gmenu = std::dynamic_pointer_cast<Gio::Menu>(object);

    if(!gmenu)
        g_warning("GMenu not found");
    else {
        auto pMenuBar = Gtk::make_managed<Gtk::PopoverMenuBar>(gmenu);

        // Add the popover MenuBar to the window
        m_Box.append(*pMenuBar);
    }

    canvas.set_expand();

    Gtk::Box hbox(Gtk::Orientation::HORIZONTAL);
    Gtk::Box vbox(Gtk::Orientation::VERTICAL);
    hbox.append(vbox);
    hbox.append(canvas);

    m_Box.append(hbox);
    vbox.append(m_btnIniciar);
    vbox.set_margin(20);

    m_statusbar.push("Puerto : ?  : Status: OFFLINE");
    m_Box.append(m_statusbar);

    // m_Box.append(m_BoxBottom);

    m_btnIniciar.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnIniciar_clicked));
    m_btnIniciar.set_sensitive(false);
    m_btnIniciar.set_expand(false);

    m_btnConectar.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnConectar_clicked));

    ///////////////

    grid.set_margin(35);
    grid.set_row_spacing(10);
    grid.set_column_homogeneous();
    grid.set_column_spacing(10);
    grid.attach(m_labelPuerto, 2, 1);
    grid.attach(m_entryPuerto, 2, 2);
    grid.attach(m_btnConectar, 2, 3);

    m_labelPuerto.set_halign(Gtk::Align::START);
    m_entryPuerto.set_max_length(10);
    m_entryPuerto.set_text("COM3");
}

MainWindow::~MainWindow() {}

void MainWindow::on_puerto_dialog()
{
    Gtk::Dialog* dlg = new Gtk::Dialog("Puerto Serie", *this, true);
    dlg->set_child(grid);
    dlg->signal_close_request().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::delete_dlg), dlg), false);
    dlg->show();
}

void MainWindow::on_parameters_dialog()
{

    ParametrosDlg* dlg = new ParametrosDlg("Parámetros", *this, true);
    dlg->signal_close_request().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::delete_dlg), dlg), false);
    dlg->set_parametros(str_params);
    dlg->show();
}

bool MainWindow::delete_dlg(Gtk::Dialog* p)
{
    p->hide();
    delete p;
    return true;
}

void MainWindow::on_puerto_desconectar() { std::cout << "desconectando..." << std::endl; }

bool MainWindow::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state)
{
    // Gdk::ModifierType::ALT_MASK -> the 'Alt' key(mask)
    // GDK_KEY_1 -> the '1' key
    // GDK_KEY_2 -> the '2' key

    // select the first radio button, when we press alt + 1
    if((keyval == GDK_KEY_1) && (state & (Gdk::ModifierType::SHIFT_MASK | Gdk::ModifierType::CONTROL_MASK |
                                          Gdk::ModifierType::ALT_MASK)) == Gdk::ModifierType::ALT_MASK) {
        canvas.pop_axes();
        return true;
    } else if((keyval == GDK_KEY_2) && (state & (Gdk::ModifierType::SHIFT_MASK | Gdk::ModifierType::CONTROL_MASK |
                                                 Gdk::ModifierType::ALT_MASK)) == Gdk::ModifierType::ALT_MASK) {
        // and the second radio button, when we press alt + 2
        return true;
    } else if(keyval == GDK_KEY_Escape) {
        // close the window, when the 'esc' key is pressed
        hide();
        return true;
    }

    switch(keyval) {
    case GDK_KEY_Escape:
        hide();
        return true;
    case GDK_KEY_p:
    case GDK_KEY_P:
        break;
    default:
        break;
    }

    // the event has not been handled
    return false;
}

void MainWindow::on_btnIniciar_clicked()
{
    Glib::ustring label = m_btnIniciar.get_label();
    Glib::ustring labelConectar = m_btnConectar.get_label();
    std::ostringstream ss;
	
	static sigc::connection conn;

    if(label == "Iniciar") {

        m_btnIniciar.set_label("Parar");
        serial->escribir_puerto("iniciar");
        m_btnConectar.set_sensitive(false);
		
		sigc::slot<bool()> slot_timeout = sigc::mem_fun(*this, &MainWindow::on_timeout);
		conn = Glib::signal_timeout().connect(slot_timeout, 500, Glib::PRIORITY_HIGH_IDLE);

        ss << "Puerto : " << m_entryPuerto.get_text() << ", Status : CONECTADO , Proceso : EJECUTANDOSE";

        str_params[str_params.size()-1]='1'; // La lectura del puerto se lleva a cabo en Timeout
        canvas.limpiar_trazas();
        serial->leer_puerto();

    } else {
        m_btnIniciar.set_label("Iniciar");
        serial->escribir_puerto("parar");
        m_btnConectar.set_sensitive(true);
		
		if(conn.connected())
			conn.disconnect();
			
        str_params[str_params.size()-1]='0';
        
        ss << "Puerto : " << m_entryPuerto.get_text() << ", Status : CONECTADO , Proceso : DETENIDO";
    }

    m_statusbar.push(ss.str().c_str());
	
}

void MainWindow::on_btnConectar_clicked()
{

    Glib::ustring label = m_btnConectar.get_label();
    Glib::ustring strpuerto = m_entryPuerto.get_text();

    std::ostringstream ss;

    if(label == "Conectar") {

        m_btnConectar.set_label("Desconectar");
        m_entryPuerto.set_editable(false);
        m_entryPuerto.set_opacity(0.5);

        crear_serial(strpuerto.c_str());

        ss << "Puerto : " << strpuerto << " , Status : CONECTADO, Proceso : DETENIDO";
        m_statusbar.push(ss.str().c_str());

        m_btnIniciar.set_sensitive(true);

    } else {
        m_btnConectar.set_label("Conectar");
        m_btnIniciar.set_label("Iniciar");
        m_entryPuerto.set_editable();
        m_entryPuerto.set_opacity(1.0);

        borrar_serial();

        ss << "Puerto : " << m_entryPuerto.get_text() << " , Status : DESCONECTADO , Proceso : DETENIDO";
        m_statusbar.push(ss.str().c_str());

        m_btnIniciar.set_sensitive(false);
    }
}

void MainWindow::on_archivo_guardar()
{
    std::ofstream oarchivo;

    oarchivo.open(filename.c_str(), std::ios::out);
    oarchivo.write(str_params.c_str(), str_params.size());
    oarchivo.close();

    set_title(std::string("Monitor Serial - ") + filename);
}

void MainWindow::on_archivo_nuevo() { 
    filename = "default.txt";
    str_params = "0 1 0 0 0:0 1 0 1 0:10:14:1:20:1:1:1:0";
    
    set_title(std::string("Monitor Serial - ") + filename);

}

void MainWindow::on_archivo_guardarc() { on_file_dialog(Gtk::FileChooser::Action::SAVE); }
void MainWindow::on_archivo_abrir() { on_file_dialog(Gtk::FileChooser::Action::OPEN); }

void MainWindow::set_str_params(std::string str) { str_params = str; }

void MainWindow::on_file_dialog(Gtk::FileChooser::Action action)
{
    auto dialog = new Gtk::FileChooserDialog("Selecciona un archivo", action);
    dialog->set_transient_for(*this);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::on_file_dialog_response), dialog));

    // Add response buttons to the dialog:
    dialog->add_button("_Cancelar", Gtk::ResponseType::CANCEL);

    if(action == Gtk::FileChooser::Action::SAVE) {
        dialog->add_button("_Guardar", Gtk::ResponseType::OK);
    } else {
        dialog->add_button("_Abrir", Gtk::ResponseType::ACCEPT);
    }

    dialog->set_margin(50);

    // Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog->add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    // Show the dialog and wait for a user response:
    dialog->show();
}

void MainWindow::on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog)
{
    std::ofstream oarchivo;
    std::ifstream iarchivo;

    // Handle the response:
    switch(response_id) {
    case Gtk::ResponseType::OK: { // Save
        // Notice that this is a std::string, not a Glib::ustring.
        filename = dialog->get_file()->get_path();
        oarchivo.open(filename.c_str(), std::ios::out);
        oarchivo.write(str_params.c_str(), str_params.size());
        oarchivo.close();

        set_title(std::string("Monitor Serial - ") + filename);

        break;
    }
    case Gtk::ResponseType::ACCEPT: { // Abrir
        filename = dialog->get_file()->get_path();
        iarchivo.open(filename.c_str());
        iarchivo.seekg(0, std::ios::end);
        size_t size = iarchivo.tellg();
        char buff[size];
        iarchivo.seekg(0);
        iarchivo.read(buff, size);
        buff[size] = '\0';
        str_params = std::string(buff);
        iarchivo.close();

        set_title(std::string("Monitor Serial - ") + filename);

        break;
    }
    case Gtk::ResponseType::CANCEL: {
        std::cout << "Cancelado" << std::endl;
        break;
    }
    default: {
        std::cout << "boton inesperado pulsado." << std::endl;
        break; 
    }
    }
    delete dialog;
}

void MainWindow::crear_serial(std::string name)
{
    if(serial == nullptr) {
        try {
            // pasar una referencia inmediatamente al area de dibujo
            serial = new Serial(name.c_str());
        } catch(std::exception& e) {
            std::cout << "Error al crear el puerto serie : " << e.what() << std::endl;
        }
    }
    
    serial->signal_str_params().connect(sigc::mem_fun(*this,&MainWindow::set_str_params));
}

void MainWindow::borrar_serial()
{
    if(serial != nullptr) {
		serial->detener_hilo();
        delete serial;
        serial = nullptr;
    }
}

void MainWindow::escribir_serial(std::string str)
{
    if(serial != nullptr) {
        serial->escribir_puerto(str);
    }
}

bool MainWindow::on_timeout()
{
	g_print("on_timeout start\n");
    if(serial != nullptr) {
        std::queue<std::vector<double>> datos = serial->leer_datos();
        std::vector<double> dato;

        if(datos.empty())
            return true;

        while(!datos.empty()) {

            dato = datos.front();
            datos.pop();

            canvas.add_to_trazas(dato);
        }
        canvas.monitor.adding_points(true);
    } else {
        // Es importante para fijar
        canvas.monitor.adding_points(false);
    }

    canvas.queue_draw();
    /*
     * Si se retorna falso, el handle (la función) se desconecta de las señales timeout
     */
	g_print("on_timeout end\n");
    return true;
}


