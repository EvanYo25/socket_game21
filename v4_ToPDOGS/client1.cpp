/*-------------------Echo_client.cpp-------------------*/
#include <stdio.h>
#include <iostream>
#include <conio.h>			//?

#include <windows.h>
#include <winsock.h>

#include <signal.h>
using namespace std;

 
//DECLARATIONS:
//1.error trapping signals
#define SIGINT 2
#define SIGKILL 9
#define SIGQUIT 3
//2.SOCKETS
SOCKET sock , client ;

void s_handle(int s)
{
	if(sock) closesocket(sock);
	if(client) closesocket(client);
	
	WSACleanup();
    Sleep(1000);
    cout << endl << "EXIT SIGNAL :" << s ;
    exit(0);
}

void s_cl(char *a, int x)
{
    cout << a ;
    s_handle(x+1000);
}

int main(){
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
     
	//Declarations
	DWORD poll;
	int res , i = 1 , port = 999 ;
	char buf[100] ;
	char msg[100] = "" ;
	char ip[15] ;
	WSADATA data ;

    signal(SIGINT,s_handle);
    signal(SIGKILL,s_handle);
    signal(SIGQUIT,s_handle);
    
    cout<<"Enter IP to connect to: ";
    gets(ip);

	sockaddr_in ser;
	sockaddr addr; 


    ser.sin_family=AF_INET;
    ser.sin_port=htons(123);				//Set the port
    ser.sin_addr.s_addr=inet_addr(ip);		//Set the address we want to connect to
    
    memcpy(&addr,&ser,sizeof(SOCKADDR_IN));

    res = WSAStartup(MAKEWORD(1,1),&data);      //Start Winsock
/*    cout<<"\n\nWSAStartup"
        <<"\nVersion: "<<data.wVersion
        <<"\nDescription: "<<data.szDescription
        <<"\nStatus: "<<data.szSystemStatus<<endl;
*/
	cout << "Status:" << data.szSystemStatus << endl ;

	if(res != 0) s_cl("WSAStarup failed",WSAGetLastError());

    sock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);       //Create the socket
	
	if(sock==INVALID_SOCKET) s_cl("Invalid Socket ",WSAGetLastError());
	else if(sock==SOCKET_ERROR) s_cl("Socket Error",WSAGetLastError());
	else cout<<"Socket Established"<<endl;


    res=connect(sock,&addr,sizeof(addr));               //Connect to the server
        if(res !=0 )
        {
            s_cl("SERVER UNAVAILABLE",res);
        }
        else
        {
            cout<<"\nConnected to Server: \n";
            memcpy(&ser,&addr,sizeof(SOCKADDR));
        }
     
    char RecvdData[100] = "";
    int ret;
    
 	//receive to server
    strcpy(RecvdData,""); 
   	ret = recv(sock,RecvdData,sizeof(RecvdData),0);
   	cout<<"You are player "<<RecvdData[0]<<endl;
   	cout<<"What's your Name?"<<endl;
   	cout<<"Your name : ";
   	
    //send to server
    strcpy(buf , "");
	fgets(buf , sizeof(buf),stdin);
    Sleep(5); 
    res = send(sock,buf,sizeof(buf),0); 

  
    
    strcpy(RecvdData,""); 
   	ret = recv(sock , RecvdData , sizeof(RecvdData) , 0);		//Waiting...
   	cout << RecvdData ;


	playagain:

    strcpy(RecvdData,""); 
   	ret = recv(sock , RecvdData , sizeof(RecvdData) , 0);		//connection success
   	cout << RecvdData ;

    strcpy(RecvdData,""); 
   	ret = recv(sock , RecvdData , sizeof(RecvdData) , 0);		//初始手牌 
   	cout << "初始手牌(第一張是暗牌): " ; 
	cout << RecvdData ;
    
    strcpy(RecvdData,""); 
   	ret = recv(sock , RecvdData , sizeof(RecvdData) , 0);		//對手初始手牌 
   	cout << "對手的手牌: " ; 
	cout << RecvdData ;
	
    
    while( strcmp(RecvdData , "waiting for result...\n") != 0 ){
		strcpy(RecvdData , "");
	    ret = recv(sock , RecvdData , sizeof(RecvdData) , 0);
		
		if( !strcmp(RecvdData , "bye") ){
			cout << "waiting for result...\n" ;
			break ;
		}
		
		cout << RecvdData ;
	    
		char ans ;
	    cin >> ans ;
	    res = send(sock,&ans,sizeof(ans),0);
	
	    strcpy(RecvdData,"");
	    ret = recv(sock,RecvdData,sizeof(RecvdData),0);
	    cout << RecvdData ;
	}
    
    
	strcpy(RecvdData,"");
	ret = recv(sock,RecvdData,sizeof(RecvdData),0);    
	cout << "攤牌!\n" ;
	cout << "您的牌: " ;
	cout << RecvdData ;
	
    
	strcpy(RecvdData,"");
	ret = recv(sock,RecvdData,sizeof(RecvdData),0);
	cout << "對手的牌: " ;
	cout << RecvdData ;
    
    //cout<<"hi";
	strcpy(RecvdData,"");
	ret = recv(sock,RecvdData,sizeof(RecvdData),0);
	cout << RecvdData ;
	//cout<<"hi2";
	
    //send to server
    cout<<"type [bye] to exit"<<endl;
    
	strcpy(buf,"");
//	fgets(buf,100,stdin);
	cin >> buf ;
    Sleep(5); 
    res = send(sock,buf,sizeof(buf),0); 
	if(strstr(buf,"play")){
		goto playagain;
	}
	else if(strstr(buf,"bye")){
		strcpy(RecvdData,""); 
   		ret = recv(sock,RecvdData,sizeof(RecvdData),0);
   		cout<<RecvdData;
   		Sleep(1000);
		closesocket(client);
    	WSACleanup();
	}
     
    return 0;
}
/*-------------------Echo_client.cpp-------------------*/
