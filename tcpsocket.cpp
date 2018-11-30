

#include "tcpsocket.h"

CTcpSocket::CTcpSocket()
{
    fd_socket = -1;
}
CTcpSocket::~CTcpSocket()
{}

CTcpSocket *CTcpSocket::tcpsockrt = new CTcpSocket();

CTcpSocket *CTcpSocket::getInstance()
{
    return tcpsockrt;
}
int CTcpSocket::InitTcpSocket()
{
    fd_socket = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr =inet_addr("192.168.0.102");
    connect(fd_socket,(struct sockaddr*)&addr,sizeof(addr));
    return 0;
}
int CTcpSocket::SendInfo( const char *devid, const char *printinfo)
{
    if(fd_socket == -1)
    {
        printf("socket isn't init!\n");
        return -1;
    }
    char sendbuff[128] = {0};
    strcpy(sendbuff,devid);
    strcat(sendbuff,":");
    strcat(sendbuff,printinfo);
    write(fd_socket,devid,strlen(devid));
    return 0;
}

