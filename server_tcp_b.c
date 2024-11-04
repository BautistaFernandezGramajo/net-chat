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

int main() {
    int sockfd, connfd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    // Crear socket TCP
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Llenar información del servidor
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Enlazar el socket con la dirección del servidor
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(sockfd, 5) < 0) {
        perror("listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    len = sizeof(cliaddr);

    // Aceptar una conexión
    if ((connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len)) < 0) {
        perror("accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connection accepted.\n");

    // Recibir mensaje del cliente
    int n = recv(connfd, buffer, MAXLINE, 0);
    if (n < 0) {
        perror("receive failed");
        close(connfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    // Enviar respuesta al cliente
    send(connfd, hello, strlen(hello), 0);
    printf("Hello message sent.\n");

    // Cerrar el socket de la conexión y el socket principal
    close(connfd);
    close(sockfd);
    return 0;
}
