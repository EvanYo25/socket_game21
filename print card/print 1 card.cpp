#include <iostream>
using namespace std;
void printCard(string num){
	cout<<"¢z-----¢{"<<endl; 
	if(num=="10"){
		cout<<"| 1 0 |";
	}
	else { 
		cout<<"|  "<<num<<"  |";
	} 
	cout<<endl;

	cout<<"|     |"<<endl;
	cout<<"¢|-----¢}"<<endl; 
}

int main(){
	string haha;
	cin>>haha; 
	printCard(haha);
	return 0;
}
