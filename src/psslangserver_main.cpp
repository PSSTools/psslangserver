/*
 * psslangserver_main.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: ballance
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "PSSLangServer.h"
#include "LangServerMethodHandler.h"
#include "MessageDispatcher.h"
#include "SocketMessageTransport.h"

int main(int argc, char **argv) {
	int32_t port = -1;

	for (uint32_t i=1; i<argc; i++) {
		if (argv[i][0] == '-') {
			if (!strcmp(argv[i], "-port")) {
				i++;
				port = atoi(argv[i]);
			} else {
				fprintf(stdout, "Error: unknown option %s\n", argv[i]);
				exit(1);
			}
		} else {
			fprintf(stdout, "Error: unknown argument %s\n", argv[i]);
			exit(1);
		}
	}

	// First, connect to the socket
	struct sockaddr_in serv_addr;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serv_addr.sin_port = htons(port);

	fprintf(stdout, "--> langserver connecting...\n");
	fflush(stdout);
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect failure");
		exit(1);
	}
	fprintf(stdout, "connected %d\n", sock);
	fflush(stdout);

	lls::MessageDispatcher dispatcher;
	lls::SocketMessageTransport transport(sock);

	PSSLangServer server;

	transport.init(&dispatcher);

	lls::LangServerMethodHandler lsp_method_handler(
			&transport,
			&server);

	lsp_method_handler.register_methods(&dispatcher);

	while (transport.process(0)) {
		//
	}

	return 0;
}


