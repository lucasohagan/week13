#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3456
#define BUFFER 1024

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char hostname[256];
    char filename[256];
    FILE *file;
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

    printf("Sending LIST command.\n");
    const char *list_command = "LIST\n";
    ret = send(sock, list_command, strlen(list_command), 0);
    if (ret < 0)
    {
        printf("Error sending LIST.\n");
        return 1;
    }

    printf("Receiving file list.\n");
    ret = read(sock, buffer, sizeof(buffer) - 1);
    if (ret < 0)
    {
        printf("Error reading file list.\n");
        return 1;
    }

    buffer[ret] = '\0';

    printf("Files available for download:\n");
    char *line = strtok(buffer, "\n");
    while (line != NULL)
    {
        printf("%s\n", line);
        line = strtok(NULL, "\n");
    }

    printf("Enter the filename to retrieve:");
    scanf("%s", filename);

    sprintf(buffer, "GET %s\n", filename);
    write(sock, buffer, strlen(buffer));

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    printf("Saving file.\n");
    while ((ret = read(sock, buffer, sizeof(buffer) - 1)) > 0)
    {
        fwrite(buffer, 1, ret, file);
    }

    if (ret < 0)
    {
        printf("Error receiving contents of file.\n");
    }

    fclose(file);
    close(sock);

    printf("File has been downloaded.\n");

    return 0;
}