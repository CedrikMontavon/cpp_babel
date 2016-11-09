//
// Created by lemonti on 11/7/16.
//

#include <string.h>
#include <iostream>
#include "LinSocket.hh"

LinSocket::LinSocket(short port, const char *protocol)
{
  struct protoent *pe;

  pe = getprotobyname(protocol);
  this->_fd = socket(AF_INET, SOCK_STREAM, pe->p_proto);
  this->_port = port;
  if (this->_fd == -1)
	throw "socket failed";
  std::cout << "socket ok" << std::endl;
}

LinSocket::LinSocket(int fd, struct sockaddr_in *saddr)
{
  this->_fd = fd;
  this->_saddr = *saddr;
}

LinSocket::~LinSocket()
{
}

bool LinSocket::Bind()
{
  this->_saddr.sin_family = AF_INET;
  this->_saddr.sin_addr.s_addr = INADDR_ANY;
  this->_saddr.sin_port = htons(this->_port);
  if (bind(this->_fd, reinterpret_cast<struct sockaddr *>(&(this->_saddr)), sizeof(struct sockaddr_in)) < 0)
	throw "bind failed";
  std::cout << "bind ok" << std::endl;
  return (true);
}

bool LinSocket::Listen()
{
  if (listen(this->_fd, 255) < 0)
	throw "listen failed";
  std::cout << "listen ok" << std::endl;
  return (true);
}

ASocket *LinSocket::Accept()
{
  ASocket *newSocket;
  struct sockaddr_in saddr;
  int clientFD;
  socklen_t len = sizeof(saddr);

  if ((clientFD = accept(this->_fd, (struct sockaddr *) (&saddr), &len)) < 0)
	throw "accept failed";
  if (clientFD > 0)
  {
	newSocket = new LinSocket(clientFD, &saddr);
	std::cout << "accept ok" << std::endl;
	return (newSocket);
  }
  return (NULL);
}

bool LinSocket::Connect(const std::string &ip, short port)
{
  struct sockaddr_in saddr;

  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr(ip.c_str());
  saddr.sin_port = htons(port);
  if (connect(this->_fd, reinterpret_cast<struct sockaddr *>(&(saddr)), sizeof(struct sockaddr_in)) < 0)
	throw "connect failed";
  std::cout << "connect ok" << std::endl;
  return (true);
}

char *LinSocket::Receive() const
{
  char *buff = new char[44000];
  int i;

  memset(buff, 0, 44000);
  if ((i = read(this->_fd, buff, 43999)) < 0)
	throw "read failed";
  else if (i > 0)
  {
    std::cout << buff << std::endl;
    return buff;
  }
  return NULL;
}

bool LinSocket::Send(const char *message) const
{
  if (write(this->_fd, message, strlen(message)) < 0)
	throw "write failed";
  return (true);
}

unsigned int LinSocket::getSocket() const
{
  return (this->_fd);
}