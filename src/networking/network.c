/**
 * @file This file provides the implementation of network module.
 */

#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errors.h>
#include "network.h"

int sockfd = -1;

int network_connect(char *hostname, int port)
{
    if (sockfd > 0)
        return CONNECTION_IN_USE;

    if (hostname == NULL)
        return POINTER_NULL;

    if (port < 0 || port > 65535)
        return PARAMETER_OUT_OF_RANGE;

    struct hostent *server = gethostbyname(hostname);
    if (server == NULL)
        return INVALID_HOSTNAME;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return CONNECTING_ERROR;

    struct sockaddr_in serv_addr;

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    memmove((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr_list[0],
            server->h_length);

    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sockfd);
        sockfd = -1;
        return CONNECTING_ERROR;
    }

    return NO_ERROR;
}

int network_send(void *data, size_t size)
{
    if (data == NULL)
        return POINTER_NULL;

    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;

    if (size == 0)
        return PARAMETER_OUT_OF_RANGE;

    if (write(sockfd, data, size) <= 0)
        return WRITING_ERROR;

    return NO_ERROR;
}

int network_read(void *buffer, size_t size)
{
    if (buffer == NULL)
        return POINTER_NULL;

    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;

    if (size == 0)
        return PARAMETER_OUT_OF_RANGE;

    int bytes_read = read(sockfd, buffer, size);

    if (bytes_read < 1)
        return READING_ERROR;

    return bytes_read;
}

int network_disconnect()
{
    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;
    else {
        close(sockfd);
        sockfd = -1;
        return NO_ERROR;
    }
}

int network_readLine(char *buffer, size_t size)
{
    // Cannot read a line longer than the buffer size.
    if (size > BUFFER_SIZE)
        return PARAMETER_OUT_OF_RANGE;

    // Create a temporary buffer.
    static char internalBuffer[BUFFER_SIZE];
    static int bufferedBytes = 0;

    int bytesRead = network_read(internalBuffer + bufferedBytes,
                                 size - 1 - bufferedBytes);

    if (bytesRead < 1) {
        return READING_ERROR;
    }

    bufferedBytes += bytesRead;

    // Iterate over the internalBuffer elements and copy them to the parameter
    // buffer until current character is a new line or the size of the
    // parameter buffer has been exceeded.
    int bufferIndex = 0;
    while (internalBuffer[bufferIndex] != '\n' && bufferIndex < size - 1) {
        buffer[bufferIndex] = internalBuffer[bufferIndex];
        bufferIndex++;
    }

    if (internalBuffer[bufferIndex] == '\n') {
        buffer[bufferIndex] = internalBuffer[bufferIndex];
        bufferIndex++;
    }

    // Set last character to be a NUL.
    buffer[bufferIndex] = '\0';

    return NO_ERROR;
}
