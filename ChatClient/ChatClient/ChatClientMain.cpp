#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 1024

void ErrorHandling(const char* message) {
    WSACleanup();
    fputs(message, stderr);
    exit(1);
}

int main() {
    WSADATA wsaData;
    SOCKET sock;
    SOCKADDR_IN servAddr;
    char message[BUFSIZE];
    int strLen = 0, nSent;

    // �ϵ��ڵ��� IP�� ��Ʈ
    const char* ipAddress = "127.0.0.1"; // ����ȣ��Ʈ
    int port = 12345; // ����� ��Ʈ

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        ErrorHandling("���� ���� ����\n");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress, &servAddr.sin_addr);

    if (connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("���� ���� ����\n");
    }

    printf("������ ����Ǿ����ϴ�.\n");

    while (1) {
        printf("���� �޽���: ");
        fgets(message, BUFSIZE, stdin);
        message[strcspn(message, "\n")] = 0; // �� �ٲ� ���� ����

        nSent = send(sock, message, strlen(message), 0);
        if (nSent == SOCKET_ERROR) {
            printf("�޽��� ���� ����\n");
            break;
        }

        if (strcmp(message, "exit") == 0) break;

        // ���� �κ� �߰�
        int nRcv = recv(sock, message, sizeof(message) - 1, 0);
        if (nRcv == SOCKET_ERROR) {
            printf("���� ����\n");
            break;
        }
        message[nRcv] = '\0';
        printf("�����κ����� ����: %s\n", message);
    }

    closesocket(sock);
    WSACleanup();
    printf("���� ����..\n");
    return 0;
}
