#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

#define PORT_NUMBER 8080

struct request {
	char method[8];
	char path[128];
};

// Generalizing intake of 'method' for scalability for future http1 implementation
void get_request_from_http_msg(struct request *req, char *http_msg) {
	if (http_msg == NULL || strcmp(http_msg, "") == 0) {
		perror("get_http_request_substr: 'src' is NULL or empty");
		return;
	}
	char *token = NULL;
	memset(req, 0, sizeof(struct request));

	// Extract request substring out of the http message
        token = strtok(http_msg, " ");
	strcpy(req->method, token);
        token = strtok(NULL, " ");
        strcpy(req->path, token);

	//printf("req->method = %s, req->path = %s\n", req->method, req->path);
}

int main(void) {
	printf("Server says hello!\n");

	int sock_fd;
	struct sockaddr_in my_addr;
	int opt = 1;

	unsigned char request[128] = { "" };
	unsigned char recv_buf[128] = { "" };

	if ( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(sock_fd,
		       SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		       &opt, sizeof(opt))) {
        	perror("setsockopt");
        	exit(EXIT_FAILURE);
   	}

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(PORT_NUMBER);
	if (bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(sock_fd, 10) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while(1) {
		int client_sfd;
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);

		if ((client_sfd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
			perror("accept");
			continue;
		}
		puts("~~~~~~~~~~~");

		int r = recv(client_sfd, recv_buf, ARRAY_LENGTH(recv_buf), 0);
		if (r < 0) {
			perror("recv");
			continue;
		}

		struct request req;
		get_request_from_http_msg(&req, recv_buf);
		printf("%s %s\n", req.method, req.path);

		// Service stuff
		memset(recv_buf, 0, ARRAY_LENGTH(recv_buf));
	}

	return 0;
}
