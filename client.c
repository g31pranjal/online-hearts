#include <stdio.h>
#include <stdlib.h>
// socket(), connect(), send(), recv()
#include <sys/socket.h>
// socketaddr_in and in_addr structures
#include <netinet/in.h>
#include <string.h>
// inet_addr structure
#include <arpa/inet.h>

#include <unistd.h>


#ifndef BOLDBLACK
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#endif

#ifndef BOLDRED
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#endif

#ifndef BOLDGREEN
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#endif

#ifndef BOLDYELLOW
#define BOLDYELLOW  "\033[1m\033[33m"      // user inputs. 
#endif

#ifndef BOLDBLUE
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#endif

#ifndef BOLDMAGENTA
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#endif

#ifndef BOLDCYAN
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#endif

#ifndef BOLDWHITE
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#endif

#ifndef RESET
#define RESET   "\033[0m"
#endif


int main() {

	printf("%s----------------------------------------------- HEARTS v.1 -----------------------------------------------%s\n", BOLDRED, RESET);

	int clientSocket;
	int flag = 0, highestIndex = -1;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	int i, c, played, tmp, j, k, command, playingSuit, playingCardPresent, p1, p2, zeroScore;
	int cards[13], roundCards[4], lScore[4], gScore[4];
	int rIter = 0, port = 0, connS = -1;
	char sAdr[20];
	char * tokens, * splayed;

	// initializing global scores
	for(c=0;c<4;c++) {
		gScore[c] = 0;
	}
	
	int LEVEL, ROUND, NAMEID ;
	char NAME[4][1024], CPLAYER[1024];
	char * suit[] = { "\u2663"/*club*/, "\u2666"/*diamond*/, "\u2665"/*hearts*/, "\u2660"/*spades*/ };
	char * valu[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};


	// get the server details from the user.
	printf("Enter the server details :\n");
	printf("server address \t :\t " );
	memset(sAdr, 0, 20);
	scanf("%s", sAdr);
	printf("server port \t :\t " );
	scanf("%d", &port);


	// create network socket
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family  = AF_INET;
	serverAddr.sin_port  = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(sAdr);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	
	addr_size = sizeof serverAddr;

	while(connS != 0) {
		printf("\n\n%sConnecting to server at %s:%d\n%s", BOLDBLUE, sAdr, port, RESET);
		connS = connect(clientSocket, (struct sockaddr*) &serverAddr, addr_size );

		if(connS == -1) {
			printf("Connection refused. retrying...\n");		
			sleep(2);
		}
	}

	while(1) {
		// command 
		if(flag == 0)
		{
			if( ( i = recv(clientSocket, buffer, 1024, 0) )  > 0) {
				// printf("received %d bytes : %s\n", i, buffer);
				if(!strcmp(buffer,""))
				{
					printf("%s\n\nERROR : communication problem , server closing the room.\n%s",BOLDRED, RESET);
					flag = 1;
					break;
				}
				command = atoi(buffer);

				switch(command) {

					// 1 : First acknowledgment signal to client, connection established
					case 1 : 
						printf("Connected.");
						printf("%s\n\nWaiting for other players to connect to this room...\n%s", BOLDBLUE, RESET);
						break;

					// 2 : room closed. Each user is alloted random names.
					case 2 : 
						printf("%s\n\nRoom closed. dealing begins soon...\n%s", BOLDYELLOW, RESET);
						memset(buffer, '\0', sizeof buffer);
						while( 	( i = recv(clientSocket, buffer, 1024, 0) )  < 0);
						// printf("ye wala buffer : %s\n",buffer );
						if(!strcmp(buffer,""))
						{
							printf("%s\n\nERROR : communication problem , server closing the room.\n%s",BOLDRED, RESET);
							flag = 1;
							break;
						}
						tokens = strtok(buffer, ",");
							
						for(c=0;c<4;c++) {

							 strcpy(NAME[c],tokens);
							tokens = strtok(NULL, ",");
						}

						memset(buffer, '\0', sizeof buffer);
						if(recv(clientSocket , buffer , 1024 , 0) <0);		
						NAMEID = atoi(buffer);		
						printf("%s\n\nYour alloted name is : %s%s\n",BOLDYELLOW, NAME[NAMEID], RESET);
						break;

					// 3 : level 1 begins and card dealing 
					case 3 : 

						printf("%s\n\nDealing for LEVEL 1 ... receiving set of cards .\n%s", BOLDBLUE, RESET);
						LEVEL = 1;

						// initiating level scores array
						for(c=0;c<4;c++) {
							gScore[c] += lScore[c];
							lScore[c] = 0;
						}
						
						// receiving cards information
						memset(buffer, '\0', sizeof buffer);
						while( ( i = recv(clientSocket, buffer, 1024, 0) )  < 0);
						// printf("cards received (%d bytes) : \"%s\"\n", i, buffer);
						
						// decoding cards
						if(!strcmp(buffer,""))
						{
							printf("%s\n\nERROR : communication problem , server closing the room.\n%s",BOLDRED, RESET);
							flag = 1;
							break;
						}
						tokens = strtok(buffer, ",");
						for(c=0;c<13;c++) {
							cards[c] = atoi(tokens);
							tokens = strtok(NULL, ",");
						}

						for(j=0;j<13;j++) {
							for(k=0;k<12;k++) {
								if(cards[k+1] < cards[k]) {
									tmp = cards[k+1];
									cards[k+1] = cards[k];
									cards[k] = tmp;
								}
							}
						}

						// printing cards
						printf("%s\n\nEnumerating cards ... \n%s", BOLDYELLOW, RESET);
						for(c=0;c<13;c++) {
							printf("%d.\t%s %s\n", c+1, suit[(cards[c]-1)/13], valu[(cards[c]-1)%13]  );
						}

						break;

					// 4 : round information
					case 4 :
						memset(buffer, '\0', sizeof buffer);
						while( ( i = recv(clientSocket, buffer, 1024, 0) )  < 0);
						if(!strcmp(buffer,""))
						{
							printf("%s\n\nERROR : communication problem , server closing the room.\n%s",BOLDRED, RESET);
							flag = 1;
							break;
						}
						ROUND = atoi(buffer);
						rIter = -1;
						printf("%s\n\nBeginning ROUND %d of LEVEL %d ... \n%s", BOLDYELLOW, ROUND, LEVEL, RESET);
						break;

					// 5 : signal to prompt user to play the card
					case 5 :
						
						while(1) {
							printf("%s\n\nPlay your cards out of these...\n%s",BOLDGREEN, RESET);
							for(c=0;c<13;c++) {
								if(cards[c] != -1)
									printf("%d.\t%s %s\n", c+1, suit[(cards[c]-1)/13], valu[(cards[c]-1)%13]  );
							}
							printf("\n\nYour choice : ");

							scanf("%d", &played);

							if(cards[played-1] != -1) {
								if(rIter != -1) { 
									playingSuit = (roundCards[0]-1)/13;
									playingCardPresent = 0;
									for(j=0;j<13;j++) {
										if(cards[j]-1 >= playingSuit*13 && cards[j]-1 < (playingSuit+1)*13 )
											playingCardPresent = 1; 
									}
									// printf("%d : %d\n", playingSuit*13, (playingSuit+1)*13);
									// printf("%d\n", cards[played-1] - 1);
									if(playingCardPresent && !(cards[played-1]-1 >= playingSuit*13 && cards[played-1]-1 < (playingSuit+1)*13 ) ) {
										printf("%s\n\nERROR : play the card from running suit\n%s",BOLDRED, RESET);
										continue;
									}
								}
								else {
									// rIter == -1 (first play to play)
									zeroScore = 1;

									for(c=0;c<4;c++) {
										if(lScore[c] > 0)
											zeroScore = 0;
									}

									if(zeroScore == 1 && LEVEL == 3 && cards[played-1] >= 27 && cards[played-1] <= 39 ) {
										
										printf("%s\n\nERROR : cannot play hearts as first card\n%s",BOLDRED, RESET);
										continue;	
									}
								}
									
								memset(buffer, '\0', sizeof buffer);
								sprintf(buffer, "%d", cards[played-1]);
								roundCards[++rIter] = cards[played-1];
								send(clientSocket, buffer, 1024, 0);
								// printf("sending this : %s\n", buffer);
								cards[played-1] = -1;
								break;
								
							}
							else {
								printf("%s\n\nERROR : card has already been played.\n%s",BOLDRED, RESET);
								continue;
							}


						}

						break;

					// 6 : waiting for other player to play their turn
					case 6 : 
						while( ( i = recv(clientSocket, buffer, 1024, 0) )  < 0);
						if(!strcmp(buffer,""))
						{
							printf("%s\n\nERROR : communication problem , server closing the room.\n%s",BOLDRED, RESET);
							flag = 1;
							break;
						}
						// printf("is wale buffer mein : %s\n",buffer);
						strcpy(CPLAYER, buffer);
						printf("%s\n\nWaiting for %s to play ... \n%s",BOLDYELLOW, CPLAYER, RESET);	
						rIter++;
						break;

					// 7 : notify user about the card played by the client
					case 7 : 
						while( ( i = recv(clientSocket, buffer, 1024, 0) )  < 0);
						if(!strcmp(buffer,""))
						{
							printf("%s\n\nERROR : communication problem , server closing the room.\n%s",BOLDRED, RESET);
							flag = 1;
							break;
						}
						tmp = atoi(buffer);
						roundCards[rIter] = tmp;
						printf("%s\n\n %s played  ... %s %s \n%s",BOLDCYAN, CPLAYER, suit[(tmp-1)/13], valu[(tmp-1)%13], RESET );	
						break;

					// score update after the round
					case 8 : 
						memset(buffer, '\0', sizeof buffer);
						while( ( i = recv(clientSocket, buffer, 1024, 0) )  < 0);
						if(!strcmp(buffer,""))
						{
							printf("%s\n\nERROR : communication problem , server closing the room.\n%s",BOLDRED, RESET);
							flag = 1;
							break;
						}
						// printf("scores of this round are: %s\n", buffer);
						tokens = strtok(buffer, ",");
						
						for(c=0;c<4;c++) {
							int temp = atoi(tokens);
							lScore[c] += temp;
							tokens = strtok(NULL, ",");
						}

						printf("%s\n\nScores at the end of Round %d are : ......\n%s",BOLDMAGENTA, ROUND, RESET);

						printf("--------------------------------------------------------------------------------\n");
						printf("%s\t\t%s\t\t%s\t\t%s\n", NAME[0], NAME[1], NAME[2], NAME[3]);
						printf("%d\t\t%d\t\t%d\t\t%d\n", lScore[0], lScore[1], lScore[2], lScore[3]);
						printf("--------------------------------------------------------------------------------\n");

						// for(c=0;c<4;c++)
						// {
						// 	printf("Player %s score : %d\n",NAME[c] , lScore[c]);
						// }

						break;

					case 9 :

						printf("%s\n\nLEVEL 1 ends. .\n%s",BOLDYELLOW, RESET);
						// initiating level scores array
						for(c=0;c<4;c++) {
							gScore[c] += lScore[c];
							lScore[c] = 0;
						}
						
						// global scores after level 1 

						sleep(1);
						printf("%s\n\nDealing for level 2 ... receiving set of cards .\n%s",BOLDBLUE, RESET);
						LEVEL = 2;

						
						// receiving cards information
						memset(buffer, '\0', sizeof buffer);
						while( ( i = recv(clientSocket, buffer, 1024, 0) )  < 0);
						//printf("cards received (%d bytes) : \"%s\"\n", i, buffer);
						
						// decoding cards
						tokens = strtok(buffer, ",");
						for(c=0;c<13;c++) {
							cards[c] = atoi(tokens);
							tokens = strtok(NULL, ",");
						}

						for(j=0;j<13;j++) {
							for(k=0;k<12;k++) {
								if(cards[k+1] < cards[k]) {
									tmp = cards[k+1];
									cards[k+1] = cards[k];
									cards[k] = tmp;
								}
							}
						}

						// printing cards
						printf("%s\n\nenumerating cards ... \n%s", BOLDGREEN, RESET);
						for(c=0;c<13;c++) {
							printf("%d.\t%s %s\n", c+1, suit[(cards[c]-1)/13], valu[(cards[c]-1)%13]  );
						}

						break;

					case 10 : 

						printf("%s\n\nYou have to select any 2 cards to pass to your adjacent player. \n\n%s", BOLDMAGENTA, RESET);


						break;

					case 11 :

						printf("Enter 2 cards : \n");
						
						scanf("%d", &p1);
						while(!(p1 >= 1 && p1 <= 13)) {
							printf("%s\n\nERROR: Invalid cards... try again\n%s",BOLDRED,RESET);
							scanf("%d", &p1);
						}
						scanf("%d", &p2);
						while(!(p2 >= 1 && p2 <= 13) || p1==p2 ) {
							printf("%d\n",p2 );
							printf("%s\n\nERROR:Invalid cards or avoid selecting repetative card... try again\n%s",BOLDRED, RESET);
							scanf("%d", &p2);
						}


						memset(buffer, '\0', 1024);
						sprintf(buffer,"%d,%d", cards[p1-1], cards[p2-1]);

						send(clientSocket, buffer, 1024, 0);

						printf("sending 2 cards\n");
						printf("%s %s\n", suit[(cards[p1-1]-1)/13], valu[(cards[p1-1]-1)%13]  ) ;
						printf("%s %s\n", suit[(cards[p2-1]-1)/13], valu[(cards[p2-1]-1)%13]  ) ;

						printf("%s\n\nPlease wait while other player passes the card.\n%s", BOLDYELLOW,  RESET);

						break;

					case 12 :

						printf("You have received 2 cards\n");

						while( 	( i = recv(clientSocket, buffer, 1024, 0) )  < 0);
						if(!strcmp(buffer,""))
						{
							printf("%s\n\nERROR : communication problem , server closing the room.\n%s",BOLDRED, RESET);
							flag = 1;
							break;
						}
						tokens = strtok(buffer, ",");
						cards[p1-1] = atoi(tokens);
						tokens = strtok(NULL, ",");
						cards[p2-1] = atoi(tokens);
						
						printf("%s %s\n", suit[(cards[p1-1]-1)/13], valu[(cards[p1-1]-1)%13]  ) ;
						printf("%s %s\n", suit[(cards[p2-1]-1)/13], valu[(cards[p2-1]-1)%13]  ) ;

						break;

					case 13 :

						printf("%s\n\nLEVEL 2 ends. .\n%s", BOLDYELLOW, RESET);
						// initiating level scores array and 
						for(c=0;c<4;c++) {
							gScore[c] += lScore[c];
							lScore[c] = 0;
						}

						// print global scores !!!
						
						sleep(1);
						printf("%s\n\nDealing for level 3 ... receiving set of cards .\n%s",  BOLDBLUE, RESET);
						LEVEL = 3;

						
						// receiving cards information
						memset(buffer, '\0', sizeof buffer);
						while( ( i = recv(clientSocket, buffer, 1024, 0) )  < 0);
						if(!strcmp(buffer,""))
						{
							printf("%s\n\nERROR : communication problem , server closing the room.\n%s",BOLDRED, RESET);
							flag = 1;
							break;
						}
						printf("cards received (%d bytes) : \"%s\"\n", i, buffer);
						
						// decoding cards
						tokens = strtok(buffer, ",");
						for(c=0;c<13;c++) {
							cards[c] = atoi(tokens);
							tokens = strtok(NULL, ",");
						}

						for(j=0;j<13;j++) {
							for(k=0;k<12;k++) {
								if(cards[k+1] < cards[k]) {
									tmp = cards[k+1];
									cards[k+1] = cards[k];
									cards[k] = tmp;
								}
							}
						}

						// printing cards
						printf("enumerating cards ... \n");
						for(c=0;c<13;c++) {
							printf("%d.\t%s %s\n", c+1, suit[(cards[c]-1)/13], valu[(cards[c]-1)%13]  );
						}

						break;

					case 14 :
						printf("%s\n\n LEVEL 3 ends .. \n%s",BOLDYELLOW, RESET);
						printf("%s\n\n ...........GAME ENDS.........%s", BOLDRED,  RESET);

						highestIndex = 0;

						for(j=1;j<=3;j++) {
							if(lScore[highestIndex] < lScore[j])
								highestIndex = j;
						}

						printf("%s\n\n Winner for this game is: %s\n%s", BOLDMAGENTA, NAME[highestIndex], RESET);

						flag = 1;
						break;

					default : 
						printf("invalid signals\n");
						printf("%s", buffer);
						break;
				}
			}
		}
		else {
			break;
		}
	}

	// printf("client :: %d\n" , getpid() );

	return 0;

}

