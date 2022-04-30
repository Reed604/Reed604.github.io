#include<ctype.h> 
#include<sys/types.h> 
#include<unistd.h>
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<signal.h> 
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE sizeof(struct sockaddr_in) 

void check (char gameBoard[]);
void catcher(int sig); 
int newsockfd[2]; 
int pid[2];

int main(void) 
{ 
	int sockfd[2]; 
	char serverRead[1];
	char ans[1];
	int usr=0;
	int ctr=1;
	char move = 0;
	char choice=0;
	char x[4];
	char a[2][40];
  char gameBoard[10] = { '0', '1', '2', '3', '4', '5', '6','7', '8', '9' };
	
	struct sockaddr_in server = {AF_INET, 8001, INADDR_ANY}; 
	strcpy(ans,"");
	static struct sigaction act,act2; 
	act.sa_handler = catcher; 
	sigfillset(&(act.sa_mask)); 
	sigaction(SIGPIPE, &act, 0); 
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGINT, &act, 0);

	if ( (sockfd[0] = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		{ 
		perror("Socket Call Failed"); 
		exit(1); 
		} 

	if ( bind(sockfd[0], (struct sockaddr *)&server, SIZE) == -1) 
		{ 
		perror("Bind Call Failed"); 
		exit(1); 
		}

	printf("--------------------------------------------------------------------------\n");
	printf("			                             TicTacToe	                            \n");
	printf("--------------------------------------------------------------------------\n");
	
	printf("Waiting for players to join in the game \n");

	strcpy(a[0],"Waiting for the other player to join \n");

	while(usr<2)
	{
		if ( listen(sockfd[0], 5) == -1 ) 
		{ 
			perror("Listen Call Failed\n"); 
			exit(1) ; 
		}
		
		newsockfd[usr] = accept(sockfd[0], 0, 0);
		usr++;

		if (usr==1)
		{	
			strcpy(a[1],"0");
			write(newsockfd[0],a,sizeof(a));
			read(newsockfd[0],x,sizeof(x));
			pid[0]=atoi(x);
									
		}

		printf("Both Players have joined\n");
	
		if (usr==2)
		{
			strcpy(a[0],"Let's play TicTacToe! GLHF");
			strcpy(a[1],"1");
			write(newsockfd[0],a,sizeof(a));
			strcpy(a[1],"2");
			write(newsockfd[1],a,sizeof(a));
			read(newsockfd[1],x,sizeof(x));
			pid[1]=atoi(x);
						
		} 	
	}	
	
		if ( fork() == 0) 
		{
			int count=0; 		
			
			while (count==0) 
			{ 	
			
				read(newsockfd[ctr], serverRead, sizeof(serverRead));			
				choice = atoi(serverRead);
				printf("The number received is: %d\n",choice);
				move = choice;
				gameBoard[move] = (ctr==0)?'X':'O';
				
				//check(gameBoard);				
				if(ctr == 1)
					ctr = 0;
				else
					ctr = 1;
				
				write(newsockfd[ctr],gameBoard,sizeof(gameBoard)); 
				check(gameBoard);				
				
			}
			
			close(newsockfd[0]);
			exit (0);
		} 	
			wait();
			close(newsockfd[1]);
}

void check (char gameBoard[])
{
	  int i;
	  char key = ' ';

	  if (gameBoard[1] == gameBoard[2] && gameBoard[2] == gameBoard[3])
        key = gameBoard[1];
        
    else if (gameBoard[4] == gameBoard[5] && gameBoard[5] == gameBoard[6])
        key = gameBoard[4];
        
    else if (gameBoard[7] == gameBoard[8] && gameBoard[8] == gameBoard[9])
        key = gameBoard[7];
        
    else if (gameBoard[1] == gameBoard[4] && gameBoard[4] == gameBoard[7])
        key = gameBoard[1];
        
    else if (gameBoard[2] == gameBoard[5] && gameBoard[5] == gameBoard[8])
        key = gameBoard[2];
        
    else if (gameBoard[3] == gameBoard[6] && gameBoard[6] == gameBoard[9])
        key = gameBoard[3];
        
    else if (gameBoard[1] == gameBoard[5] && gameBoard[5] == gameBoard[9])
        key = gameBoard[1];
        
    else if (gameBoard[3] == gameBoard[5] && gameBoard[5] == gameBoard[7])
        key = gameBoard[3];


	  if (key == 'X')
	  {

		   printf("Player 1 WINS gg\n\n");
		   exit (0); 
	  }

	  if (key == 'O')
	  {

		   printf("Player 2 WINS gg\n\n");
		   exit (0);
	  }
	
}

void catcher(int sig) 
{ 
	printf("Quitting: Comeback soon!\n");
}
