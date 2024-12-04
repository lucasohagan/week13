#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3456

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char hostname[256];
    int choice;
    int ret;

    printf("Which server would you like to connect to?:\n");
    printf("1. Newark (newark.cs.sierracollege.edu)\n");
    printf("2. London (london.cs.sierracollege.edu)\n");
    printf("Enter 1 or 2 to choose your server: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        strcpy(hostname, "newark.cs.sierracollege.edu");
    }
    else if (choice == 2)
    {
        strcpy(hostname, "london.cs.sierracollege.edu");
    }
    else
    {
        printf("Invalid choice, closing.\n");
        return 1;
    }

    printf("Socket creating.  \n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("Socket creation failed.  \n");
        return 1;
    }

    printf("Setting up address.  \n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (strcmp(hostname, "newark.cs.sierracollege.edu") == 0)
    {
        server_addr.sin_addr.s_addr = inet_addr("45.79.143.41");
    }
    else if (strcmp(hostname, "london.cs.sierracollege.edu") == 0)
    {
        server_addr.sin_addr.s_addr = inet_addr("178.79.184.64");
    }

    printf("Connecting to server %s \n", hostname);
    ret = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        printf("Connection failed.  \n");
        return 1;
    }

    printf("Reading greeting.  \n");
    ret = read(sock, buffer, sizeof(buffer) - 1);
    if (ret < 0)
    {
        printf("Error.  \n");
        return 1;
    }

    buffer[ret] = '\0';
    printf("Server: %s\n", buffer);

    printf("Sending HELO server.\n");
    const char *helo_command = "HELO\n";
    ret = send(sock, helo_command, strlen(helo_command), 0);
    if (ret < 0)
    {
        printf("Error sending HELO.\n");
        return 1;
    }

    ret = read(sock, buffer, sizeof(buffer) - 1);
    if (ret < 0)
    {
        printf("Error reading server response.\n");
        return 1;
    }

    buffer[ret] = '\0';
    printf("Server: %s\n", buffer);

    close(sock);
    return 0;
}