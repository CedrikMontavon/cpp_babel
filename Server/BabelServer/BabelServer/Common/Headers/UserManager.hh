//
// Created by lemonti on 11/3/16.
//

#ifndef CPP_BABEL_USERMANAGER_HH
#define CPP_BABEL_USERMANAGER_HH

#include "User.hh"

class UserManager
{
  public:
	UserManager();
	~UserManager();
	int 					addUser(int fd, const std::string& name);
	void 					removeUser(const std::string& name);
	User					*getUser(const std::string& name);

  private:
	std::vector<User *>		_users;
};


#endif //CPP_BABEL_USERMANAGER_HH