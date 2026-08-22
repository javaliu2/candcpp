#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("localhost", "12345");
        tcp::resolver::iterator endpoint_iter = resolver.resolve(query);
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iter);
        while (true) {
            std::cout << "Enter Message: ";
            std::string message;
            std::getline(std::cin, message);
            boost::asio::write(socket, boost::asio::buffer(message));
            char reply[512];
            size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, message.size()));
            std::cout << "Reply is: ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
        }
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}