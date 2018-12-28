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

void setColor(int color){
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,color);
}
void printCard(char number[]){         //暗牌 128
	int len,n=0;
	len=strlen(number);
	for(int i=0;i<len;i++){
		if(number[i]=='A')
			n++;
		if(number[i]=='2')
			n++;
		if(number[i]=='3')
			n++;
		if(number[i]=='4')
			n++;
		if(number[i]=='5')
			n++;
		if(number[i]=='6')
			n++;	
		if(number[i]=='7')
			n++;
		if(number[i]=='8')
			n++;
		if(number[i]=='9')
			n++;
		if(number[i]=='J')
			n++;
		if(number[i]=='K')
			n++;
		if(number[i]=='Q')
			n++;
		if(number[i]=='0')
			n++;
		if(number[i]=='?')	
			n++;
	}
	
	int* num=new int [n];
	int* flower=new int[n];
	
	for(int i=0;i<n;i++){
		flower[i]=rand()%4;
		num[i]=0;
	}
	int j=0;
	for(int i=0;i<len;i++){
		if(number[i]=='A'){
			num[j]=1;
			j++;
		}
		if(number[i]=='2'){
			num[j]=2;
			j++;
		}
		if(number[i]=='3'){
			num[j]=3;
			j++;
		}
		if(number[i]=='4'){
			num[j]=4;
			j++;
		}
		if(number[i]=='5'){
			num[j]=5;
			j++;
		}
		if(number[i]=='6'){
			num[j]=6;
			j++;
		}
		if(number[i]=='7'){
			num[j]=7;
			j++;
		}
		if(number[i]=='8'){
			num[j]=8;
			j++;
		}
		if(number[i]=='9'){
			num[j]=9;
			j++;
		}
		if(number[i]=='J'){
			num[j]=11;
			j++;
		}
		if(number[i]=='K'){
			num[j]=13;
			j++;
		}
		if(number[i]=='Q'){
			num[j]=12;
			j++;
		}
		if(number[i]=='0'){
			num[j]=10;
			j++;
		}
		if(number[i]=='?'){        
			num[j]=14;
			j++;
		}
	}
	//+---------+
	//|    ?    |
	//|  ?????  |
	//|    ?    |
	//|    ?    |
	//+---------+

	string boundary = "+---------+";
	string      bar = "|         |";
    string       left="|    ";
	string            right="    |";
	string      leftf="|   ";
	char word[14]={'A','2','3','4','5','6','7','8','9','x','J','Q','K','?'};
	//flower: 0   1  2  3
	//       桃  愛 磚 梅 

	//upboundary
	for(int i=0;i<n;i++){
		if(num[i]==14){
			setColor(128);
			cout<<boundary;
		}
		else{
			setColor(240);
			cout<<boundary;
		}
		setColor(7);
		cout<<" ";
		
	}cout<<endl;
	//secondline
	for(int i=0;i<n;i++){
		if(num[i]==14){
			setColor(128);
			cout<<"|    ?    |";
		}
		else{
		setColor(240);
		if(num[i]!=10){
			cout<<left;
			if(flower[i]==1||flower[i]==2){//愛,梅 
				setColor(252);
				cout<<word[num[i]-1];
			}
			if(flower[i]==0||flower[i]==3){//桃,梅 
				setColor(240);
				cout<<word[num[i]-1];
			}setColor(240);
			cout<<right;	
		}
		else if(num[i]==10){
			if(flower[i]==1||flower[i]==2){//愛,梅 
				setColor(240);
				cout<<"|";
				setColor(252);
				cout<<"   1 0   ";
				setColor(240);
				cout<<"|";
			}
			if(flower[i]==0||flower[i]==3){//桃,梅 
				setColor(240);
				cout<<"|   1 0   |";
			}setColor(240);
		}
		}
		setColor(7);
		cout<<" ";
	}cout<<endl;
	//thirdline
	for(int i=0;i<n;i++){
		if(num[i]==14){
			setColor(128);
			cout<<"|  ?????  |";
		}
		else{
			setColor(240);
			cout<<bar;
		}
		setColor(7);
		cout<<" ";
	}cout<<endl;
	//forthline
	for(int i=0;i<n;i++){
		if(num[i]==14){
			setColor(128);
			cout<<"|    ?    |";
		}
		else{
			setColor(240);
			cout<<bar;
		}
		setColor(7);
		cout<<" ";
	}cout<<endl;
	//fifthline
	for(int i=0;i<n;i++){
		if(num[i]==14){
			setColor(128);
			cout<<"|    ?    |";
		}
		else{
			setColor(240);
			cout<<leftf;
			if(flower[i]==1){//愛 
				setColor(252);
				cout<<"愛";
			}
			if(flower[i]==2){//磚 
				setColor(252);
				cout<<"磚";
			}
			if(flower[i]==0){//桃 
				setColor(240);
				cout<<"桃";
			}
			if(flower[i]==3){//梅 
				setColor(240);
				cout<<"桃";
			}
			setColor(240);
			cout<<right;
		}
		setColor(7);
		cout<<" ";
	}cout<<endl;
	//downboundary
	for(int i=0;i<n;i++){
		if(num[i]==14){
			setColor(128);
			cout<<boundary;
		}
		else{
			setColor(240);
			cout<<boundary;
		}
		setColor(7);
		cout<<" ";
	}cout<<endl;
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
	printCard(RecvdData);
    
    strcpy(RecvdData,""); 
   	ret = recv(sock , RecvdData , sizeof(RecvdData) , 0);		//對手初始手牌 
   	cout << "對手的手牌: " ; 
	cout << RecvdData ;
	printCard(RecvdData);
	
    
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
	printCard(RecvdData);
	
    
	strcpy(RecvdData,"");
	ret = recv(sock,RecvdData,sizeof(RecvdData),0);
	cout << "對手的牌: " ;
	cout << RecvdData ;
	printCard(RecvdData);
    
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
