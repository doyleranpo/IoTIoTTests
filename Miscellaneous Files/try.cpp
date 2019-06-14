#include<iostream>
#include<stdio.h>
#include<string.h>
#include<fstream>
#include<stdlib.h>
using namespace std;

class scholar
{
	private:	
	string name,father;
        int lpunest;
	float board;
	static long id_no;
	int y;
	static int j,k;
	
	public:
		
		void in()
		{
			cout<<endl<<endl<<"Enter Details For Student("<<j++<<") :-";
			cout<<endl<<endl<<"NAME:-";
			fflush(stdin);
			getline(cin,name);
			cout<<endl<<"FATHER'S NAME:-";
			getline(cin,father);
			
		        cout<<endl<<"LPUNEST MARKS (Out Of 360):-";
			cin>>lpunest;
			
			if(lpunest>360||lpunest<0)  // invalid condition for lpunest
			{
				cout<<endl<<"Your Marks Are Not Valid They Won't Be Counted";
				cout<<endl<<"Do You Want To Proceed?"<<endl<<"(Press 1 Or 2)";
				cout<<endl<<endl<<"1.YES"<<endl<<"2.NO"<<endl<<endl;
				int x2;
				cin>>x2;
				if(x2!=1&&x2!=2)
				{
					cout<<endl<<"Invalid Choice";
					cout<<endl<<"Please Press 1 Or 2"<<endl;
					int x100;
					cin>>x100;
					if(x100==1)
					x2=1;
					else if(x100==2)
					x2=2;
					else exit(0);
				}
				if(x2==2)
				{
				cout<<endl<<"Thank You!";
				exit(0);
				}
				else if(x2==1)
				{
					cout<<endl<<"Your LPUNEST Marks Are not Going To Count"<<endl;
				}
			}
			
			cout<<endl<<"12th BOARD MARKS (In Percentage):-";
			cin>>board;
			
			if(board>100||board<0)  // invalid condition for 12th board
			{
				cout<<endl<<endl<<"Your 12th Board Marks Are Invalid";
				cout<<endl<<"SORRY! You Can't Procceed Further";
				exit(0);
			}
			
			cout<<endl<<endl<<"Have You Played National In Any Sport And Received Any Medal?"; // sports scholarship details
			cout<<endl<<"(Press 1 Or 2)";
			cout<<endl<<endl<<"1. YES"<<endl<<"2. NO"<<endl<<endl;
			int x5;
			cin>>x5;
				if(x5!=1&&x5!=2)
				{
					cout<<endl<<"Invalid Choice";
					cout<<endl<<"Please Press 1 Or 2"<<endl;
					int x300;
					cin>>x300;
					if(x300==1)
					x5=1;
					else if(x300==2)
					x5=2;
					else exit(0);
				}
			if(x5==1)
			{
				cout<<endl<<"Name Of Your Sport";
				cout<<endl<<"(Please Enter Valid Name Of The Sport Otherwise It Won't Be Counted):-";
				char a[30];
				cin>>a;
				cout<<endl<<"Which Medal Have You Received?"<<endl<<endl;
				cout<<"1. GOLD"<<endl<<"2. SILVER"<<endl<<"3. BRONZE"<<endl<<endl;
				cin>>y;
				if(y!=1&&y!=2&&y!=3)
				cout<<endl<<"Invalid Choice";
			}
			else if(x5==2)
			{
				y=20;
			}
			
			cout<<endl<<endl<<"Your Information Is Recorded. Please Wait!";
			
			if(board<40)  // fail condition
			{
				cout<<endl<<endl<<"You Are Failed In Your 12th Standard";
				cout<<endl<<"SORRY! You Can't Take Admission In LPU This Year";
				cout<<endl<<"Try Next Year";
				exit(0);
			}
			
			if(board<65)  // not eligible for b-tech condition
			{
				cout<<endl<<endl<<"You Are Passed In Your 12th Standard";
				cout<<endl<<"But Your Percentage Are Not Sufficient To Take Admission In B.Tech";
				cout<<endl<<"SORRY! You Can't Take Admission In LPU This Year";
				cout<<endl<<"Try Next Year";
				exit(0);
			}
		}
		
		void out()
		{
			cout<<endl<<endl<<"Your SCHOLARSHIP Details:-"<<endl<<endl;
			
			int x7; // test condition for lpunest
			if(lpunest>-1&&lpunest<361)
			{ 
				if(lpunest>99&&lpunest<201)
				{
					x7=30000;
					cout<<"CONGRATULATIONS! You Are Among Top 20%-30% Candidates Who Gave LPUNEST";
					cout<<endl<<"Scholarship Based On LPUNEST:-"<<x7<<"/- Per Sem";
				}
				else if(lpunest>200&&lpunest<301)
				{
					x7=40000;
					cout<<"CONGRATULUTIONS! You Are Among Top 10%-20% Candidates Who Gave LPUNEST";
					cout<<endl<<"Scholarship Based On LPUNEST:-"<<x7<<"/- Per Sem";
				}
				else if(lpunest>300&&lpunest<361)
				{
					x7=50000;
					cout<<"CONGRATULATIONS! You Are Among Top 10% Candidates Who Gave LPUNEST";
					cout<<endl<<"Scholarship Based On LPUNEST:-"<<x7<<"/- Per Sem";
				}
				else if(lpunest>-1&&lpunest<100)
				{
					x7=0;
				        cout<<endl<<"Scholarship Based On LPUNEST:- N.A"<<endl<<"(No Scholarship For Marks Less Than 100)";
		  	        }
			} else x7=0;
			
			  int x8; // test condition for 12th board
			  if(board>64&&board<101)
			  {
			  	if(x7==50000)
			  	{
			  		x8=0;
			  		cout<<endl<<"Scholarship Based On 12th Board Marks:- N.A";
			  		cout<<endl<<"(You Have Maximum Scholarship Already)";
				}
				else if(x7==40000)
			        {
					if(board>89)
					{
                                           x8=10000;
				           cout<<endl<<"Scholarship Based On 12th Board Marks:-"<<x8<<"/- Per Sem";
					}
					else if(board>69&&board<90)
					{
					   x8=5000;
					   cout<<endl<<"Scholarship Based On 12th Board Marks:-"<<x8<<"/- Per Sem";
				        }
					else if(board>64&&board<70) 
				        {
					   x8=0;
				           cout<<endl<<"Scholarship Based On 12th Board Marks:- N.A"<<endl<<"(No Scholarship For Percentage Less Than 70)";
					}
				}
				else if(x7==30000)
				{
					if(board>89)
					{
                                           x8=10000;
					   cout<<endl<<"Scholarship Based On 12th Board Marks:-"<<x8<<"/- Per Sem";
					}
					else if(board>69&&board<90)
					{
					   x8=5000;
					   cout<<endl<<"Scholarship Based On 12th Board Marks:-"<<x8<<"/- Per Sem";
				        }
					else if(board>64&&board<70) 
					{
					   x8=0;
				           cout<<endl<<"Scholarship Based On 12th Board Marks:- N.A"<<endl<<"(No Scholarship For Percentage Less Than 70)";
					}
				}
				else if(x7==0)
				{
				        if(board>89)
				        {
                                           x8=30000;
					   cout<<endl<<"Scholarship Based On 12th Board Marks:-"<<x8<<"/- Per Sem";
					}
					else if(board>79&&board<90)
					{
					   x8=25000;
					   cout<<endl<<"Scholarship Based On 12th Board Marks:-"<<x8<<"/- Per Sem";
				        }
				        else if(board>69&&board<80)
					{
					   x8=20000;
					   cout<<endl<<"Scholarship Based On 12th Board Marks:-"<<x8<<"/- Per Sem";
					}
					else if(board>64&&board<70) 
					{
					   x8=0;
				           cout<<endl<<"Scholarship Based On 12th Board Marks:- N.A"<<endl<<"(No Scholarship For Percentage Less Than 70)";
					}
				}
				}
				
				int x9; // test condition for sports scholarship
				if (y==1||y==2||y==3)
				{
					if(y==1)
					{
						x9=84500;
						cout<<endl<<endl<<"CONGRATULATION! You Are Going To Get 100% Scholarship On Tuition Fee"; 
						cout<<endl<<"On The Basis Of Sports Scholarship As You Are Gold Medalist";
					}
					else if(y==2)
					{
						x9=42250;
						cout<<endl<<endl<<"CONGRATULATION! You Are Going To Get 50% Scholarship On Tuition Fee";
						cout<<endl<<"On The Basis Of Sports Scholarship As You Are Silver Medalist";
					}
					else if(y==3)
					{
						x9=21125;
						cout<<endl<<endl<<"CONGRATULATION! You Are Going To Get 25% Scholarship On Tuition Fee";
						cout<<endl<<"On The Basis Of Sports Scholarship As You Are Bronze Medalist";
					}
				}
				else x9=0;
				
				int x55; // total scholarship calculation
				if(y==1||y==2||y==3)
				{
					cout<<endl<<endl<<"1. Total Academic Scholarship:-"<<x7+x8<<"/- Per Sem";
					cout<<endl<<"2. Total Sports Scholarship:-"<<x9<<"/- Per Sem"<<endl<<endl;
					
					if((x7+x8)>x9)
					{
						cout<<"You Will Get Academic Scholarship:-"<<(x55=x7+x8)<<"/- Per Sem";
					}	
					else cout<<"You Will Get Sports Scholarship:-"<<(x55=x9)<<"/- Per Sem";
				}
				else cout<<endl<<endl<<"Total Scholarship:-"<<(x55=x7+x8)<<"/- Per Sem";
				
				cout<<endl<<endl<<"Please Wait! You Will Receive A File In Which You Will Get All Information About Your Fee";
				cout<<endl<<"Thank You!";
				
			fstream f10; // scholarship details in a file
			f10.open("Scholarship Details.cpp",ios::out|ios::app);
			f10<<"Scholarship Details For Student("<<k++<<") :-";
			f10<<endl<<"Name:-"<<name;
			f10<<endl<<"Father's Name:-"<<father;
			f10<<endl<<"ID Number:-"<<id_no++;
			f10<<endl<<"LPUNEST MARKS:-"<<lpunest<<"/360";
			f10<<endl<<"12th Board Marks:-"<<board<<"%";
			f10<<endl<<endl<<"Total Academic Scholarship:-"<<x7+x8<<"/- Per Sem";
		        f10<<endl<<"Sports Scholarship:-"<<x9<<"/- Per Sem";
			f10<<endl<<endl<<"(TERMS:- Maximum Scholarship Among Academic Or Sports Will Be Considered)";
			f10<<endl<<endl<<"Your Fee Structure For B.Tech (CSE) is:-";
			f10<<endl<<"Tuition Fee:- 84,500/- Per Semester";
			f10<<endl<<"Exam Fee:- 3,000/- Yearly";
			f10<<endl<<"Laundry Fee:- 2,950/- Yearly";
			f10<<endl<<"Mess Fee:- 26,000/- Yearly";
			f10<<endl<<"Hostal Fee (Yearly):-"<<endl<<endl;
			f10<<"4 Seater:- 43,500/-"<<endl<<"3 Seater:- 53,500/-"<<endl<<"2 Seater:- 63,500/-"<<endl<<"1 Seater:- 73,500/-";
			f10<<endl<<endl<<"Total Scholarship:- "<<x55<<"/- Per Sem";
			f10<<endl<<endl<<"Remaining Tuition Fee:- "<<84500-x55<<"/- Per Sem"<<endl<<endl<<endl;
			f10.close();
		}
};

long scholar::id_no=2016001;
int scholar::j=1;
int scholar::k=1;

int main()
{
	cout<<"\t\t\tLOVELY PROFESSIONAL UNIVERSITY";  // welcome screen
	cout<<endl<<endl<<endl;
	cout<<"\t\t\t\tWELCOMES YOU";
	cout<<endl<<endl<<endl;
	
	cout<<"You Can Get The Following Information:-";  // operations
	cout<<endl<<endl<<"1. Fee Structure"<<endl<<"2. Scholarship Calculation For FreshMen"<<endl<<endl;
	
	int x;
	cout<<"Please Enter Your Choice (Press 1 Or 2):-";
	cin>>x;
	
	if(x==1||x==2)
	{
		if(x==1)  // fee structure details in a file
		{ 
		        fstream f1; 
		        f1.open("Fee Structure.cpp",ios::out);
			
			f1<<"Your Fee Structure For B.Tech (CSE) is:-";
			f1<<endl<<"Tuition Fee:- 84,500/- Per Semester";
			f1<<endl<<"Exam Fee:- 3,000/- Yearly";
			f1<<endl<<"Laundry Fee:- 2,950/- Yearly";
			f1<<endl<<"Mess Fee:- 26,000/- Yearly";
			f1<<endl<<"Hostal Fee (Yearly):-"<<endl<<endl;
			f1<<"4 Seater:- 43,500/-"<<endl<<"3 Seater:- 53,500/-"<<endl<<"2 Seater:- 63,500/-"<<endl<<"1 Seater:- 73,500/-";
			f1.close();
			
			cout<<endl<<endl<<"Your Fee Structure For B.Tech (CSE) is:-"; // details on console
			cout<<endl<<"Tuition Fee:- 84,500/- Per Semester";
			cout<<endl<<"Exam Fee:- 3,000/- Yearly";
			cout<<endl<<"Laundry Fee:- 2,950/- Yearly";
			cout<<endl<<"Mess Fee:- 26,000/- Yearly";
			cout<<endl<<"Hostal Fee (Yearly):-"<<endl<<endl;
			cout<<"4 Seater:- 43,500/-"<<endl<<"3 Seater:- 53,500/-"<<endl<<"2 Seater:- 63,500/-"<<endl<<"1 Seater:- 73,500/-";
			cout<<endl<<endl<<"You Will Receive A FILE Regarding Your Fees For Your Reference"<<endl<<endl<<"THANK YOU!";
		}
		
		else if(x==2)
		{
			cout<<endl<<endl<<"Welcome To Scholarship Calculation For FreshMen";
			cout<<endl<<endl<<"Calculate Scholarship For How Many Students ?"<<endl<<"(Please Enter Number Of Students)\t";
			
			int n,i;
			cin>>n;
			scholar *s1;
			s1=new scholar[n];  // object of a class
			for(i=0;i<n;i++)
			{
				(s1+i)->in();
				(s1+i)->out();
			}
		}
}
else cout<<endl<<endl<<"Invalid Choice";
return 0;
}