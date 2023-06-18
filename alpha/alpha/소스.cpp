#pragma warning(disable:4996)
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <conio.h>
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")
struct runrbt {
	SOCKET sock;
	char* ip;
	int port;
	runrbt(char* ip, int port):ip(ip),port(port) {}
	void send(int x, int y, int z, int t, int g, int a, int b) {
		char msg[1000];
		WSADATA wsa;
		hostent* host;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) throw("wsastartup");
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) throw("socket");
		SOCKADDR_IN addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);
		if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) throw("connect");
		sprintf(msg, "...../arm?x=%d&y=%d&z=%d&t=%d&g=%d&m1=%d&m2=%d\n", x, y, z, t, g, a, b);
		::send(sock, msg,strlen(msg), 0);
		printf("$%s",msg);
	}
};
void runcam(char* ip, int port) {
	char tmp[1000];
	sprintf(tmp, "http://%s:%d", ip, port);
	ShellExecuteA(NULL, "open", tmp, NULL, NULL, SW_SHOWNORMAL);
}
int main() {
	char ip[100] = "172.18.52.241";
	int portc, portr;
	//printf("IP ют╥б : ");
	//scanf("%s", ip);
	portc = 8081; portr = 8085;
	Sleep(1000);
	//runcam(ip,portc);
	try {
		runrbt rbt(ip, portr);

		printf("!");
		while (1) {
			char c = getch();
			printf("!%c?", c);
			switch (c) {
			case 'w':
				rbt.send(100, 0, 0, 0, 1, 100, 100);
				Sleep(1000);
				rbt.send(100, 0, 0, 0, 1, 0, 0);
				break;
			case 's':
				rbt.send(100, 0, 0, 0, 1, -100, -100);
				Sleep(1000);
				rbt.send(100, 0, 0, 0, 1, 0, 0);
				break;
			case 'a':
				rbt.send(100, 0, 0, 0, 1, 100, -100);
				Sleep(1000);
				rbt.send(100, 0, 0, 0, 1, 0, 0);
				break;
			case 'd':
				rbt.send(100, 0, 0, 0, 1, -100, 100);
				Sleep(1000);
				rbt.send(100, 0, 0, 0, 1, 0, 0);
				break;
			case '1':
				rbt.send(100, 0, 0, 0, 0, 0, 0);
				Sleep(2000);
				rbt.send(100, 0, -150, 0, 0, 0, 0);
				Sleep(15000);
				rbt.send(100, 0, -150, 0, 1, 0, 0);
				Sleep(2000);
				rbt.send(100, 0, 0, 0, 1, 0, 0);
				break;
			}
		}
	}
	catch (char* e) { printf("%s\n", e); }
}