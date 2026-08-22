#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));
        while (true) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            boost::system::error_code error_code;
            while (true) {
                char data[512];
                size_t length = socket.read_some(boost::asio::buffer(data), error_code);
                if (error_code == boost::asio::error::eof) {
                    break;
                } else if (error_code) {
                    throw boost::system::system_error(error_code);
                }
                boost::asio::write(socket, boost::asio::buffer(data, length));
            }
        }
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}