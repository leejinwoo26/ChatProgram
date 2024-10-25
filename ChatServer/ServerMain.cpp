#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 1024

void ErrorHandling(const char* message) {
    WSACleanup();
    fputs(message, stderr);
    exit(1);
}

int main(int argc, char** argv) {
    WSADATA wsaData;
    SOCKET servSock, clntSock;
    SOCKADDR_IN servAddr, clntAddr;
    char message[BUFSIZE];
    int strLen = 0, fromLen = 0, nRcv = 0, port = 0;

    if (argc != 2) {
        printf("��Ʈ ��ȣ�� �Է����ּ���. : ");
        std::cin>>port;
    }

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == INVALID_SOCKET)
    {
        ErrorHandling("���� ����\n");
    }

    memset(&servAddr, 0, sizeof(SOCKADDR_IN));
    servAddr.sin_family = AF_INET;
    if (argc == 2)    servAddr.sin_port = htons(atoi(argv[1]));
    else            servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);


    if (bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("���ε� ����\n");

    if (listen(servSock, 2) == SOCKET_ERROR)
        ErrorHandling("������ ����\n");

    fromLen = sizeof(clntAddr);
    clntSock = accept(servSock, (sockaddr*)&clntAddr, &fromLen);

    if (clntSock == INVALID_SOCKET)
        ErrorHandling("���� ����\n");
    else
    {
        char clntAddrStr[INET_ADDRSTRLEN];  // IPv4 �ּ� ����
        inet_ntop(AF_INET, &clntAddr.sin_addr, clntAddrStr, sizeof(clntAddrStr));
        printf("%s ���� ����!\nStart ...\n", clntAddrStr);
    }

    while (1) {
        printf("�޽��� ��ٸ��� ��...\n");
        nRcv = recv(clntSock, message, sizeof(message) - 1, 0);

        if (nRcv == SOCKET_ERROR) {
            printf("���� ����..\n");
            break;
        }
        message[nRcv] = '\0';

        if (strcmp(message, "exit") == 0) {
            printf("Ŭ���̾�Ʈ�� ������ �����Ͽ����ϴ�.\n");
            break;
        }

        printf("���� �޽��� : %s\n", message);
        send(clntSock, message, (int)strlen(message), 0);
    }

    closesocket(clntSock);
    WSACleanup();
    printf("���� ����..\n");
    return 0;
}
