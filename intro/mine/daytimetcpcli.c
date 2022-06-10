#include "unp.h" // numerous system headers that are needed by most network programs and defines various constants that we use (e.g., MAXLINE).

int main(int argc, char **argv) {
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr; // an Internet socket address structure (a sockaddr_in structure named servaddr) with the server's IP address and port number.
	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //The socket function creates an Internet (AF_INET) stream (SOCK_STREAM) socket, which is a fancy name for a TCP socket. The function returns a small integer descriptor that we can use to identify the socket in all future function calls (e.g., the calls to connect and read that follow).
		err_sys("socket error");
	bzero(&servaddr, sizeof(servaddr)); // set the entire structure to 0 using bzero
	servaddr.sin_family = AF_INET; // set the address family to AF_INET
	servaddr.sin_port   = htons(13); /* daytime server */ // set the port number to 13 (which is the well-known port of the daytime server on any TCP/IP host that supports this service
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) // set the IP address to the value specified as the first command-line argument (argv[1]). The IP address and port number fields in this structure must be in specific formats: We call the library function htons ("host to network short") to convert the binary port number, and we call the library function inet_pton ("presentation to numeric") to convert the ASCII command-line argument (such as 206.62.226.35 when we ran this example) into the proper format. Older code uses the inet_addr function to convert an ASCII dotted-decimal string into the correct format, but this function has numerous limitations that inet_pton corrects.
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) // connect function, when applied to a TCP socket, establishes a TCP connection with the server specified by the socket address structure pointed to by the second argument. We must also specify the length of the socket address structure as the third argument to connect, and for Internet socket address structures, we always let the compiler calculate the length using C's sizeof operator. we #define SA to be struct sockaddr,that is,a generic socket address structure.
		err_sys("connect error");
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) { // read the server's reply and display the result using the standard I/O fputs function. We must be careful when using TCP because it is a byte-stream protocol with no record boundaries. The server's reply is normally a 26-byte string of the form.
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");
	exit(0);
}
