#ifndef NETWORK_H_
# define NETWORK_H_

#include <QThread>
#include <QEventLoop>
#include <QTimer>
#include "SocketManager.hh"

class Network : public QThread
{
  Q_OBJECT
public:
  Network();
  
  public slots:
   void		run();

   void		newName(QString const &);
   void		sndCall(QString const &);
   void		acceptCall();
   void		refuseCall();
   void		endCall();
   
 signals:
   void		nameTaken();
   void		sndFailed();
   void		nameWrong();
   void		unknownName();
   void		callFailed();
   void		connectFailed();
  
  private:
    SocketManager *_sm;
    QEventLoop *_loop;
    QTimer *_timer;
};

#endif /* !NETWORK_H_ */
