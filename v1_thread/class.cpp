#include <iostream>
#include <cstring>
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

int main (){
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
	
	
	for(int i=0 ; i<countP ; i++){
		p[i]->printPlayer();
	}

	int aaa=23;
	int bbb=10000;
	
	for(int i=0 ; i<countP ; i++){
		if(p[i]->ID()==aaa){
			p[i]->addMoney(bbb);
			break;
		}
	}
	
	for(int i=0 ; i<countP ; i++){
		p[i]->printPlayer();
	}
	
	return 0;
}
