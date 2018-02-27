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

#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <thread>
#include <functional>
#include <netinet/in.h>

#define UDPSERVER_BUFLEN 512

#define UDPSERVER_ERROR_NONE 0
#define UDPSERVER_ERROR_SOCKET 1
#define UDPSERVER_ERROR_BIND 2
#define UDPSERVER_ERROR_RECEIVE 3

class UDPServer {
public:

	/**
	 * Creates a UDPServer to listen on port.
	 */
	UDPServer(uint16_t port);

	/**
	 * Deallocates the servers resources.
	 */
	virtual ~UDPServer();

	/**
	 * Registers a callback for when this server receives a broadcast packet on the specified port.
	 */
	void setBroadcastCallback(std::function<void(in_addr, uint16_t, uint8_t[], size_t)> callback);

	/**
	 * Initializes the socket.
	 */
	int init();

	/**
	 * Start the server's listener thread.
	 */
	void start();

	/**
	 * Stops the server's listener thread.
	 */
	void stop();

	/**
	 * Deinitializes the socket;
	 */
	void close();

	/**
	 * Is the server running?
	 */
	bool isRunning();

	/**
	 * Gets the error state of this server.
	 */
	int getError();

private:

	uint16_t port;
	int socketHandle;
	std::function<void(in_addr, uint16_t, uint8_t[], size_t)> broadcastCallback;

	std::thread *t;
	bool running;
	int error;

	void serverLoop();

};

#endif // UDPSERVER_H
