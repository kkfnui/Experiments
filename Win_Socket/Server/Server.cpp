// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <string>
#include <iostream>
using namespace std;
typedef struct _HostInfo
{
    char szHostName[64];
    char szIPAddress[64];
    ULONGLONG ulNetworkSpeed;

}HostInfo, *pHostInfo;

void PrintClientInfo(const pHostInfo clientHostInfo);
void ConvertSpeedToString(ULONGLONG ulSpeedByte);

int _tmain(int argc, _TCHAR* argv[])
{
    char test[15] = "dafdasfasd";
    cout << test << endl;
    // 以下的几句都是固定的
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD( 2, 2 );

    //该函数的功能是加载一个Winsocket库版本
    err = WSAStartup(wVersionRequested, &wsaData );
    if ( err != 0 ) {
        return 0;
    }

    if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) 
    {
            WSACleanup( );
            return 0; 
    }


    //创建socket,面向连接的可靠性服务SOCK_STRAM
    SOCKET sockSrv=socket(AF_INET, SOCK_STREAM, 0);

    //存放本地地址信息的
    SOCKADDR_IN addrSrv;
    //htol将主机字节序long型转换为网络字节序
    addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    addrSrv.sin_family=AF_INET;
    //htos用来将端口转换成字符，1024以上的数字即可
    addrSrv.sin_port=htons(6001);

    //将socket绑定到相应地址和端口上
    bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
    
    //等待队列中的最大长度为5
    listen(sockSrv,5);

    SOCKADDR_IN addrClient;
    int len=sizeof(SOCKADDR);

    while(1)
    {
        //建立一个新的套接字用于通信，不是前面的监听套接字
        SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);
        char sendBuf[100];
        //inet_nota函数是将字符转换成ip地址
        sprintf(sendBuf,"Server IP is:%s", inet_ntoa(addrClient.sin_addr));
       
        send(sockConn,sendBuf,strlen(sendBuf)+1,0);//服务器向客户端发送数据

        HostInfo recvBuf;
        ZeroMemory(&recvBuf, sizeof(HostInfo));
        //服务器从客户端接受数据
        recv(sockConn, 
            reinterpret_cast<char *>(&recvBuf),
            sizeof(HostInfo), 0);
        PrintClientInfo(&recvBuf);
        closesocket(sockConn);//关闭socket
    }	
    WSACleanup();
    return 0;
}

void PrintClientInfo(const pHostInfo clientHostInfo)
{
    char szBuffer[256];
    ZeroMemory(szBuffer, 256);
    //string strBuffer ( (char *)clientHostInfo->szHostName);
    //sprintf(szBuffer, "client host name :%s", strBuffer);
    //cout << szBuffer <<endl;
    cout << clientHostInfo->szHostName << endl;
    ZeroMemory(szBuffer, 256);
    sprintf(szBuffer, "client ip address :%s", clientHostInfo->szIPAddress);
    cout << *(clientHostInfo->szIPAddress) << endl;
    cout << "cliden network speed = " << clientHostInfo->ulNetworkSpeed;
}
void ConvertSpeedToString(ULONGLONG ulSpeedByte, string& outputString)
{
    double dSpeedKB = ulSpeedByte / 1024.0;
    char szBuffer[10];
    if (dSpeedKB >= 1000)
    {
        double dSpeedMB = dSpeedKB /1024.0;
        sprintf(szBuffer, "%d MB/S", dSpeedMB);
    }
    else
    {
        sprintf(szBuffer, "%d KB/s", dSpeedKB);
    }
    outputString = szBuffer;
}