//Async. Server using Boost Asio

#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

void processRequest(asio::ip::tcp::socket& sock) {
  // We use extensible buffer because we don't
  // know the size of the request message.
  asio::streambuf request_buf;

  system::error_code ec;

  // Receiving the request.
  asio::read(sock, request_buf, ec);
  std::istream input(&request_buf);
  std::string receiveMessage;
  std::getline(input, receiveMessage);

  std::cout << "Client message: " << receiveMessage << std::endl;

  if (ec != asio::error::eof)
    throw system::system_error(ec);

  // Request received. Sending response.
  // Allocating and filling the buffer with
  // binary data.
  //std::string sendMessage = "Hello from server";

  // Sending the request data.
  //asio::write(sock, asio::buffer(sendMessage));
  std::cout << "Shutting down server" << std::endl;

  // Shutting down the socket to let the
  // client know that we've sent the whole
  // response.
  sock.shutdown(asio::socket_base::shutdown_send);
}
int main(){
  unsigned short port_num = 9200;
  
  try {
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
    asio::io_service ios;
    asio::ip::tcp::acceptor acceptor(ios, ep);
    
    std::cout << "Waiting for Client" <<  std::endl;
    asio::ip::tcp::socket sock(ios);
    acceptor.accept(sock);
    std::cout << "Connected to Client\n" << std::endl;

    processRequest(sock);
  }
  catch (system::system_error &e) {
    std::cout << "Error occured! Error code = " << e.code()
      << ". Message: " << e.what();

    return e.code().value();
  }

  return 0;
}
