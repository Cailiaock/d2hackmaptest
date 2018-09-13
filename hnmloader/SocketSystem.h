class SocketSystem
{
public:
	SocketSystem() {
		WORD wVersionRequested;
		WSADATA wsaData;
		wVersionRequested = MAKEWORD(2, 0);
		WSAStartup(wVersionRequested, &wsaData);
	}
	~SocketSystem() {
		WSACleanup();
	}
};