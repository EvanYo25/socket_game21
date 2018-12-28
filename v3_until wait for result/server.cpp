#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <windows.h>
#include <winsock.h>

#include <string>
#include <cstring>
#include <ctime>
using namespace std;
 
class Deck{
	private:
		int count ;				//ノ碭捌汲礟 
		int point[13] = {0} ;	//翴计眎计 
		int total = 0 ;			//礟舱逞緇羆眎计 
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
};

class Player{
	private:
		string name ;
		int id ;
		int holding[5] = {0} ;			//も礟程き眎(int篈 计1~13) 
		int holdingCnt = 0 ;			//ヘ玡も礟计
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
		int get(Deck& dek){
			this->holding[holdingCnt++] = dek.draw() ;
			return this->holding[holdingCnt-1] ;
		}
		int Sum(){					//σ納A猵ノㄧ计τぃノ跑计sum 
			int sum = 0 ;
			for(int i = 0 ; i < this->holdingCnt ; i++){
				if(this->holding[i] == 1){
					sum += 11 ;
					if(sum > 21) sum -= 10 ;
				}
				else if(this->holding[i] > 10){
					sum += 10 ;
				}
				else{
					sum += this->holding[i] ;
				}
			}
			return sum ;
		}
		bool passFive(){
			return (this->holdingCnt == 5 && this->Sum() <= 21) ;
		}
		int cardCnt(){
			return holdingCnt ;
		}
		string send_(){
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
			s += "\n" ;
			
			return s ;
		}
};

Deck deck(4) ;

int countPlayer=0;
int flag=0;		//flag=0(wait) flag=1(player1 move) flag=2(player2 move) 
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
		Player p1;
		
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
	   
		p1.get(deck);				//穞礟 
		p1.get(deck);
		
		string s = p1.send_() ;
		char msg[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg[i] = s[i] ;
		}
		msg[s.length()] = '\0' ;
		
		send(current_client , msg , sizeof(msg) , 0); 
		
		Sleep(500) ;
		while(now == 1){
			
			strcpy(sendData , "膥尿干礟?(y/n)\n") ;
			send(current_client , sendData , sizeof(sendData) , 0); 
			
			res = recv(current_client , buf , sizeof(buf) , 0); // recv cmds (yes or no)
			
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
    	Player p2;
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
		send(current_client , sendData , sizeof(sendData), 0);
	   
	   playp2:
	   while( (flag1+flag2)!=2 ) ;
	   cout<<"player2 start play game"<<endl;
	   
		send(current_client,"connection success\n",sizeof(sendData),0); 	   
	   
	   
	   
	   
	   //ゴ硂柑~ 
	   
//	   send(current_client,"play~~~\n",sizeof(sendData),0); 
	   
	   p2.get(deck);				//穞礟 
	   p2.get(deck);
	   
		string s = p2.send_() ;
		char msg[s.length() + 1] ;
		for(int i = 0 ; i < s.length() ; i++){
			msg[i] = s[i] ;
		}
		msg[s.length()] = '\0' ;
		
		send(current_client , msg , sizeof(msg) , 0); 
		
		while(now != 2) ;

		//Sleep(500) ;
		while(now == 2){
			strcpy(sendData , "") ;
			strcpy(buf , "") ;
			
			
			strcpy(sendData , "膥尿干礟?(y/n)\n") ;
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
	   
	   
	   
	   
	   flag2=0;
	    // our recv loop
	    while(true) 
	    {
	       res = recv(current_client,buf,sizeof(buf),0); // recv cmds
	      
	       Sleep(10);
	       
	       if(res == 0)
	       {
	        MessageBox(0,"error","error",MB_OK);
	        closesocket(current_client);
	        ExitThread(0);
	       }
	      
	       if(strstr(buf,"play"))
	       { // greet this user
	         printf("\nrecived play cmd\n");
	         flag2=1;
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
