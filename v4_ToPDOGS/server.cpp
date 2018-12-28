#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <windows.h>
#include <winsock.h>

#include <string>
#include <cstring>
#include <ctime>
#include <iomanip>
using namespace std;

const int myself = 1 ;
const int opponent = 0 ;

class Deck{
	private:
		int count ;				//用幾副撲克牌 
		int point[13] = {0} ;	//各點數張數 
		int total = 0 ;			//牌組剩餘總張數 
		void draw(int num){
			point[num-1] -- ;
			total-- ;
		}
	public:
		Deck(){
			this->count = 0 ;
		}
		Deck(int count){
			this->count = count ;
			for(int i = 0 ; i < 13 ; i++){
				this->point[i] = this->count * 4 ;
			}
			this->total = this->count * 52 ;
		}
		int draw(){
			int num = rand() % this->total + 1 ;
			for(int i = 0 ; i < 13 ; i++){
				num -= point[i] ;
				if(num <= 0){
					this->draw(i+1) ;
					return i+1 ;
				}
			}
		}
		void print(){
			cout << "目前牌堆狀況：\n" ; 
			
			for(int j = 0 ; j < point[0] ; j++){
				cout << setw(3) ;
				cout << "A " ;
			}
			cout << endl ;
			for(int i = 1 ; i < 10 ; i++){
				for(int j = 0 ; j < point[i] ; j++){
					cout << setw(2) ;
					cout << i+1 << ' ' ;
				}
				cout << endl ;
			}
			for(int i = 10 ; i < 13 ; i++){
				for(int j = 0 ; j < point[i] ; j++){
					cout << setw(3) ;
					if(i == 10) cout << "J " ;
					else if(i == 11) cout << "Q " ;
					else if(i == 12) cout << "K " ;
				}
				cout << endl ;
			}
		}
};

class Player{
	private:
		string name ;
		int id ;
		int holding[5] = {0} ;			//手牌，最多五張(int型態 數字1~13) 
		int holdingCnt = 0 ;			//目前手牌數
	public:
		Player(){
			name = "" ;
			id = 0 ;
		}
		Player(string name){
			this->name = name ;
		}
		void set(string name , int id){
			this->setName(name) ;
			this->setID(id) ;
		}
		void setName(string name){
			this->name = name ;
		}
		void setID(int id){
			this->id = id ;
		}
		void cleargame(){
			for(int i=0 ; i<5 ; i++) holding[i]=0;
			holdingCnt=0; 
		}
		int get(Deck& dek){
			this->holding[holdingCnt++] = dek.draw() ;
			return this->holding[holdingCnt-1] ;
		}
		int Sum(){					//為考慮A的狀況，用函數而不用變數存sum 
			int sum = 0 ;
			int AceCount = 0 ;
			for(int i = 0 ; i < this->holdingCnt ; i++){
				if(this->holding[i] == 1){
					AceCount++ ;
				}
				else if(this->holding[i] > 10){
					sum += 10 ;
				}
				else{
					sum += this->holding[i] ;
				}
			}
			sum += AceCount * 11 ;
			while(sum > 21 && AceCount > 0){
				sum -= 10 ;
				AceCount -- ;
			}
			return sum ;
		}
		bool passFive(){
			return (this->holdingCnt == 5 && this->Sum() <= 21) ;
		}
		int cardCnt(){
			return holdingCnt ;
		}
		string send_(int status = myself){
			string s = "" ;
			
			for(int i = 0 ; i < this->holdingCnt ; i++){
				if(holding[i] == 1){s += "A " ;}
				else if(holding[i] == 11){s += "J " ;}
				else if(holding[i] == 12){s += "Q " ;}
				else if(holding[i] == 13){s += "K " ;}
				else if(holding[i] == 10){s += "10 " ;}
				else{
					s += holding[i] + 48 ;
					s += " " ;
				}
			}
			if(this->Sum() > 21) s += "(爆)" ;
			else if(this->passFive()) s += "(過五關!)" ;
			
			if(status == myself){
				if(this->Sum() == 21) s += "(blackjack!)" ;
			}
			else{		//status == opponent
				if(this->Sum() <= 21){
					if(this->holding[0] == 10){
						for(int i = 0 ; i < s.length()-1 ; i++){
							s[i] = s[i+1] ;
						}
					}
					s[0] = '?' ;
				}
			}
			
			s += "\n" ;
			
			return s ;
		}
		void print(){
			if(this->holdingCnt == 0){
				cout << "0\n" ;
				return ;
			}
			for(int i = 0 ; i < this->holdingCnt ; i++){
				cout << this->holding[i] << ' ' ;
			}
			cout << endl ;
		}
		int ID(){
			return id ;
		}
};

int judge(Player p1 , Player p2){
	if(p1.Sum() > 21 && p2.Sum() > 21){
		return -8763 ;
	}
	else if( (p1.Sum()>21 && (p2.Sum()<=21)) ) return p2.ID();
	else if( (p2.Sum()>21 && (p2.Sum()<=21)) ) return p1.ID();
	
	else if(p1.passFive() && p2.passFive()){
		if(p1.Sum() > p2.Sum()) return p1.ID() ;
		else if(p2.Sum() > p1.Sum()) return p2.ID() ;
		else return -87 ;
	}
	else if(p1.passFive()) return p1.ID() ;
	else if(p2.passFive()) return p2.ID() ;
	
	else if(p1.Sum() == p2.Sum()){
		if(p1.cardCnt() > p2.cardCnt()) return p1.ID() ;
		else if(p2.cardCnt() > p1.cardCnt()) return p2.ID() ;
		else return -87 ;
	}
	else if( (p1.Sum() > p2.Sum()) && p1.Sum()<=21 ) return p1.ID() ;
	else if( (p2.Sum() > p1.Sum()) && p2.Sum()<=21 ) return p2.ID() ;
}

Deck deck(4) ;
Player p1 , p2 ;
int winnerID ;

int countPlayer=0;
int flag1=0;	//flag for player1  if(flag1==1) ready to start game
int flag2=0;

int now = 1 ;

// our thread for recving commands
DWORD WINAPI receive_cmds(LPVOID lpParam){
	//printf("thread created\r\n");
	cout << "thread created : " ;
	cout << "thread" << countPlayer << endl ;
   
	SOCKET current_client = (SOCKET)lpParam;	//set our socket to the socket passed in as a parameter

	if(countPlayer==1){
		srand(time(NULL)) ;
	//	Player p1;
		
		char buf[100];		// buffer to hold our received data
	    char sendData[100];	// buffer to hold our sent data
	    
		int res;	// for error checking
	   
		//send to client
		sendData[0]='1';
	   	send(current_client , sendData , sizeof(sendData) , 0); 
	   
	   	//receive from client
		res = recv(current_client , buf , sizeof(buf) , 0); // recv cmds

	   	cout << "player1's name is "<< buf << endl ;
	    
		p1.set(buf , 1);
	   
		flag1 = 1 ;
		
		strcpy(sendData , "Waiting...\n") ;
		send(current_client , sendData , sizeof(sendData), 0);
		
		playp1:
		while( (flag1 + flag2) != 2 ) ;
		
		cout << "player1 start play game\n" ;
		
		strcpy(sendData , "connection success\n") ;
		send(current_client , sendData , sizeof(sendData), 0);
	   
		p1.get(deck);				//暗牌 
		p1.get(deck);
		
		string s = p1.send_() ;
		char msg[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg[i] = s[i] ;
		}
		msg[s.length()] = '\0' ;
		
		send(current_client , msg , sizeof(msg) , 0); 		//自己的初始手牌
		
		Sleep(500) ;
		
		s = p2.send_(opponent) ;
		char msg2[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg2[i] = s[i] ;
		}
		msg2[s.length()] = '\0' ;
		
		send(current_client , msg2 , sizeof(msg2) , 0);		//對手的初始手牌 
		
		Sleep(500) ;
		while(now == 1){
			
			p1.print() ;
			p2.print() ;
			
			if(p1.passFive() || p1.Sum() >= 21){
				strcpy(sendData , "bye") ;
				send(current_client , sendData , sizeof(sendData) , 0);
				now = 2 ;
				break ;
			}
			
			strcpy(sendData , "繼續補牌?(y/n)\n") ;
			send(current_client , sendData , sizeof(sendData) , 0); 
			
			res = recv(current_client , buf , sizeof(buf) , 0); // recv cmds
			
			if(buf[0] == 'y'){
				p1.get(deck) ;
				string s = p1.send_() ;
				char msg[s.length() + 1] ;
				for(int i = 0 ; i < s.length() ; i++){
					msg[i] = s[i] ;
				}
				msg[s.length()] = '\0' ;
		
				send(current_client , msg , sizeof(msg) , 0);
			}
			else if(buf[0] == 'n'){
				char msg[] = "waiting for result...\n" ;
				send(current_client , msg , sizeof(msg) , 0);
				now = 2 ;
			}
		}
		
		while(now != 1) ;
		
		
		
		// 
		s = p1.send_() ;
		char msg3[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg3[i] = s[i] ;
		}
		msg3[s.length()] = '\0' ;
		
		send(current_client , msg3 , sizeof(msg3) , 0);		//p1手牌 
		
		s = p2.send_() ;
		char msg4[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg4[i] = s[i] ;
		}
		msg4[s.length()] = '\0' ;
		
		send(current_client , msg4 , sizeof(msg4) , 0);		//p2手牌 
		// 
		
		
		winnerID = judge(p1 , p2) ;
		strcpy(sendData, "恭喜您獲勝!\n");
		cout<<"winner:"<<winnerID<<endl; 
		if(winnerID == p1.ID()){
			strcpy(sendData , "恭喜您獲勝!\n") ;
		}
		else if(winnerID == p2.ID()){
			strcpy(sendData , "可惜，您輸了!\n") ;
		}
		else if(winnerID == -87){
			strcpy(sendData , "打成平手!\n") ;
		}
		else if(winnerID == -8763){
			strcpy(sendData , "可惜，您們二位點數都爆了!\n") ;
		}
		
		send(current_client , sendData , sizeof(sendData) , 0);
		now = 2 ;
		
		flag1=0;
		
	    // our recv loop
	    while(true) {
			res = recv(current_client,buf,sizeof(buf),0); // recv cmds
			
			Sleep(10);
			
			if(res == 0){
				MessageBox(0,"error","error",MB_OK);
				closesocket(current_client);
				ExitThread(0);
			}
			if(strstr(buf,"play")){ // greet this user
				printf("\nrecived play cmd\n");
				flag1 = 1;
				p1.cleargame();
				strcpy(sendData,"");
				strcpy(buf,"");
				goto playp1;
			}
			else if(strstr(buf,"bye")){ // dissconnected this user
				printf("\nrecived bye cmd\n");
				strcpy(sendData,"See you again!\n");
				Sleep(10);
				send(current_client,sendData,sizeof(sendData),0);

				// close the socket associted with this client and end this thread
				closesocket(current_client);
				ExitThread(0);
			}
			else
			{
				cout<< buf <<endl;
				strcpy(sendData,"respond from server\n");
				Sleep(10);
				send(current_client,sendData,sizeof(sendData),0);
			}
	      
			// clear buffers
			strcpy(sendData,"");
			strcpy(buf,"");
	    }

    }
    else if(countPlayer==2){
    	srand(time(NULL)) ;
    //	Player p2;
	  	// buffer to hold our recived data
	    char buf[100];
	    // buffer to hold our sent data
	    char sendData[100];
	    // for error checking 
	    int res;
	   
	   //send to client
	   	sendData[0]='2';
	   	send(current_client,sendData,sizeof(sendData),0); 
	   
	   	//receive from client
		res = recv(current_client,buf,sizeof(buf),0); // recv cmds
		string name;
		name=buf;
	   	cout<<"player2's name is "<< name <<endl;
	   	p2.set(name,2);
	   
	   flag2 = 1 ;
	   
		strcpy(sendData , "Waiting...\n") ;
		send(current_client , sendData , sizeof(sendData) , 0);	   
	   
	   playp2:
	   while( (flag1+flag2)!=2 ) ;
	   cout<<"player2 start play game"<<endl;
	   
		strcpy(sendData , "connection success\n") ;
		send(current_client , sendData , sizeof(sendData) , 0);
	   
	   
	   
	   
	   //打在這裡~ 
	   
//	   send(current_client,"play~~~\n",sizeof(sendData),0); 
	   
	   p2.get(deck);				//暗牌 
	   p2.get(deck);
//	   p2.print() ;
	   
		string s = p2.send_() ;
		char msg[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg[i] = s[i] ;
		}
		msg[s.length()] = '\0' ;
		
		send(current_client , msg , sizeof(msg) , 0); 		//自己的起始手牌 
		
		
		
		
	/*	strcpy(sendData , "Waiting...\n") ;
		send(current_client , sendData , sizeof(sendData), 0);*/
		
		while(now != 2) ;
		
		s = p1.send_(opponent) ;
		char msg2[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg2[i] = s[i] ;
		}
		msg2[s.length()] = '\0' ;
		
		send(current_client , msg2 , sizeof(msg2) , 0);		//對手的玩完的手牌 但是第一張一樣蓋住 

		Sleep(500) ;
		while(now == 2){
			strcpy(sendData , "") ;
			strcpy(buf , "") ;
			
			if(p2.passFive() || p2.Sum() >= 21){
				strcpy(sendData , "bye") ;
				send(current_client , sendData , sizeof(sendData) , 0); 
				now = 1 ;
				break ;
			}
			
			strcpy(sendData , "繼續補牌?(y/n)\n") ;
			send(current_client , sendData , sizeof(sendData) , 0); 
			
			res = recv(current_client , buf , sizeof(buf) , 0); // recv cmds
			
			if(buf[0] == 'y'){
				p2.get(deck) ;
				string s = p2.send_() ;
				char msg[s.length() + 1] ;
				for(int i = 0 ; i < s.length() ; i++){
					msg[i] = s[i] ;
				}
				msg[s.length()] = '\0' ;
		
				send(current_client , msg , sizeof(msg) , 0);
			}
			else if(buf[0] == 'n'){
				char msg[] = "waiting for result...\n" ;
				send(current_client , msg , sizeof(msg) , 0);
				now = 1 ;
			}
		}
	   
		
		// 
		s = p2.send_() ;
		char msg4[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg4[i] = s[i] ;
		}
		msg4[s.length()] = '\0' ;
		cout<<"Player2 最終手牌"<<msg4<<endl;
		send(current_client , msg4 , sizeof(msg4) , 0);		//p2手牌 
		
		s = p1.send_() ;
		char msg3[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg3[i] = s[i] ;
		}
		msg3[s.length()] = '\0' ;
		cout<<"Player1 最終手牌"<<msg3<<endl;
		send(current_client , msg3 , sizeof(msg3) , 0);		//p1手牌 
		// 
		
		
		winnerID = judge(p1 , p2) ;
		strcpy(sendData, "可惜，您輸了!\n");
		cout<<"winner:"<<winnerID<<endl; 
		if(winnerID == p2.ID()){
			strcpy(sendData , "恭喜您獲勝!\n") ;
		}
		else if(winnerID == p1.ID()){
			strcpy(sendData , "可惜，您輸了!\n") ;
		}
		else if(winnerID == -87){
			strcpy(sendData , "打成平手!\n") ;
		}
		else if(winnerID == -8763){
			strcpy(sendData , "可惜，您們二位點數都爆了!\n") ;
		}
		
		send(current_client , sendData , sizeof(sendData) , 0);
	   
	   flag2=0;
	    // our recv loop
	    while(true) 
	    {
	       res = recv(current_client,buf,sizeof(buf),0); // recv cmds
	      
	       Sleep(10);
	      
	       if(strstr(buf,"play"))
	       { // greet this user
	         printf("\nrecived play cmd\n");
	         flag2=1;
	         p2.cleargame();
	         Sleep(500);
	         goto playp2;
	       }                              
	       else if(strstr(buf,"bye"))   
	       { // dissconnected this user
	         printf("\nrecived bye cmd\n");
	         strcpy(sendData,"See you again\n");
	         Sleep(10);
	         send(current_client,sendData,sizeof(sendData),0);
	  
	         // close the socket associted with this client and end this thread
	         closesocket(current_client);
	         ExitThread(0);
	       }
	       else
	       {
	       	cout<< buf <<endl;
	        strcpy(sendData,"respond from server\n");
	        Sleep(10);
	        send(current_client,sendData,sizeof(sendData),0);
	      }
	      
	      // clear buffers
	        strcpy(sendData,"");
	        strcpy(buf,"");
	    }
    }
}   
 
 
int main()
{
	
srand(time(NULL)) ;
 printf("Starting up BlackJack server\r\n");
  
 // our masterSocket(socket that listens for connections)
 SOCKET sock;
  
 // for our thread
 DWORD thread; 
  
 WSADATA wsaData;
 sockaddr_in server;
  
 // start winsock
 int ret = WSAStartup(0x101,&wsaData); // use highest version of winsock avalible
  
 if(ret != 0)
 {
    return 0;
 }
   
 // fill in winsock struct ... 
 server.sin_family=AF_INET; 
 server.sin_addr.s_addr=INADDR_ANY; 
 server.sin_port=htons(123); // listen on telnet port 23
  
 // create our socket
 sock=socket(AF_INET,SOCK_STREAM,0); 
  
 if(sock == INVALID_SOCKET)
 {
    return 0;
 }
   
 // bind our socket to a port(port 123) 
 if( bind(sock,(sockaddr*)&server,sizeof(server)) !=0 )
 {
    return 0;
 }
   
 // listen for a connection  
 if(listen(sock,5) != 0)
 {
    return 0;
 }
  
 // socket that we snedzrecv data on
 SOCKET client;
  
 sockaddr_in from;
 int fromlen = sizeof(from); 
   
 // loop forever 
 while(true)
 { 
  // accept connections
  client = accept(sock,(struct sockaddr*)&from,&fromlen);
  printf("Client connected\r\n");
countPlayer++;
  // create our recv_cmds thread and parse client socket as a parameter
  CreateThread(NULL, 0,receive_cmds,(LPVOID)client, 0, &thread);  
  
 }
  
 // shutdown winsock
 closesocket(sock); 
 WSACleanup(); 
  
 // exit
 return 0;
}
