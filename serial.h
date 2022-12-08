#pragma once

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <cstring>
#include <iostream>
#include <istream>
#include <queue>
#include <sigc++/sigc++.h>
#include <string>
#include <thread>
#include <unistd.h>

#define NUM_CHARS 2048

class Serial
{
    char read_msg_[NUM_CHARS];

    boost::asio::io_service m_io;
    boost::asio::serial_port m_port;

    boost::thread* m_ptrhilo = nullptr;

    std::ostringstream oss;
    std::queue<std::vector<double>> pila;

    const unsigned int NUM_SENALES = 5;

    bool running;

private:
    void handler(const boost::system::error_code& error, size_t bytes_transferred)
    {
		g_print("Entrando al handler...\n");
        oss << read_msg_;

        std::string str, str_linea;
        std::string str_buff = oss.str();

        std::size_t pos = str_buff.find_last_of("\n");

        if(pos == std::string::npos) {
            return;
        }

        str = str_buff.substr(0, pos);
        str_buff.replace(0, pos, "");

        oss.str(str_buff);
        std::istringstream iss(str);
        std::istringstream iss_linea;

        double v;
        std::vector<double> v_datos;

        // Buscar la línea de parametros
        std::getline(iss, str_linea);
        pos = str_linea.find("parametros");

        if(pos < str_linea.size()) {
            m_signal_str_params.emit(str_linea.substr(pos + 1)); // desecha 'parametros' y toma el resto
            return;
        }
        // .. de lo contrario deben de ser datos
        while(true) {
            std::getline(iss, str_linea);
            if(iss.fail())
                break;
            iss_linea.str(str_linea);
            for(unsigned int i = 0; i < NUM_SENALES; i++) {
                iss_linea >> v;
                if(iss_linea.fail())
                    break;
                v_datos.push_back(v);
            }
            if(v_datos.size() == NUM_SENALES)
                pila.push(v_datos);
            v_datos.clear();
        }

        if(running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            read_some();
        }
    }

protected:
    // signal accessor:
    using signal_str_params_t = sigc::signal<void(std::string)>;
    signal_str_params_t m_signal_str_params;

public:
    Serial(const char* dev_name)
        : m_io()
        , m_port(m_io, dev_name)
    {
        m_port.set_option(boost::asio::serial_port_base::parity()); // default none
        m_port.set_option(boost::asio::serial_port_base::character_size(8));
        m_port.set_option(boost::asio::serial_port_base::stop_bits()); // default one
        m_port.set_option(boost::asio::serial_port_base::baud_rate(115200));

    }

    ~Serial() {}

    signal_str_params_t signal_str_params() { return m_signal_str_params; }

    void read_some()
    {
        m_port.async_read_some(boost::asio::buffer(read_msg_, NUM_CHARS),
                               boost::bind(&Serial::handler, this, boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
    }

    void leer_puerto()
    {
        running = true;

		boost::thread::attributes attrs;	
		attrs.set_stack_size(1024);
    
        read_some();
        m_ptrhilo = new boost::thread(attrs, boost::bind(&boost::asio::io_service::run, &m_io));
        
    }

    void detener_hilo()
    {
        running = false;
        m_io.stop();
        m_ptrhilo->join();
    }

    void escribir_puerto(std::string s)
    {
        boost::asio::write(m_port, boost::asio::buffer(s.c_str(), s.size()));
    }

    std::queue<std::vector<double>> leer_datos()
    {
        std::queue<std::vector<double>> bkup;
        std::swap(pila, bkup); // para limpiar la pila
        return bkup;
    }
};
