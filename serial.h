#pragma once

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <istream>
#include <unistd.h>
#include <string>
#include <queue>

#define NUM_CHARS 2048

class Serial
{

    bool isReading;
    char read_msg_[NUM_CHARS];

    boost::asio::io_service m_io;
    boost::asio::serial_port m_port;  
        
    boost::thread* m_ptrhilo = nullptr;
        
    std::ostringstream oss;
    std::queue<std::vector<double>> pila;

private:
    void handler(const boost::system::error_code& error, size_t bytes_transferred)
    {
        oss << read_msg_ ;
        
        std::string str, str_linea;
        std::string str_buff = oss.str();
                
        std::size_t pos = str_buff.find_last_of("\n");
        
        if(pos == std::string::npos){
            return;
        }
        
        str = str_buff.substr(0,pos);
        str_buff.replace(0,pos,"");
        
        oss.str(str_buff);
        std::istringstream iss(str);
        std::istringstream iss_linea;
        double v;
        std::vector<double> v_datos;
        
        while(true){
            std::getline(iss,str_linea);
            if(iss.fail()) break;
            iss_linea.str(str_linea);
            for(int i = 0; i < 4; i++){
                iss_linea >> v;
                if(iss_linea.fail()) break;
                v_datos.push_back(v);
            }
            if(v_datos.size()==4) pila.push(v_datos);
            v_datos.clear();
        };
        
        /*read_msg_[bytes_transferred] = 0;
        std::cout << bytes_transferred << " bytes: " << read_msg_ << std::endl;*/

        if(isReading)
        {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
            read_some();
        }
    }

public:
    Serial(const char* dev_name)
        : m_io()
        , m_port(m_io, dev_name)
    {
        isReading = false;

        m_port.set_option(boost::asio::serial_port_base::parity());	// default none
        m_port.set_option(boost::asio::serial_port_base::character_size( 8 ) );
        m_port.set_option(boost::asio::serial_port_base::stop_bits());	// default one
        m_port.set_option(boost::asio::serial_port_base::baud_rate(115200));

        // run the IO service as a separate thread, so the main thread can do others
    }

    ~Serial()
    {
    }

    void read_some()
    {
        m_port.async_read_some(boost::asio::buffer(read_msg_, NUM_CHARS),
                               boost::bind(&Serial::handler, this, boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
    }

    void leer_puerto(bool b = true)
    {
        isReading = b;
        
        boost::thread::attributes attrs;
        attrs.set_stack_size(1024);
        
        if(b) {
            read_some();
            m_ptrhilo = new boost::thread(attrs, boost::bind(&boost::asio::io_service::run, &m_io));
        } else {
            
            m_ptrhilo->join();
            m_io.restart();
            
            delete m_ptrhilo;
            m_ptrhilo = nullptr;
            
        }
    }

    std::queue<std::vector<double>> leer_datos() {
        std::queue<std::vector<double>> bkup;
        std::swap( pila, bkup ); // para limpiar la pila
        return bkup;
    }
    
    bool is_reading(){
        return isReading;
    }
};
