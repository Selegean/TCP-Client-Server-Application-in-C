#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char * get_html_page_client() {


	printf("\t\t ~~ Client ~~~\n");
	
	char *msg = "GET / HTTP/1.0\r\n\r\n";  // cerere de tip HTTP GET, folosind protocolul HTTP 1.0
	int sockfd = socket(AF_INET6, SOCK_STREAM,0); // socket IPv6, tip TCP, protocol corespunzator tipului
	FILE *htmlFile; // pointer pentru lucrul cu fisier
	char buffer[50]; // buffer pentru obtinerea raspunsului de la server

	if(sockfd == -1) {
		printf("\t\t -> Eroare la crearea socket-ului");
		return 0;
	}
	else
	{
		printf("\t\t -> Socket creat cu succes\n");
	}

	// hints va fi folosit pentru a specifica criteriile de cautare,
	// iar res va fi utilizat pentru a stoca rezultatele cautarii.
	struct addrinfo hints, *res;
	
	// setarea parametrilor pentru hint a.i. sa se obtina adresa IPv6
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; 
	hints.ai_socktype = SOCK_STREAM; //socket TCP
	
	
	// apelarea functiei ce va obtine informatiile despre server	
	int status;
	if ((status = getaddrinfo("www.traceroute6.net", "80", &hints, &res)) != 0) {
    		printf("\t\t -> Eroare getaddrinfo");
    		return 0;
	}

	//conectarea la server
	if(connect(sockfd,res->ai_addr, res->ai_addrlen) != -1){
		printf("\t\t -> Conectare cu succes\n");
	}
	else
	{
		printf("\t\t -> Connect error");
		return 0;
	}

	// numarul de bytes trimisi catre server
	int bytes_sent = send(sockfd, msg, strlen(msg), 0);

	if (bytes_sent == -1) {
		printf("\t\t -> Cererea GET nu a fost trimis\n");
	}
	else
	{
		printf("\t\t -> Cererea GET a fost trimis cu succes.\n");
	}

	// deschiderea fisierului in care se stocheaza contiunutul paginii html
	htmlFile = fopen("pagina.html","w");
	if(htmlFile == NULL){
		printf("\t\t -> Fisierul nu a putut fi deschis pentru citire.");
	}

	// alocarea memoriei pentru variabila in care se va construi continutul paginii html
	char *output = malloc(40000);

	while(1)
	{
		// returnarea numarului de octeti primiti de la server
		int bytes_recv = recv(sockfd,buffer,sizeof(buffer)-1,0);
	
		if(bytes_recv == -1){
			printf("Nu s-a putut receptiona informatia\n");
			break;
		}

		// receptia a fost finalizata
		if(bytes_recv == 0){
			printf("Receive incheiat\n");
			break;
		}

		// se adauga terminatorul null la continutul primit in buffer
		// pentru a indica sfarsitul continutului de date receptionat
		buffer[bytes_recv] = '\0';

		// construirea continutului paginii html prin concatenarea informatiei receptionate
		strcat(output,buffer);	
	}

	// scrierea continutului paginii html in fisier
	fprintf(htmlFile,output);
	//inchiderea pointerului de fisier
	fclose(htmlFile);

	//inchiderea socket-ului.	
	close(sockfd);
	
	//returnarea continutului paginii ca si sir de caractere
	return output;
}