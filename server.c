#include <stdio.h>
#include <math.h>
// socket(), connect(), send(), recv()
#include <sys/socket.h>
// socketaddr_in and in_addr structures
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
// inet_addr structure
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>


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




/*
Action Sequences
1. Connection established 
2. Room closed (common) , game start , alloting names
3. Cards Distribution , level 1 begins
4. round begin , round number 
5. Sent to the player which has to play next turn. 
6. Asks other players to wait.
7. Card info which the other player played.
*/

int pid,first;
int serverSocket;
int newSocket[4];
struct sockaddr_in serverAddr;
struct sockaddr_in serverStorage;
socklen_t addr_size;
char buffer[1024];

char output[13];
char* names[] = {"Akshit","Pranjal","Rishabh","Neel"};
char nametoSend[1024];
int deal1[13];
int deal2[13];
int deal3[13];
int deal4[13];
int deal[52];

int club_plyr = -1;
int diamond_plyr = -1;
int hearts_plyr = -1;
int spades_plyr = -2;


int points[13][4] ;

char str[1024];
int c[52];

void convert(int arr[]) {
	memset(str, '\0', 1024);
	char str1[20];
	sprintf(str,"%d,%d,%d,%d,%d,%d,%d,%d",arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],arr[6],arr[7]);
	sprintf(str1,",%d,%d,%d,%d,%d",arr[8],arr[9],arr[10],arr[11],arr[12]);
	strcat(str,str1);
}

int search(int arr[] , int temp)
{
	int i;
	for(i=0;i<13;i++)
	{
		if(arr[i]==temp)
			return i;
	}
	return -1;
}

int search_cards(int temp , int key)
{
	int res =-1;;
	if(key==0)
	{
		res = search(deal1, temp);
		printf("index is %d\n",res );
	}
	else if(key==1)
	{
		res = search(deal2,temp);
		printf("index is %d\n",res );

	}
	else if(key==2)
	{
		res = search(deal3, temp);
		printf("index is %d\n",res );
		
	}
	else if(key==3)
	{
		res = search(deal4 , temp);
		printf("index is %d\n",res );
		
	}

	return res;

}


void swap_cards()
{
	char swap[4][1024];
	int index[4][2];
	char* token;
	memset(buffer,'\0',sizeof buffer);
	strcpy(buffer,"10");
	send(newSocket[0],buffer,1024,0);
	send(newSocket[1],buffer,1024,0);
	send(newSocket[2],buffer,1024,0);
	send(newSocket[3],buffer,1024,0);
	int a;
	char s[2] = ",";


// collecting cards which has to be swapped from each player 
	for(a=0;a<4;a++)
	{
		memset(buffer,'\0',sizeof buffer);
		strcpy(buffer,"11");
		sleep(1);
		send(newSocket[a],buffer,1024,0);
		memset(buffer,'\0',sizeof buffer);
		sleep(1);
		recv(newSocket[a],buffer,1024,0);

		strcpy(swap[a],buffer);

		token =  strtok(buffer , s);
		int k=0;
		while(token!= NULL)
		{
			int temp = atoi(token);
			printf("number is %d\n",temp);
			int ans = search_cards(temp,a);
			index[a][k] = ans;
			k++;
			token = strtok(NULL ,s);
		}

	}

	

// sending cards to each player after swapping

	time_t t;
	srand((unsigned) time(&t));


	int r = rand() % 3;

	for(a=0;a<4;a++)
	{
		memset(buffer,'\0',sizeof buffer);
		strcpy(buffer,"12");
		sleep(1);
		send(newSocket[a],buffer,1024,0);

		memset(buffer,'\0',sizeof buffer);
		sleep(1);
		int p = (a+r)%4;
		strcpy(buffer,swap[p]);
		send(newSocket[a],buffer,1024,0);

		token = strtok(buffer , s);
		int k=0;
		while(token!= NULL)
		{
			int temp = atoi(token);
			int deal_index = index[a][k];
			if(a==0)
				deal1[deal_index] = temp;
			else if(a==1)
				deal2[deal_index] = temp;
			else if(a==2)
				deal3[deal_index] = temp;
			else if(a==3)
				deal4[deal_index] = temp;
			


			// index[a][k] = ans;
			k++;
			token = strtok(NULL ,s);
		}

	}

		// to print the cards
			int i;
			for(i=0;i<13;i++)
				printf("%d ",deal1[i]);
			printf("\n");

			for(i=0;i<13;i++)
				printf("%d ",deal2[i]);
			printf("\n");

			for(i=0;i<13;i++)
				printf("%d ",deal3[i]);
			printf("\n");

			for(i=0;i<13;i++)
				printf("%d ",deal4[i]);
			printf("\n");




}


void random_generate(int lvl) {

	time_t t;
	srand((unsigned) time(&t));

	int i, j, k=0, temp;

	for(i=0;i<52;i++)
		c[i] = i+1;

	for(j=39;j>=0;j--) {
		int t = j+12;
		int r = rand() % t;

		temp = c[r];
		c[r] = c[j+12];
		c[j+12]= temp;

		deal[k] = c[j+12];
		k++;
	}

	for(j=0;j<13;j++) {
		deal1[j] = deal[j];
		if(deal1[j] == 1) {
			first = 0;
		}
		deal2[j] = deal[j+13];
		if(deal2[j] == 1) {
			first = 1;
		}
		deal3[j] = deal[j+26];
		if(deal3[j] == 1) {
			first = 2;
		}
		deal4[j]= c[j];
		if(deal4[j] == 1) {
			first =3;
		}
	}

	sleep(1);

	memset(buffer, '\0', sizeof buffer);
	if(lvl == 1)
	{
		strcpy(buffer,"3");
	}
	else if(lvl == 2 )
	{
		strcpy(buffer,"9");
	}
	else if(lvl == 3 )
	{
		strcpy(buffer,"13");
	}
	send(newSocket[0],buffer,1024,0);
	send(newSocket[1],buffer,1024,0);
	send(newSocket[2],buffer,1024,0);
	send(newSocket[3],buffer,1024,0);

	sleep(1);

	convert(deal1);
	puts(str);
	send(newSocket[0], str, 1024, 0);

	convert(deal2);
	puts(str);
	send(newSocket[1], str, 1024, 0);

	convert(deal3);
	puts(str);
	send(newSocket[2], str, 1024, 0);

	convert(deal4);
	puts(str);
	send(newSocket[3], str, 1024, 0);

	if(lvl == 2 || lvl == 3)
	{
		swap_cards();
	}

}


void game_room() {
	// Room information for each client
	
	sleep(1);

	memset (buffer ,'\0' , sizeof buffer);
	strcpy(buffer,"2");	
	send(newSocket[0], buffer, 1024, 0);
	send(newSocket[1], buffer, 1024, 0);
	send(newSocket[2], buffer, 1024, 0);
	send(newSocket[3], buffer, 1024, 0);

	sleep(1);

	memset (buffer ,'\0' , sizeof buffer);
	sprintf(buffer,"%s,%s,%s,%s",names[0],names[1],names[2],names[3]);
	send(newSocket[0], buffer, 1024, 0);
	send(newSocket[1], buffer, 1024, 0);
	send(newSocket[2], buffer, 1024, 0);
	send(newSocket[3], buffer, 1024, 0);
	
	sleep(1);

	int a;
	for(a=0;a<4;a++) {
		memset (buffer ,'\0' , sizeof buffer);
		sprintf(buffer,"%d",a);
		send(newSocket[a], buffer, 1024, 0);
	}

}

// this is to clalculate score for level 1 and 2
void calculateWinner(int playedCards[],int m){
	int deck = (playedCards[first]-1)/13;
	int winner = first;
	int spadeQueen = 0;
	int clubqueen = 0;
	int noHearts = 0;
	int low = 13*deck+1;
	int high = 13*(deck+1);
	int i;

	for(i=0;i<4;i++){
		
		if(playedCards[i]>= 27 && playedCards[i] <= 39)
			noHearts++;

		if(playedCards[i]>=low && playedCards[i] <= high){
			if(playedCards[winner] < playedCards[i])
				winner = i;
		}

		if(playedCards[i] == 50)
		{
			spadeQueen = 13;
		}

	}

	first = winner;
	points[m][first] += (spadeQueen + noHearts);
	//return (spadeQueen + noHearts);

}



// this is to claculate score for level 3
void calculateWinner_3(int playedCards[],int m){
	int deck = (playedCards[first]-1)/13;
	int winner = first;
	int spadeQueen = 0;
	int clubqueen = 0;
	int noHearts = 0;
	int low = 13*deck+1;
	int high = 13*(deck+1);
	int i;

	for(i=0;i<4;i++){
		
		if(playedCards[i]>= 27 && playedCards[i] <= 39)
			noHearts++;

		if(playedCards[i]>=low && playedCards[i] <= high){
			if(playedCards[winner] < playedCards[i])
				winner = i;
		}

		if(playedCards[i] == 50)
		{
			spadeQueen = 13;
		}

		// clubs queen will get you -10 points
		if(playedCards[i] == 11)
		{
			clubqueen = -10;
		}

	}

	first = winner;

	for(i=0;i<4;i++)
	{
	
		if(playedCards[i] == 50)
		{
			spades_plyr = first;
		}
		// clubs queen will get you -10 points
		if(playedCards[i] == 11)
		{
			club_plyr = first;
		}
		
		if(playedCards[i] == 37)
		{
			hearts_plyr = first;
		}
	}
	printf("spades_plyr is : %d\n",spades_plyr);
	printf("hearts_plyr is : %d\n",hearts_plyr);

	if((spades_plyr == hearts_plyr))
	{
		points[m][first] += (spadeQueen + noHearts + clubqueen -14);
		printf("yaha change hue scores-------%d",points[m][first]);
		hearts_plyr = -1;
		spades_plyr = -2;
	}
	else
		points[m][first] += (spadeQueen + noHearts + clubqueen);


}
// to send the game end signal to all the players

void game_end()
{
	memset(buffer, -1, sizeof(buffer));


	strcpy(buffer,"14");
	send(newSocket[0], buffer, 1024, 0);
	send(newSocket[1], buffer, 1024, 0);
	send(newSocket[2], buffer, 1024, 0);
	send(newSocket[3], buffer, 1024, 0);
}

void  game_logic(int lvl) {

	int i, a, b, ini, in;
	char cardbatao[1024];

	for(ini = 0;ini<13;ini++) {
		for(in = 0;in < 4;in++)
			points[ini][in] = 0;
	}
	

	for(i=1;i<=13;i++) {
		int playedCards[4] = {0};
		
		//Round Begins
		sleep(1);
		memset(buffer, -1, sizeof(buffer));


		strcpy(buffer,"4");
		send(newSocket[0], buffer, 1024, 0);
		send(newSocket[1], buffer, 1024, 0);
		send(newSocket[2], buffer, 1024, 0);
		send(newSocket[3], buffer, 1024, 0);

		//Sending round Number:
		sleep(1);
		
		memset(buffer, -1, sizeof(buffer));
		sprintf(buffer,"%d",i);
		
		send(newSocket[0], buffer, 1024, 0);
		send(newSocket[1], buffer, 1024, 0);
		send(newSocket[2], buffer, 1024, 0);
		send(newSocket[3], buffer, 1024, 0);



		// Taking card input from other players and similar things.
		int j,k;
		for(j=0;j<=3;j++) {
			k = (first+j)%4;
			
			// Asking kth player to play his/her turn.
			sleep(1);

			memset(buffer,-1,sizeof(buffer));
			strcpy(buffer,"5");
			send(newSocket[k],buffer,1024,0);
						
			//Asking others to wait.	
			memset(buffer,-1,sizeof(buffer));
			strcpy(buffer,"6");

			for(a=0;a<4;a++) {
				if(a == k) 
					continue;
				send(newSocket[a], buffer, 1024, 0);
			}

			sleep(1);
			memset(nametoSend,-1,sizeof(nametoSend));
			strcpy(nametoSend,names[k]);

			for(a=0;a<4;a++) {
				if(a == k)
					continue;
				send(newSocket[a], nametoSend, 1024, 0);
			}
			
			//Taking input kth player.		
			memset(buffer,-1,sizeof(buffer));
			recv(newSocket[k],buffer,1024,0);
			playedCards[k] = atoi(buffer);

			//Telling other players of card inputted by kth player
			sleep(1);
			memset(cardbatao,-1,sizeof(cardbatao));
			strcpy(cardbatao,"7");

			for(a=0;a<4;a++) {
				if(a == k) 
					continue;
				send(newSocket[a], cardbatao, 1024, 0);
			}

			sleep(1);

			for(a=0;a<4;a++) {
				if(a == k) 
					continue;
				send(newSocket[a], buffer, 1024, 0);
			}

		}


		for(a=0;a<4;a++) {
			if(a==0){
				for(j=0;j<13;j++)
					if(deal1[j] == playedCards[a])
						deal1[j] = -1;
			}
			else if(a == 1){
				for(j=0;j<13;j++)
					if(deal2[j] == playedCards[a])
						deal2[j] = -1;
			}
			else if(a == 2){
				for(j=0;j<13;j++)
					if(deal3[j] == playedCards[a])
						deal3[j] = -1;
			}
			else if(a == 3){
				for(j=0;j<13;j++)
					if(deal4[j] == playedCards[a])
						deal4[j] = -1;
			}
		}
		if((lvl == 1) || (lvl == 2))
			calculateWinner(playedCards,i);
		if(lvl == 3)
			calculateWinner_3(playedCards,i);
		
		sleep(1);
		memset(buffer,-1,sizeof(buffer));
		strcpy(buffer,"8");
		send(newSocket[0], buffer, 1024, 0);
		send(newSocket[1], buffer, 1024, 0);
		send(newSocket[2], buffer, 1024, 0);
		send(newSocket[3], buffer, 1024, 0);
		
		sleep(1);
		memset(buffer,-1,sizeof(buffer));
		sprintf(buffer,"%d,%d,%d,%d",points[i][0],points[i][1],points[i][2],points[i][3]);
		send(newSocket[0], buffer, 1024, 0);
		send(newSocket[1], buffer, 1024, 0);
		send(newSocket[2], buffer, 1024, 0);
		send(newSocket[3], buffer, 1024, 0);
		
	}
}



int main() {

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	// server address definition
	serverAddr.sin_family = AF_INET;

	int port = 0;
	char sAdr[20];

	printf("%s-------------------------------------------- HEARTS v.1 (SERVER) --------------------------------------------%s\n", BOLDRED, RESET);

	// get the server details from the user.
	printf("Enter the server details :\n");
	printf("server address \t :\t " );
	memset(sAdr, 0, 20);
	scanf("%s", sAdr);
	printf("server port \t :\t " );
	scanf("%d", &port);

	serverAddr.sin_port  = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(sAdr);
	

	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	bind(serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

	int i=0;

	for(i=1;listen(serverSocket, 4) == 0;i++) {
		
		printf("Listening :: %d\n", listen(serverSocket, 4));
		
		addr_size = sizeof serverStorage;
		
		newSocket[(i-1)%4] = accept(serverSocket, (struct sockaddr*) &serverStorage, &addr_size);
		
		strcpy(buffer, "1");
		send(newSocket[(i-1)%4], buffer, 1024, 0);

		if(i%4 == 0) {

			pid = fork();
			
			if(pid < 0) {
				printf("Error creating child process \n");
			}
			if(pid == 0) {
				//child 
				printf("server child :: %d\n" , getpid());

				close(serverSocket);

				printf("new socket descriptor in child process: %d, %d, %d, %d\n", newSocket[0], newSocket[1], newSocket[2], newSocket[3]);

				game_room();
				// random_generate(1);
				// game_logic(1);
				random_generate(2);
				game_logic(3);
				random_generate(3);
				game_logic(3);
				game_end();
				
			
			}
			else {
				//parent
				close(newSocket[0]); 
				close(newSocket[1]); 
				close(newSocket[2]); 
				close(newSocket[3]); 
			}
		}
		printf("got %dth child ... \n", (i-1)%4 );

	}
	
	return 0;

}
