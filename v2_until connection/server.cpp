#include <windows.h>
#include  <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include <iostream>
using namespace std;
 
class Deck{
	private:
		int count ;
		int point[13] = {0} ;
		int total = 0 ;
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
				cout << "A " ;
			}
			cout << endl ;
			for(int i = 1 ; i < 10 ; i++){
				for(int j = 0 ; j < point[i] ; j++){
					cout << i+1 << ' ' ;
				}
				cout << endl ;
			}
			for(int i = 10 ; i < 13 ; i++){
				for(int j = 0 ; j < point[i] ; j++){
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
		char holding[5] = {0} ;
		int holdingCnt = 0 ;
		int sum = 0 ;
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
			if(this->holding[holdingCnt-1] > 10) sum += 10 ;
			else sum += this->holding[holdingCnt-1] ;
			return this->holding[holdingCnt-1] ;
		}
		void print(){
			for(int i = 0 ; i < holdingCnt ; i++){
				//cout << (char)(holding[i]+48) << ' ' ;
				if(holding[i] == 1){
					cout << "A " ;
				}
			//	else if(holding[i] == 10){
			//		cout << "10 " ;
			//	}
				else if(holding[i] == 11){
					cout << "J " ;
				}
				else if(holding[i] == 12){
					cout << "Q " ;
				}
				else if(holding[i] == 13){
					cout << "K " ;
				}
				else{
					cout << (int)(holding[i]) << ' ' ;
				}
			}
//			cout << sum ;
			if(sum > 21) cout << "(爆)" ; 
			cout << endl ;
		}
		int Sum(){
			return sum ;
		}
};
 
int countPlayer=0;
int flag=0;		//flag=0(wait) flag=1(player1 move) flag=2(player2 move) 
int flag1=0;	//flag for player1  if(flag1==1) ready to start game
int flag2=0;
 
// our thread for recving commands
DWORD WINAPI receive_cmds(LPVOID lpParam) 
{
  printf("thread created\r\n");
  cout<<"thread"<<countPlayer<<endl;
   
  // set our socket to the socket passed in as a parameter   
  SOCKET current_client = (SOCKET)lpParam; 
   
    if(countPlayer==1){
    	Player p1;
	  	// buffer to hold our recived data
	    char buf[100];
	    // buffer to hold our sent data
	    char sendData[100];
	    // for error checking 
	    int res;
	   
	   //send to client
	   	sendData[0]='1';
	   	send(current_client,sendData,sizeof(sendData),0); 
	   
	   	//receive from client
		res = recv(current_client,buf,sizeof(buf),0); // recv cmds
		string name;
		name=buf;
	   	cout<<"player1's name is "<< name <<endl;
	    p1.set(name,1);
	   
	   flag1=1;
	   
	   playp1:
	   while( (flag1+flag2)!=2 ){
	   	 if(flag1+flag2==2)	break;
	   }
	   cout<<"player1 start play game"<<endl;
	   
	   strcpy(sendData,"Connection Success\n");
	   send(current_client,sendData,sizeof(sendData),0);
	   
	   
	   
	   
	   
	   //程式打在這裡~ 
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   flag1=0;
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
	         flag1=1;
	         goto playp1;
	       }                              
	       else if(strstr(buf,"bye"))   
	       { // dissconnected this user
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
	   
	   flag2=1;
	   
	   playp2:
	   while( (flag1+flag2)!=2 ){
	   	 if(flag1+flag2==2)	break;
	   }
	   cout<<"player2 start play game"<<endl;
	   
	   strcpy(sendData,"Connection Success\n");
	   send(current_client,sendData,sizeof(sendData),0);
	   
	   
	   
	   
	   //打在這裡~ 
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
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
   
  // create our recv_cmds thread and parse client socket as a parameter
  CreateThread(NULL, 0,receive_cmds,(LPVOID)client, 0, &thread);  
  countPlayer++;
 }
  
 // shutdown winsock
 closesocket(sock); 
 WSACleanup(); 
  
 // exit
 return 0;
}
