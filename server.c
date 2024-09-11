#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "client.c"
#include <unistd.h>
#define BACKLOG 10



int main(){

	printf("~ Server ~ \n");

	// portul pe care se vor primi conexiuni	
	int port = 22315;

	// structura pentru adresa serverului
	struct sockaddr_in myaddr;

	//codul comenzii implementate
	char msgNr[] = "06#";
	char msg_neimplem[] = "Commanda neimplementata\n";
	struct sockaddr_in client;
	char msg[1000];
	int clientSize;	
	int recv_size;	

	// initializarea structurii pentru adresa serverului
	memset(&myaddr, 0, sizeof(myaddr));
	
	myaddr.sin_family = AF_INET; // IPv4 
	myaddr.sin_port = htons(port); // specificarea portului 
	
	// Programul server trebuie sa asculte pe toate adrese ip pe care le are calculatorul
	myaddr.sin_addr.s_addr = INADDR_ANY; 
	
	// crearea socket 
	int s = socket(PF_INET, SOCK_STREAM, 0);
	
	// legarea socket-ului la adresa serverului 
	int b_success = bind(s, (struct sockaddr*)&myaddr, sizeof(myaddr));
	
	if( b_success != 0){
	
		printf("Eroare la bind \n");
		return 0;
	}
	else {
		printf("Bind corect (｡◕‿◕｡) \n");
	}

	// ascultarea pentru conexiouni de la clienti
	int lis = listen(s,BACKLOG);
	
	if(lis != 0){

		printf("Nu se pot accepta clienti \n ");
	}
	else {
		printf("Server deschis conexiunii pe portul 22315 \n");
	}

	
	// initializarea structurii pentru adresa clientului
	memset(&client, 0, sizeof(client));	

	while(1) {

		// acceptarea unei conexiuni de la un client
		int con = accept(s,(struct sockaddr *)&client, &clientSize);
		if (con == -1 ){
			printf("Eroare la acceptare client \n");
		}
		else
		{
			printf("Client acceptat cu succes \n");
		}
		
		// crearea unui proces copil pentru fiecare client servit
		if(!fork()) {
			//primirea comenzii date de catre client
			while( recv_size = recv(con,msg,1000,0)> 0) {
				// verificarea implemntarii comenzii	
				if(strcmp(msg,msgNr) == 0){
					// obtinerea paginii html
					char * clientMsg = get_html_page_client();
					// trimiterea raspunsului de la server la client	
					send(con, clientMsg, strlen(clientMsg),0);	
				}
				else{
					// trimiterea mesajului in cazul comnenzii neimplemntate
					send(con, msg_neimplem, strlen(msg_neimplem),0);}
				}

			if (recv_size == 0){
				printf("Client deconectat..\n");
			}

			// inchiderea conexiunii cu procesul copil.
			close(con);
		}
		// inchiderea conexiunii cu procesul parinte
		close(con);
	}
	// inchiderea socket-ului serverului.
	close(s);
}