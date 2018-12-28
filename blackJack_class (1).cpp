#include <iostream>
#include <string>
#include <ctime> 
#include <cstring>
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
		void print(){
			cout << "ヘ玡礟帮猵\n" ; 
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
		int holding[100] = {0} ;			//も礟程き眎(int篈 计1~13) 
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
		void Sort_(){
			for(int i = 0 ; i < this->holdingCnt-1 ; i++){
				for(int j = 0 ; j < this->holdingCnt-1-i ; j++){
					if(this->holding[j] > this->holding[j+1]){
						int temp = this->holding[j] ;
						this->holding[j] = this->holding[j+1] ;
						this->holding[j+1] = temp ;
					}
				}
			}
			return ;
		}
		void print(){
			for(int i = 0 ; i < holdingCnt ; i++){
				if(holding[i] == 1){
					cout << "A " ;
				}
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
			if(Sum() > 21) cout << "(脄)" ; 
			else if(this->passFive()) cout << "(筁き闽)" ;
			
			if(Sum() == 21) cout << "(blackjack!)" ;
			cout << endl ;
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
		void printWinner(){
			cout << "墓產琌" << name << ' ' << id << "!!!" << endl ; 
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


int main(){
	srand(time(NULL)) ;
	
	Player a;
	Deck deck(1) ;
	for(int i = 0 ; i < 13 ; i++){
		a.get(deck) ;
	}
	a.Sort_() ;
	a.print() ;
	string s = a.send_() ;
	
	s = a.send_() ;
	cout << s ;
	cout << endl ;
	
	char cs[1000] ;
	for(int i = 0 ; i < s.length() ; i++){
		cs[i] = s[i] ;
	}
	cs[s.length()] = '\0' ;
	
	cout << cs ;
	//
	return 0 ;
}
