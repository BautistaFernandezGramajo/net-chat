#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024
#define SERV_HOST_ADDR "10.0.19.20" // Dirección IP del servidor

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in servaddr;

    // Creación del socket TCP
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Llenado de información del servidor
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);

    // Conexión al servidor
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connection to the server failed");
        exit(EXIT_FAILURE);
    }

    // Enviar mensaje al servidor
    send(sockfd, hello, strlen(hello), 0);
    printf("Hello message sent.\n");

    // Recibir respuesta del servidor
    int n = recv(sockfd, buffer, MAXLINE, 0);
    if (n < 0) {
        perror("receive failed");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    // Cerrar el socket
    close(sockfd);
    return 0;
}
