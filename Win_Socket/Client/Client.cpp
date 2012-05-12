// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>
using namespace std;

typedef struct _HostInfo
{
    char szHostName[64];
    char szIPAddress[64];
    ULONGLONG ulNetworkSpeed;

}HostInfo, *pHostInfo;

int _tmain(int argc, _TCHAR* argv[])
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 2, 2 );

    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        return 0;
    }

    if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
            WSACleanup( );
            return 0; 
    }

    //建立通讯socket
    SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);

    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr=inet_addr("192.168.102.49");//设定需要连接的服务器的ip地址
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(6001);//设定需要连接的服务器的端口地址
    if(SOCKET_ERROR == connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR)))//与服务器进行连接
    {
        cout << "连接服务器发生错误" << endl;
        return 0;
    }

    //接收来之服务端的消息
    char recvBuf[100];
    recv(sockClient,recvBuf,100,0);
    printf("连接服务器成功，服务器ip：%s\n", recvBuf);

    //创建发送给服务端的数据
    HostInfo hostInfo;
    memset(&hostInfo, 0x00, sizeof(hostInfo));
    
    memcpy(hostInfo.szHostName, "lvfei-pc", lstrlenA("lvfei-pc")*sizeof(char));
    memcpy(hostInfo.szIPAddress, "192.168.0.45", lstrlenA("192.168.0.45")*sizeof(char));
    hostInfo.ulNetworkSpeed = 500000;

    //发送信息给服务器
    send(sockClient,(char *)&hostInfo, sizeof(hostInfo), 0);

    closesocket(sockClient);
    WSACleanup();
    int nWait;
    cin >> nWait;
    return 0;
}

