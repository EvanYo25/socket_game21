#pragma comment(lib, "Ws2_32.lib")
 
#include <WinSock2.h>
#include <iostream>
#include <cctype> 
#include <cstring>
#include <stdlib.h>
using namespace std;
 
class Player{
private:
	int id;
	string name;
	int money;
public:
	Player(){
		id=0;
		name="NoName";
		money=0;
	}
	Player(int id, string name){
		this->id=id;
		this->name=name;
		money=0;
	}
	int ID() const{
		return id;
	}
	void printPlayer(){
		cout<<this->id<<" "<<this->name<<" "<<this->money<<endl;
	}
	void addMoney(int num){
		this->money+=num;
	}
	void minusMoney(int num){
		this->money-=num;
	}
};


int main(){
	Player* p[100];
	char type;
	int countP=0;
	
	cout<<"Welcome to the system!"<<endl<<"[C]to create new player and [S]to stop creating player"<<endl;
	cout<<"Ex:\"C 23 Evan\" means create a player name Evan and his number is 23"<<endl;
	while(cin>>type){
		if(type=='C'){
			int pID=0;
			string pName;
			cin>>pID>>pName;
			p[countP]=new Player(pID,pName);
			countP++;
		}
		
		else if(type=='S'){
			break;
		}
		cout<<"[C]to create more new player and [S]to stop creating player"<<endl;
	}
	
/*	for(int i=0 ; i<countP ; i++){
		p[i]->printPlayer();
	}
*/	
	
	//=========================================================================
    int r;
    WSAData wsaData;
    WORD DLLVSERION;
    DLLVSERION = MAKEWORD(2,1);//Winsocket-DLL ����
 
    //�� WSAStartup �}�l Winsocket-DLL
    r = WSAStartup(DLLVSERION, &wsaData);
 
    //�ŧi socket ��}��T(���P���q�T,�����P����}��T,�ҥH�|�����P����Ƶ��c�s��o�Ǧ�}��T)
    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
 
    //�إ� socket
    SOCKET sListen; //listening for an incoming connection
    SOCKET sConnect; //operating if a connection was found
 
    //AF_INET�G��ܫإߪ� socket �ݩ� internet family
    //SOCK_STREAM�G��ܫإߪ� socket �O connection-oriented socket 
    sConnect = socket(AF_INET, SOCK_STREAM, NULL);
 
    //�]�w��}��T�����
    //addr.sin_addr.s_addr = inet_addr("10.122.216.144");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
 
    //�]�w Listen
    sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);//SOMAXCONN: listening without any limit
 
    //���ݳs�u
    SOCKADDR_IN clinetAddr;
    while(true)
    {
        cout << "waiting..." << endl;
 
        if(sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
        {
            cout << "a connection was found" << endl;
            printf("server: got connection from %s\n", inet_ntoa(addr.sin_addr));
 
            //�ǰe�T���� client ��
            char wordtoclient[100]="Connection Success!(message from server)";
            char *sendbuf = wordtoclient;
            send(sConnect, sendbuf, (int)strlen(sendbuf), 0);
            
            //����client���T�� 
            char message1[200];
            ZeroMemory(message1, 200);
            r = recv(sConnect, message1, sizeof(message1), 0);
            cout << "ID" << message1 << " ";
            
            char message2[200];
            ZeroMemory(message2, 200);
            r = recv(sConnect, message2, sizeof(message2), 0);
            cout << message2[0];
            
            char message3[200];
            ZeroMemory(message3, 200);
            r = recv(sConnect, message3, sizeof(message3), 0);
            cout << message3 << endl << endl;
            
            int mes1=atoi(message1);
            int mes3=atoi(message3);
			if(message2[0]=='+'){
            	for(int i=0 ; i<countP ; i++){
            		if(p[i]->ID()==mes1){
            			p[i]->addMoney(mes3);
            			break;
					}
				}
			}
			else if(message2[0]=='-'){
				for(int i=0 ; i<countP ; i++){
					if(p[i]->ID()==mes1){
						p[i]->minusMoney(mes3);
						break;
					}
				}
			}
			else{
				cout<<"Error!!";
			}
			
        }
        
        for(int i=0 ; i<countP ; i++){
        	p[i]->printPlayer();
		}
        
    }
 
    //getchar();
    return 0;
}
