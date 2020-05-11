#include <iostream>
#include <ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std; //again, I know. but it's just a test project

void main()
{
    //Start winsock
    WSADATA data;
    WORD version = MAKEWORD(2,2);
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0 )
    {
        cout << "cant start winsock" << wsOk;
    }
    //bind socket to IP and port
    SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);

    if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) 
    {
        cout << "cant bind socket" << WSAGetLastError() << endl;
        return;
    }

    sockaddr_in client;
    ZeroMemory(&client,sizeof(client));
    int clientLength = sizeof(client);
    char buf[1024];
    //enter loop
    while (true)
    {
        ZeroMemory(&buf, sizeof(buf));
       //wait for message
        int bytesIn = recvfrom(in, buf,sizeof(buf),0,(sockaddr*)&client,&clientLength);
        if (bytesIn == SOCKET_ERROR) 
        {
            cout << "error receiving from client" << WSAGetLastError() << endl; // probs not a big deal
            continue; // UDP chaos so who cares lets continue
        }
       //Display message
        char clientIp[256];
        ZeroMemory(clientIp, sizeof(clientIp));

        inet_ntop(AF_INET, &client.sin_addr, clientIp, sizeof(clientIp));
        cout << "Message recv from" << clientIp << " : " << buf << endl;
    }

    //close socket
    closesocket(in);
    //shut down winsock
    WSACleanup();
}