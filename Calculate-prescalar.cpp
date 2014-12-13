#include<iostream>
using namespace std;

int main()
{
	float delay,mindelay;
	int prescalar,timer,toverflow,over;
	long double clk;
	float counts,complete;


	cout<<"Enter the clock Frequency:";
	cin>>clk;
	cout<<"\nEnter the delay you want in seconds:";
	cin>>delay;
	cout<<"\nEnter the Prescalar:";
	cin>>prescalar;
	cout<<"\nEnter the timer register(0/1)?:";
	cin>>timer;


	/// 8-Bit clock
	if(timer==0)
	{
		/// Calculate the minimum time delay
		mindelay = 1/(clk/prescalar);
		/// Calculate the total number of counts the clock will have to do
		counts = delay/mindelay-1;
		/// Calculate the time for completing one full round of 256
		complete = 65536*mindelay;
		/// Calculate the bumber of time the counter overflows
		toverflow = int(delay/complete);
		/// Calcluate the number of counts the clock will do in the next counter
		over = int((delay-(toverflow*complete))/mindelay - 1);

		cout<<"\nThe total numnber of overflows will be:"<<toverflow;
		cout<<"\nThe number of ticks in the last counter will be:"<<over;
		cout<<endl;
			
	}

}