/*
 * Copyright (c) 2018 Jedidiah Pommert <email>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "udpserver.h"

UDPServer::UDPServer(uint16_t port) : port(port) {
	socketHandle = 0;
	running = false;
	error = UDPSERVER_ERROR_NONE;
}

UDPServer::~UDPServer() {
	if (t != nullptr) {
		delete t;
	}
}

void UDPServer::setBroadcastCallback(std::function<void (in_addr, uint16_t, uint8_t[], size_t)> callback) {
	broadcastCallback = callback;
}

int UDPServer::init() {
	sockaddr_in myAddress;

	if ((socketHandle = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error = UDPSERVER_ERROR_SOCKET;
		return UDPSERVER_ERROR_SOCKET;
	}

	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = INADDR_ANY;
	myAddress.sin_port = htons(port);
	memset(&(myAddress.sin_zero), 0, 8);

	if (bind(socketHandle, (sockaddr *) &myAddress, sizeof(sockaddr)) < 0) {
		error = UDPSERVER_ERROR_BIND;
		return UDPSERVER_ERROR_BIND;
	}

	return UDPSERVER_ERROR_NONE;
}

void UDPServer::start() {
	if (!running) {
		running = true;
		t = new std::thread(&UDPServer::serverLoop, this);
	}
}

void UDPServer::stop() {
	running = false;
	if (t != nullptr) {
		t->join();
		delete t;
	}
}

void UDPServer::close() {
	if (!running) {
		::close(socketHandle);
	}
}

bool UDPServer::isRunning() {
	return running;
}

int UDPServer::getError() {
	return error;
}


void UDPServer::serverLoop() {
	sockaddr_in otherAddress;
	socklen_t addressLen;
	ssize_t packetLen;
	uint8_t buf[UDPSERVER_BUFLEN];
	fd_set socketSet;
	timeval selectWait;
	int selectResult;

	selectWait.tv_sec = 0;
	selectWait.tv_usec = 20000;

	while (running) {
		FD_ZERO(&socketSet);
		FD_SET(socketHandle, &socketSet);

		selectResult = select(socketHandle + 1, &socketSet, nullptr, nullptr, &selectWait);
		if (selectResult < 0) {
			error = UDPSERVER_ERROR_RECEIVE;
		} else if (selectResult > 0 && FD_ISSET(socketHandle, &socketSet)) {
			addressLen = sizeof(sockaddr);
			if ((packetLen = recvfrom(socketHandle, buf, UDPSERVER_BUFLEN - 1, 0, (sockaddr *) &otherAddress, &addressLen)) < 0) {
				error = UDPSERVER_ERROR_RECEIVE;
			}

			broadcastCallback(otherAddress.sin_addr, otherAddress.sin_port, buf, packetLen);
		}
	}
}
