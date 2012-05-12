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
    // ���µļ��䶼�ǹ̶���
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD( 2, 2 );

    //�ú����Ĺ����Ǽ���һ��Winsocket��汾
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


    //����socket,�������ӵĿɿ��Է���SOCK_STRAM
    SOCKET sockSrv=socket(AF_INET, SOCK_STREAM, 0);

    //��ű��ص�ַ��Ϣ��
    SOCKADDR_IN addrSrv;
    //htol�������ֽ���long��ת��Ϊ�����ֽ���
    addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    addrSrv.sin_family=AF_INET;
    //htos�������˿�ת�����ַ���1024���ϵ����ּ���
    addrSrv.sin_port=htons(6001);

    //��socket�󶨵���Ӧ��ַ�Ͷ˿���
    bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
    
    //�ȴ������е���󳤶�Ϊ5
    listen(sockSrv,5);

    SOCKADDR_IN addrClient;
    int len=sizeof(SOCKADDR);

    while(1)
    {
        //����һ���µ��׽�������ͨ�ţ�����ǰ��ļ����׽���
        SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);
        char sendBuf[100];
        //inet_nota�����ǽ��ַ�ת����ip��ַ
        sprintf(sendBuf,"Server IP is:%s", inet_ntoa(addrClient.sin_addr));
       
        send(sockConn,sendBuf,strlen(sendBuf)+1,0);//��������ͻ��˷�������

        HostInfo recvBuf;
        ZeroMemory(&recvBuf, sizeof(HostInfo));
        //�������ӿͻ��˽�������
        recv(sockConn, 
            reinterpret_cast<char *>(&recvBuf),
            sizeof(HostInfo), 0);
        PrintClientInfo(&recvBuf);
        closesocket(sockConn);//�ر�socket
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