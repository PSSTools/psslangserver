/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*
 * psslangserver_main.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: ballance
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef _WIN32
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
typedef int socket_t;
#else
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET socket_t;
#endif
#include <string.h>
#include "PSSLangServer.h"
#include "LangServerMethodHandler.h"
#include "MessageDispatcher.h"
#include "SocketMessageTransport.h"
#ifndef _WIN32
#include <signal.h>
#include <execinfo.h>
#include <cxxabi.h>
#endif


using namespace pls;

#ifndef _WIN32
static void sig_handler(int signum) {
	void *array[16];
	char **strings;
	int size;

	fprintf(stdout, "Signal Handler called\n");
	fflush(stdout);

	size = backtrace (array, sizeof(array)/sizeof(void *));
	strings = backtrace_symbols (array, size);
	if (strings) {
		for (uint32_t i = 0; i < size; i++) {
			int status = -1;
			char *p_start, *p_end;
			if ((p_start=strchr(strings[i], '(')) &&
					((p_end=strchr(p_start, '+')))) {
				*p_end = 0;
				char *demangledName = abi::__cxa_demangle(p_start+1, NULL, NULL, &status);
				*p_start = 0;
				fprintf(stdout, "%s(%s+%s\n", strings[i], demangledName, p_end+1);
				free(demangledName);
			} else {
				fprintf(stdout, "%s\n", strings[i]);
			}
			fflush(stdout);
		}
	}
	free(strings);
	exit(1);
}
#endif

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

	if (port == -1) {
		fprintf(stdout, "Error: no -port specified\n");
		exit(1);
	}

#ifndef _WIN32
	signal(SIGSEGV, &sig_handler);
#else
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
	    printf("WSAStartup failed: %d\n", iResult);
	    return 1;
	}
#endif

	// First, connect to the socket
	struct sockaddr_in serv_addr;

	socket_t sock = socket(AF_INET, SOCK_STREAM, 0);

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


