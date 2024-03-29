//
// Created by lemonti on 11/7/16.
//

#include <iostream>
#include "SocketManager.hh"
#include "Command.hh"

SocketManager::SocketManager(short port)
{
  #ifdef _WIN32

  WSAData wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	throw "WSAStartup failed";
  std::cout << "WSAStartup ok" << std::endl;

  #endif

  this->_tv.tv_sec = 0;
  this->_tv.tv_usec = 1;
  this->_listener = ASocket::getNewSocket(port, "TCP");
  this->_listener->Bind();
  this->_listener->Listen();
}

SocketManager::~SocketManager()
{
  #ifdef _WIN32

  WSACleanup();

  #endif
}

void SocketManager::addSocket(ASocket *socket)
{
  this->_sockList.push_back(socket);
}

void SocketManager::removeSocket(const ASocket *socket)
{
  std::vector<ASocket *>::iterator toRemove = this->_sockList.end();

  for (auto it = this->_sockList.begin(); it != this->_sockList.end(); it++)
  {
	if (*it == socket)
	{
	  if (this->isSocketAvailable(socket, READ))
		this->removeFromFDSet(socket, READ);
	  if (this->isSocketAvailable(socket, WRITE))
		this->removeFromFDSet(socket, WRITE);
	  delete (socket);
	  toRemove = it;
	}
  }
  if (toRemove != this->_sockList.end())
	this->_sockList.erase(toRemove);
}

unsigned int SocketManager::_fillFDSet(FDSetType set)
{
  unsigned int maxfd;
  unsigned int tmp;

  FD_ZERO(&(this->_sets[set]));
  FD_SET(this->_listener->getSocket(), &(this->_sets[set]));
  maxfd = this->_listener->getSocket();
  for (auto it = this->_sockList.begin(); it != this->_sockList.end(); it++)
  {
	tmp = (*it)->getSocket();
	FD_SET(tmp, &(this->_sets[set]));
	if (tmp > maxfd)
	  maxfd = tmp;
  }
  return (maxfd);
}

int SocketManager::Select()
{
  unsigned int nfd;

  nfd = this->_fillFDSet(READ) + 1;
  return (select(nfd, &(this->_sets[READ]), &(this->_sets[WRITE]), NULL, &this->_tv));
}

ASocket *SocketManager::tryNewConnection()
{
  ASocket *newSocket;

  newSocket = NULL;
  if (FD_ISSET(this->_listener->getSocket(), &(this->_sets[READ])))
  {
	  std::cout << "fd_read listener ok" << std::endl;
	newSocket = this->_listener->Accept();
	if (newSocket != NULL)
	  this->_sockList.push_back(newSocket);
  }
  return (newSocket);
}

void SocketManager::addToFDSet(const ASocket *socket, FDSetType set)
{
  if (socket)
	FD_SET(socket->getSocket(), &(this->_sets[set]));
}

void SocketManager::removeFromFDSet(const ASocket *socket, FDSetType set)
{
  if (socket)
	FD_CLR(socket->getSocket(), &(this->_sets[set]));
}

bool SocketManager::isSocketAvailable(const ASocket *socket, FDSetType set) const
{
  if (!socket)
	return (false);
  return (FD_ISSET(socket->getSocket(), &(this->_sets[set])) != 0);
}
