#pragma comment(lib, "Ws2_32.lib")
 
#include <WinSock2.h>
#include <iostream>
#include <cctype> 
 
using namespace std;
 
int main()
{
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
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
            char message1fromcli[200];
            ZeroMemory(message1fromcli, 200);
            r = recv(sConnect, message1fromcli, sizeof(message1fromcli), 0);
            cout << message1fromcli << endl;
            
            char wordtoclient2[100];
            cin>>wordtoclient2;
            char *sendbuf2 = wordtoclient2;
            send(sConnect, sendbuf2, (int)strlen(sendbuf2), 0);
            
            //�怜�쪺�T�����B�@ 
            for(int i=0 ; i<sizeof(message1fromcli) ; i++){
            	message1fromcli[i]=toupper(message1fromcli[i]);
			}
			cout<<message1fromcli<<endl;
			
        }
        
        
        
        
    }
 
    //getchar();
    return 0;
}
