

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
void CTcpSocket::InitTcpSocket()
{
    fd_socket = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(44444);
    addr.sin_addr.s_addr =inet_addr("192.168.0.101");
    connect(fd_socket,(struct sockaddr*)&addr,sizeof(addr));

}
void CTcpSocket::SendInfo( const char *devid, const char *printinfo)
{
    if(fd_socket == -1)
    {
        printf("socket isn't init!\n");

    }
    char sendbuff[128] = {0};
    strcpy(sendbuff,devid);
    strcat(sendbuff,":");
    strcat(sendbuff,printinfo);
    if(send(fd_socket,sendbuff,strlen(sendbuff),0) == -1)
        printf("send error!\n");


}

