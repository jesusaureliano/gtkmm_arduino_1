#include "mainwindow.h"

#include <exception>
#include <iostream>

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app)
    : m_Box(Gtk::Orientation::VERTICAL, 10)
    , m_BoxVert1(Gtk::Orientation::VERTICAL, 10)
    , m_btnClosed("Lazo cerrado")
    , m_btnIniciar("Iniciar")
    , m_btnConectar("Conectar")
    , m_labelPuerto("Puerto Serie : ")
    , m_entryPuerto()
    , m_statusbar()
{
    set_title("Monitor Serial");
    set_default_size(900, 700);

    set_child(m_Box);

    // Events.
    auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(sigc::mem_fun(*this, &MainWindow::on_window_key_pressed), true);
    add_controller(controller);

    // Define the actions
    m_refActionGroup = Gio::SimpleActionGroup::create();

    m_refActionGroup->add_action("conectar", sigc::mem_fun(*this, &MainWindow::on_puerto_dialog));
    m_refActionGroup->add_action("desconectar", sigc::mem_fun(*this, &MainWindow::on_puerto_desconectar));

    insert_action_group("example", m_refActionGroup);

    m_refBuilder = Gtk::Builder::create();

    // Layout the actions in a menubar and toolbar:
    const char* ui_info = "<interface>"
                          "  <menu id='menubar'>"
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
                          "  </menu>"
                          "</interface>";

    app->set_accel_for_action("example.conectar", "<Primary>c");
    app->set_accel_for_action("example.desconectar", "<Primary>d");

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
    m_Box.append(canvas);

    m_statusbar.push("Puerto : ?  : Status: OFFLINE");
    m_Box.append(m_statusbar);

    // m_Box.append(m_BoxBottom);

    m_btnIniciar.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnIniciar_clicked));
    m_btnIniciar.set_can_target(false);
    m_btnIniciar.set_expand();

    m_btnClosed.signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::on_btnClosed_toggled));

    m_btnConectar.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnConectar_clicked));

    ///////////////

    grid.set_margin(35);
    grid.set_row_spacing(10);
    grid.set_column_homogeneous();
    grid.set_column_spacing(10);
    grid.attach(m_btnClosed, 1, 2);
    grid.attach(m_btnIniciar, 1, 3);
    grid.attach(m_labelPuerto, 2, 1);
    grid.attach(m_entryPuerto, 2, 2);
    grid.attach(m_btnConectar, 2, 3);

    m_labelPuerto.set_halign(Gtk::Align::START);
    m_entryPuerto.set_max_length(10);
    m_entryPuerto.set_text("COM3");

    m_puertoDlg = new Gtk::Dialog("Puerto Serie", *this, true);
    m_puertoDlg->set_child(grid);
    
}

MainWindow::~MainWindow() {}

void MainWindow::on_puerto_dialog()
{
    if(m_puertoDlg)
        delete m_puertoDlg;

    m_puertoDlg = new Gtk::Dialog("Puerto Serie", *this, true);
    m_puertoDlg->set_child(grid);

    m_puertoDlg->show();
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

void MainWindow::on_btnClosed_toggled()
{
    std::cout << "The Button was toggled: state=" << (m_btnClosed.get_active() ? "true" : "false") << std::endl;
}

void MainWindow::on_btnIniciar_clicked()
{
    Glib::ustring label = m_btnIniciar.get_label();
    Glib::ustring labelConectar = m_btnConectar.get_label();
    std::ostringstream ss;

    if(label == "Iniciar") {
        
        m_btnIniciar.set_label("Parar");
        m_btnConectar.set_can_target(false);
        
        ss << "Puerto : " << m_entryPuerto.get_text() << ", Status : CONECTADO , Proceso : EJECUTANDOSE";
        
        canvas.limpiar_trazas();
        canvas.leer_puerto(true);
        
    } else {
        m_btnIniciar.set_label("Iniciar");
        m_btnConectar.set_can_target(true);
        
        ss << "Puerto : " << m_entryPuerto.get_text() << ", Status : CONECTADO , Proceso : DETENIDO";
        canvas.leer_puerto(false);
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

        canvas.crear_serial(strpuerto.c_str());
        
        ss << "Puerto : " << strpuerto << " , Status : CONECTADO, Proceso : DETENIDO";
        m_statusbar.push(ss.str().c_str());

        m_btnIniciar.set_can_target(true);

    } else {
        m_btnConectar.set_label("Conectar");
        m_btnIniciar.set_label("Iniciar");
        m_entryPuerto.set_editable();
        m_entryPuerto.set_opacity(1.0);

        canvas.borrar_serial();

        ss << "Puerto : " << m_entryPuerto.get_text() << " , Status : DESCONECTADO , Proceso : DETENIDO";
        m_statusbar.push(ss.str().c_str());

        m_btnIniciar.set_can_target(false);
    }
}