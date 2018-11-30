#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

class CTcpSocket
{
public:
    CTcpSocket();
    ~CTcpSocket();

    static CTcpSocket *getInstance();
    int InitTcpSocket();
    int SendInfo(const  char *devid,const  char *printinfo);
private:
    static CTcpSocket *tcpsockrt;
    int fd_socket;
};

#endif // TCPSOCKET_H
