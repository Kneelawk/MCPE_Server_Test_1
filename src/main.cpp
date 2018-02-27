#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

#include "udpserver.h"

#define MCPE_PORT 19132
#define BUF_LEN 512

#define MCPE_MAGIC_NUMBER 0x00ffff00fefefefefdfdfdfd12345678

using namespace std;

void printChars(uint8_t *data, size_t len) {
	for (int i = 0; i < len; i++) {
		cout << hex << setfill('0') << setw(2) << (unsigned int) data[i];
		if (i % 16 == 15) {
			cout << endl;
		} else {
			cout << " ";
		}
	}
	cout << endl;
}

void messageReceived(in_addr addr, uint16_t port, uint8_t *buf, size_t len) {
	cout << "Got a packet from " << inet_ntoa(addr) << endl;
	cout << "The packet is " << dec << len << " bytes long" << endl;
	cout << "The packet contains:" << endl;
	printChars(buf, len);
}

int main(int argc, char **argv) {
	UDPServer serv(MCPE_PORT);

	serv.setBroadcastCallback(&messageReceived);

	cout << "Starting Server..." << endl;
	cout << "Type 'stop' to stop." << endl;
	serv.init();

	serv.start();

	string in;
	while (in != "stop") {
		getline(cin, in);
	}

	serv.stop();

	serv.close();

	cout << "Done." << endl;
	return 0;
}
