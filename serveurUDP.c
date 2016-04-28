#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define RCVSIZE 1024

int test(int i,char buffer[]){
	char ss[10];
	char str[8];
	char ack1[10]="ACK000000";
	sprintf(str,"%d",i);
	strcpy(ss,ack1);
	strcat(ss,str);
	if (strcmp(ss,buffer) == 0){
		return 1;
	}else{
		return 2;
	}
}


int main (int argc, char *argv[]) {
	
  struct sockaddr_in serveur,strucontrole,nvstruct,structprincipale;
  int port= atoi(argv[1]);
		
  int valid= 1;
  char buffer[RCVSIZE];
  char msg[RCVSIZE];
  char synack[12]="SYN-ACK4567";
  socklen_t alen=sizeof(structprincipale);
  socklen_t al1=sizeof(strucontrole);
  socklen_t nval=sizeof(nvstruct);

   
  int len=strlen(synack);
  int nvport=4567;
  
  //create socket
  int desc= socket(AF_INET, SOCK_DGRAM, 0);
  int desc1=socket(AF_INET, SOCK_DGRAM, 0);

  // handle error
  if (desc < 0) {
    perror("cannot create socket\n");
    return -1;
  }

  setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  serveur.sin_family= AF_INET;
  serveur.sin_port= htons(port);
  serveur.sin_addr.s_addr= htonl(INADDR_ANY);

  if (bind(desc, (struct sockaddr*) &serveur, sizeof(serveur)) == -1) {
    perror("Bind fail\n");
    close(desc);
    return -1;
  }
  
  
  
  int rec0=recvfrom (desc, msg,RCVSIZE , 0, (struct sockaddr *)&strucontrole,&al1);
  printf("%d %s reçu\n",rec0,msg);
  if(strcmp(msg,"SYN") == 0){
  	 int envoie0=sendto(desc , synack, len, 0, (struct sockaddr *)&strucontrole , al1);
  	 printf("sent %d\n", envoie0);
  	 memset(msg,0,RCVSIZE);
  	
  }
	  
   //printf("ici msg nul  %s je sors du if \n",msg);
     int rec1=recvfrom (desc, msg,RCVSIZE , 0, (struct sockaddr *)&strucontrole,&al1);
     printf("%d %s reçu(ici recevoir ack)\n",rec1,msg);
	 memset(msg,0,RCVSIZE);
	 int envoie1=sendto(desc , "4567", 5, 0, (struct sockaddr *)&strucontrole , al1);
	 
 	structprincipale.sin_family= AF_INET;
  	structprincipale.sin_port= htons(nvport);
  	structprincipale.sin_addr.s_addr= htonl(INADDR_ANY);
	 
  
    setsockopt(desc1, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  if (bind(desc1, (struct sockaddr*) &structprincipale, alen) == -1) {
    perror("Bind fail\n");
    close(desc);
    return -1;
  }
  
  

  printf("avant la boucle\n");
  
  int cont=1;
  
  
  if (strcmp(msg,"stop\n") == 0) {
	      cont= 0; 
	    }

	 
	char buff[100000];
	int a=recvfrom (desc1, buff,100000 , 0, (struct sockaddr *)&nvstruct,&nval);
	
	printf("après la boucle j'ai retenu la structure du client %d %s \n",a,buff);
	FILE *fp;
	char nomdefichier[20];
	strcpy(nomdefichier,buff);
	printf("le nom de fichier est %s\n",nomdefichier);
	fp = fopen(nomdefichier,"rb");
	memset(buff,0,100000);
	

	
 while (cont) {		
			
			

			 int r=fread(buff,1,1000,fp);
			 int size=strlen(buff);
			printf("le size du buffer esr %d \n",size);
			int envoie=sendto (desc1 , buff, r, 0, (struct sockaddr *)&nvstruct,nval);
			int w,i=1;
			

				
				

			  while(r==1000){
				
				memset(buff,0,100000);
				r=fread(buff,1,1000,fp);
				char ss[10];
				char str[8];
				char seq[1008];
				if(i<10){
				char seq[1008]="00000";
				}else if(i>=10 && i<100){
				char seq[1008]="0000";
				}else if(i>=100 && i<1000){
				char seq[1008]="000";
				}else if(i>=1000 && i<10000){
				char seq[1008]="00";
				}else if(i>=10000 && i<100000){
				char seq[1008]="0";
				}else if(i>=10000 && i<100000){
				char *seq=NULL;
				}
				printf("la séquence avant est %s\n",seq);
			  	sprintf(ss,"%d",i);
				//printf("la séquence avant est %s\n",seq);
				//strcpy(ss,ack1);
				strcat(seq,ss);
				printf("le buffer est %s\n",buff);
				strcat(seq,buff);
				printf("la séquence est %s\n",seq);



				   
				int envoie=sendto (desc1 , buff, r, 0, (struct sockaddr *)&nvstruct,nval);


				 printf("envoie du fichier numéro %d la valeur d'envoie est %d \n",i,envoie);
					
				
				while(recvfrom (desc1, buffer,RCVSIZE , 0, (struct sockaddr *)&nvstruct,&nval)==NULL){printf("dans while");
				sendto (desc1 , buff, r, 0, (struct sockaddr *)&nvstruct,nval);
				}
				if(test(i,buffer)==1){ 
					printf("%s\n",buffer);
					memset(buffer,0,RCVSIZE);
					i++;
				}else{
				printf("ack non reçu retransmission du paquet");
				int envoie=sendto (desc1 , buff, r, 0, (struct sockaddr *)&nvstruct,nval);
				}
				


			 }
			r=fread(buff,1,1000,fp);
		       sendto (desc1 , buff, r, 0, (struct sockaddr *)&nvstruct,nval);
			cont=0;
			 
	}
    // receive 
    /*memset(buffer,0,RCVSIZE);
    int msgSize=recvfrom (desc, buffer,RCVSIZE , 0, (struct sockaddr *)&structprincipale,&alen);
    printf("recu %d %s\n", msgSize, buffer);
    
		while(msgSize>0){
	      //printf("%s\n",buffer);
	      int envoie=sendto (desc , buffer, strlen(buffer), 0, (struct sockaddr *)&structprincipale, sizeof(structprincipale));
	      printf("sent %d %s\n", envoie, buffer);
		  memset(buffer,0,RCVSIZE);
		  msgSize=recvfrom (desc, buffer,RCVSIZE , 0, (struct sockaddr *)&structprincipale,&alen);
		  printf("recv %d %s\n", msgSize, buffer);
	    }
	    
	   */
	    
	 
	

  


close(desc);
close(desc1);
return 0;

}
