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

    // 하드코딩된 IP와 포트
    const char* ipAddress = "127.0.0.1"; // 로컬호스트
    int port = 12345; // 사용할 포트

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        ErrorHandling("소켓 생성 에러\n");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress, &servAddr.sin_addr);

    if (connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("서버 연결 에러\n");
    }

    printf("서버에 연결되었습니다.\n");

    while (1) {
        printf("보낼 메시지: ");
        fgets(message, BUFSIZE, stdin);
        message[strcspn(message, "\n")] = 0; // 줄 바꿈 문자 제거

        nSent = send(sock, message, strlen(message), 0);
        if (nSent == SOCKET_ERROR) {
            printf("메시지 전송 에러\n");
            break;
        }

        if (strcmp(message, "exit") == 0) break;

        // 수신 부분 추가
        int nRcv = recv(sock, message, sizeof(message) - 1, 0);
        if (nRcv == SOCKET_ERROR) {
            printf("수신 에러\n");
            break;
        }
        message[nRcv] = '\0';
        printf("서버로부터의 응답: %s\n", message);
    }

    closesocket(sock);
    WSACleanup();
    printf("연결 종료..\n");
    return 0;
}
