/*This project was done by:
Jordan Patterson
Bergen Davis 
Brittany Anderson
*/
////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
//Header files
////////////////////////////////////////////////
#define BACKSPACE 127 //value for backspace
#define UP 65 //value for up key
#define DOWN 66 //value for down key
#define FALSE 0
#define TRUE 1
//Global variables(constants)
char *parishes[14]={"   KINGSTON    ","  ST. ANDREW   ","   PORTLAND    ","  ST. THOMAS   ","   ST. MARY    ",
					" ST. CATHERINE ","    ST. ANN    ","   MANCHESTER  ","   CLARENDON   ","    HANOVER    ",
					" ST. ELIZABETH ","   ST. JAMES   ","    TRELAWNY   ","  WESTMORELAND "
				};
////////////////////////////////////////////////
typedef struct date{
	char month[10];
	int day;
	int year;
}DATE;
typedef struct company{
	int idnum;
	char name[30];
	char contactPerson[30];
	char email[30];
	int telephone;
}COMPANY;
typedef struct client{
	int idnum;
	char name[30];
	char idtype[30];
	char companyCode[15];
	int companyidnum;
	int personalIDnum;
}CLIENT;
typedef struct employee{
	char title[15];
	int idnum;
	char name[30];
	char username[15];
	char password[30];
}EMPLOYEE;
typedef struct sale{
	int idnum;
	int EEidnum;
	char companycode[15];
	char customerName[30];
	int customerID;
	char petroltype[20];
	int pumpnum;
	float costPerGal;
	int petrolAmount;
	DATE dos;
	float value;
	int employeeID;
	char employeeName[30];
}SALE;
typedef union record{
	EMPLOYEE employee;
	CLIENT customer;
	SALE sale;
	COMPANY company;
}RECORD;
//Structures and unions
////////////////////////////////////////////////
void printComp(COMPANY *ptr);
void updateComp();
void updateEMP();
void updateCLI();
void printEmp(EMPLOYEE *emp);
void creditRep();
float totalcredit(int id);
int clientcount(int id);
void getID(int clientIDs[],int companyidnum);
void ClientReport();
void Petrolreport(FILE *file,int num);
void petrolReportMenu();
void update();
void searchMain();
int search(int key,int num);
void report();
float sum(float a, float b);
float diff(float a, float b);
float divide(float a, float b);
float mult(float a, float b);
int Calc_menu(char **list,int size);
void Calc_arrow(char **list,int x,int size);
void Calculator();
float *result=NULL;
//CALCULATOR
////////////////////////////////////////////////
CLIENT *VerifyClient();
void SellPetrol();
void getSaleData();
int eagleID(const char *parish);
int checkLogin(char *username,char *password);
int getch();//linux does not have conio.h so this is an alternative
int Login();//returns true if both the username and encrypted password is equal to the ones above, false if otherwise
void GetPassword();//function to hide a string while it is being entered
void arrow(int x,char **options,int size,char *header,char *instructions);//takes a list of options and prints an arrow beside the string with the index that is equal to x
/*takes in list of options,size of list,header(logo to be displayed),instructions pertaining to the menu*/
int menu(char **options,int size,char *header,char *instructions);//returns an integer that indicates the option the user selected
void printLogo(char *filename);//prints out contents of a textfile(filename)
int generateID(char *filename,int size);//generates and returns an ID based on how many records are in the file->filaname
void AddCompany();//prompts for company details
void AddCustomer();//prompts for new customer details
void printClientInfo(CLIENT *pointer);//takes in a pointer that points to client structure for printing out data without having to make a copy of said structure
void CheckClientInfo(CLIENT *pointer);//prompts the user if there are errors in any of the client info entered and allows changes to be made if any
void chooseID(CLIENT *pointer);//provides a small menu to choose the type of identification
void AddEmployee();//prompts for new employee info and credentials
int checkUsername(char *username);//checks if username exists, return False if it does else return True if not
void Update();
void getRidofnewline(char *buffer);//due to the use of fgets(), a '\n' is added to the end of string so this function is self-explanatory
void display();//provides a menu asking user to select the type of data to display
int printFromFile(int num);//prints either employee, customer or company data based on an integer num
void getParish();
void Add();
//User-Defined function prototypes
////////////////////////////////////////////////
const char const *parish_pointer=NULL;//constant pointer points to the parish that was selected in getParish() function
const EMPLOYEE const* activeUser=NULL;//constant pointer points to the employee structure of the person logged on
const DATE const* date=NULL;
////////////////////////////////////////
int main(){
	DATE today;
	date=&today;
	char *instructions="\n\n\t\t\t\t\t\t    WELCOME TO THE EAGLE ENERGY MAIN MENU\n\t\t\t\t\t\t        USE ARROW KEYS TO NAVIGATE.\n\n\n\n";
	int entry=10;
	int result=Login();
	char *main[8]={"      SELL PETROL       ","     DISPLAY RECORDS    ","     DISPLAY REPORT     ",
	               "   SEARCH FOR RECORD    ","       ADD RECORD       ","      UPDATE RECORD     ",
	               "       CALCULATOR       ","         EXIT           "};
	if(result){
		printf("\n\n\n\t\t\t\t\t\t\t     [ LOGIN SUCCESSFUL. ]\n\n");
		sleep(1);
		getch();
		getParish();
		printLogo("logo2.txt");
		printf("\n\n\n\n\t\t\t\t\t\tENTER THE CURRENT DAY:");
		scanf("%d",&today.day);
		printf("\n\n\t\t\t\t\t\tENTER THE CURRENT MONTH (ALL COMMON):");
		getch();
		fgets(today.month,sizeof(today.month)/sizeof(char),stdin);
		getRidofnewline(today.month);
		printf("\n\n\t\t\t\t\t\tENTER THE CURRENT YEAR:");
		scanf("%d",&today.year);
		printLogo("welcome.txt");
		sleep(3);
		getch();
		while(entry!=7){
			entry=menu(main,8,"logo2.txt",instructions);
			switch(entry){
				case 0: SellPetrol();
				break;
				case 1: display();
				break;
				case 2: report();
				break;
				case 3: searchMain();
				break;
				case 4: Add();
				break;
				case 5: update();
				break;
				case 6: Calculator();
				break;
			}
		}
	}else{
		printf("\n\n\n\t\t\t\t\t\t\t[ LOGIN FAILED. YOU HAVE BEEN BLOCKED ]\n\n");
	}
}
void printLogo(char *filename){
  char data;
  system("clear");
	printf("\n");
  FILE *file=NULL;
  file = fopen(filename,"r");
  if(file==NULL){
    printf("Error: File does not exist");
  }else{
    while((data=getc(file))!=EOF){
      putchar(data);//prints character until it reaches end of file
    }
    printf("\n");
  }
}
int menu(char **options,int size,char *header,char *instructions){
	int key,count=0;
	arrow(0,options,size,header,instructions);
	while(key!='\n'){
		key=getch();
		printf("\a");
		if(key==DOWN){
			if(count<(size-1)){
				count++;
			}
		}else if (key==UP && count>0){
				count--;
		}
		arrow(count,options,size,header,instructions);
	}
	return count;
}
void arrow(int x,char **options,int size,char *header,char *instructions){
	int i;
	time_t _TIME_;
	struct tm * timeinfo;
	time(&_TIME_);
	timeinfo=localtime(&_TIME_);
	system("clear");
	printLogo(header);
	if(activeUser!=NULL){
		printf("\n\t\t\t\tCURRENTLY LOGGED ON: %s\t\tCURRENT TIME: %s",activeUser->name,asctime(timeinfo));
		printf("\n\t\t\t\tTITLE/ POSITION: %s",activeUser->title);
		if(parish_pointer!=NULL){
			printf("\t\t        LOCATION: %s",parish_pointer);
		}
	}
	printf("%s",instructions);
	for(i=0;i<size;i++){
			printf("\t\t\t\t\t\t %s[%s]%s\n\n",(i==x)? "@@-->>>":"       ",options[i],(i==x)? "<<<--@@":"        ");
	}
}
int Login(){
	int i=1,result;
	FILE *fp=NULL;
	system("clear");
	printf("\n\n\n\t\t\t\t\t\t\tWELCOME TO. .\n\n");
	printf("\n\n\n\t\t\t\t\tI'll show you how deep the rabbit hole goes. . . .");
	getch();
	char user[30],pass[30];
	while(i<=3){
		printLogo("logo2.txt");
		if((fp=fopen("employees.dat","rb"))==NULL){
			printf("\n\t\t\t\t\t\tOops! Seems like there aren't any employees\n\t\t\t\t\t\t    SO LETS ADD A NEW EMPLOYEE ACCOUNT");
			printf("\n\n\t\t\t\t\t\tPRESS ANY KEY TO CONTINUE. . .");
			getch();
			getch();
			getch();
			AddEmployee();
			printf("\n\n\t\t\t\t\tPRESS ANY KEY TO CONTINUE. . .");
			getch();
			continue;
		}
		printf("\n\n\n\n\t\t\t\t\t\t\t\t [ Attempt %d of 3 ]\n\n",i);
		i++;
		printf("\n\n\t\t\t\t\t\t\t  [ ENTER USERNAME: ");
		fgets(user,sizeof(user)/sizeof(char),stdin);
		getRidofnewline(user);
		printf("\n\n\t\t\t\t\t\t\t  [ ENTER PASSWORD: ");
		GetPassword(pass,30);
		/*
		if(*user=='\0' || *pass=='\0'){//if variables are empty(nothing was entered)
			continue;
		}
		*/
		result=checkLogin(user,pass);
		if(result)break;
	}
	return result;
}
int checkLogin(char *username,char *password){
	static EMPLOYEE x;//static keyword prevents x from decaying when function returns
	FILE *filepointer=fopen("employees.dat","rb");
		while(fread(&x,sizeof(EMPLOYEE),1,filepointer)){
			if(strcmp(username,x.username)==0 && strcmp(password,x.password)==0){
				activeUser=&x;//global pointer points to employee that has logged on
				fclose(filepointer);
				return TRUE;
		}
	}
	fclose(filepointer);
	return FALSE;
}
void GetPassword(char *pass,int size){
	int i=0;
	char c;
	char password[size];
	while((c=getch())!='\n' && i+1<size){
		if(c==BACKSPACE){
			if(i>0){
				i--;
				printf("\b \b");//erases the key when backspace key is pressed
			}
		}else{
			password[i++]=c;//assigns c to password[i] then increments i
			printf("*");
		}
	}
	printf("\n");
	password[i]='\0';//stores null character at last index of string
	strncpy(pass,password,size);
}

void AddCompany(){
	int i;
	COMPANY company;
	printLogo("logo2.txt");
	FILE *filepointer=fopen("companies.dat","ab+");
	if(filepointer==NULL){
		printf("Error: File does not exit!\n");
	}else{
		fseek(filepointer,0,SEEK_END);//sets the file position to end of file
		company.idnum=generateID("companies.dat",sizeof(COMPANY));//id number is calculated by finding how many records are in the file and adding 1
		printf("\n\n\n\t\t\t\t\tENTER THE NAME OF THE COMPANY:");
		fgets(company.name,sizeof(company.name)/sizeof(char),stdin);
		getRidofnewline(company.name);
		printf("\n\t\t\t\t\tENTER THE PERSON OF CONTACT FOR THE COMPANY:");
		fgets(company.contactPerson,sizeof(company.contactPerson)/sizeof(char),stdin);
		getRidofnewline(company.contactPerson);//gets rid of newline character at the end of string due to fgets
		printf("\n\t\t\t\t\tENTER THE COMPANY'S EMAIL:");
		fgets(company.email,sizeof(company.email)/sizeof(char),stdin);
		getRidofnewline(company.email);
		printf("\n\t\t\t\t\tENTER THE TELEPHONE NUMBER FOR THE COMPANY:");
		scanf("%d",&company.telephone);
		fwrite(&company,sizeof(COMPANY),1,filepointer);
		printf("\n\t\t\t\t\t\t\tCOMPANY ADDED SUCCESSFULLY.");
		fclose(filepointer);
	}
}
void AddCustomer(){
	CLIENT customer;
	CLIENT *client_pointer;
	client_pointer=&customer;
	int validation;
	FILE *filepointer=fopen("customers.dat","ab+");
	if(filepointer==NULL){
		printf("Error: File does not exit!\n");
	}else{
			customer.idnum=generateID("customers.dat",sizeof(CLIENT));
			printLogo("logo2.txt");
			printf("\n\t\t\t\t\t\tENTER THE PERSON'S NAME:");
			fgets(customer.name,(sizeof(customer.name)/sizeof(char)),stdin);
			getRidofnewline(customer.name);
			chooseID(client_pointer);
			printf("\n\t\t\t\t\tENTER %s's COMPANY CODE:",customer.name);
			scanf("%s",customer.companyCode);
			printf("\n\t\t\t\t\tENTER %s's COMPANY IDENTIFICATION NUMBER:",customer.name);
			scanf("%d",&customer.companyidnum);
			printf("\n\t\t\t\t\tENTER %s's PERSONAL IDENTIFICATION NUMBER:",customer.name);
			scanf("%d",&customer.personalIDnum);
			CheckClientInfo(client_pointer);
			fwrite(&customer,sizeof(CLIENT),1,filepointer);
			fclose(filepointer);
			printf("\n\n\n\t\t\t\t\tCUSTOMER WAS ADDED SUCCESSFULLY.\n\n");
	}
}

void printClientInfo(CLIENT *pointer){
	printf("\n\t\t\t\t\t\tID NUMBER:\t%d\n",pointer->idnum);
	printf("\t\t\t\t\t\tNAME:\t%s\n",pointer->name);
	printf("\t\t\t\t\t\tIDENTIFCATION TYPE:\t%s\n",pointer->idtype);
	printf("\t\t\t\t\t\tCOMPANY CODE:\t%s\n",(strcmp(activeUser->title,"MANAGER")!=0)?"******":pointer->companyCode);
	printf("\t\t\t\t\t\tCOMPANY IDENTIFICATION NUMBER:\t%d\n",pointer->companyidnum);
	printf("\t\t\t\t\t\tPERSONAL IDENTIFICATION NUMBER:\t%d\n\n",pointer->personalIDnum);
}

void CheckClientInfo(CLIENT *pointer){
	int key,i;
	char *list[7]={"         ID NUMBER        ","           NAME           ","  TYPE OF INDENTIFICATION ",
	               "       COMPANY CODE       ","  COMPANY IDENTIFICATION  ","  PERSONAL IDENTIFICATION ","  CONTINUE  "};
	while(TRUE){
		printLogo("logo2.txt");
		printClientInfo(pointer);
		for(i=0;i<7;i++){
			printf("\t\t\t\t\t%d. %s\n",(i+1),list[i]);
		}
		printf("\n\t\t\tPRESS A NUMBER FROM ABOVE NUMBER TO CORRECT THE CORESPONDING INFORMATION\n\t\t\t\t\tOTHERWHISE 7 TO CONTINUE SAVING....");
		printf("\n\n\t\t\t\tENTER HERE:");
		while(TRUE){
			key=getch();
			if(key=='\n')continue;//skips to next iteration when newline is in stdin from previous input
			if(key==49){//ascii key for 1
				printf("\n\n\t\t\t\t\tENTER THE UPDATED ID NUMBER:");
				scanf("%d",&pointer->idnum);
				break;
			}else if(key==50){//ascii key for 2
				printf("\n\n\t\t\t\t\tENTER THE UPDATED NAME:");
				fgets(pointer->name,sizeof(pointer->name)/sizeof(char),stdin);
				getRidofnewline(pointer->name);
				break;
			}else if(key==51){//ascii key for 3
				chooseID(pointer);
				break;
			}else if(key==52){//ascii key for 4
				printf("\n\n\t\t\t\t\tENTER THE UPDATED COMPANY CODE:");
				scanf("%s",pointer->companyCode);
				break;
			}else if(key==53){//ascii key for 5
				printf("\n\n\t\t\t\t\tENTER THE UPDATED COMPANY ID:");
				scanf("%d",&pointer->companyidnum);
				break;
			}else if(key==54){//ascii key for 6
				printf("\n\n\t\t\t\t\tENTER THE UPDATED PERSONAL ID:");
				scanf("%d",&pointer->personalIDnum);
				break;
			}else if(key==55){//ascii key for 7
				break;
			}else if(key>55 || key<49){
				printf("\n\n\t\t\t\tPLEASE ENTER A NUMBER FROM THE LIST:");
			}
		}
		if(key==55)break;
	}
}
void chooseID(CLIENT *pointer){
	char *id_instructions="\n\n\n\t\t\t\t\t\tPLEASE CHOOSE THE TYPE OF INDENTIFICATION PROVIDED\n\n";
	char *idtypes[3]={"       PASSPORT       ","   DRIVER'S LICENSE   ","      NATIONAL ID     "};
	int idtype_indicator;
	idtype_indicator=menu(idtypes,3,"logo2.txt",id_instructions);
			if(idtype_indicator==0){
				strcpy(pointer->idtype,"PASSPORT");
			}else if(idtype_indicator==1){
				strcpy(pointer->idtype,"DRIVER'S LICENCE");
			}else{
				strcpy(pointer->idtype,"NATIONAL ID");
			}
}
void getRidofnewline(char *buffer){
	int length;
	length=strlen(buffer);
	if(length>0 && buffer[length-1]=='\n'){
		buffer[--length]='\0';
	}
}
void AddEmployee(){
	int title_indicator;
	EMPLOYEE employee;
	EMPLOYEE *employee_pointer;
	char confirm_pass[30];
	FILE *filepointer=fopen("employees.dat","ab+");//creates new file if file does not exist
	char *titles[3]={"    MANAGER     ","     CASHIER    "," PUMP ATTENDANT "};
	char *instructions="\n\n\n\t\t\t\t\t\tPLEASE CHOOSE THE TITLE OF THE EMPLOYEE\n\n";
	employee_pointer=&employee;
	title_indicator=menu(titles,3,"logo2.txt",instructions);
	if(title_indicator==0){
		strcpy(employee.title,"MANAGER");
	}else if(title_indicator==1){
		strcpy(employee.title,"CASHIER");
	}else if (title_indicator==2){
		strcpy(employee.title,"PUMP ATTENDANT");
	}
	employee.idnum=generateID("employees.dat",sizeof(EMPLOYEE));
	printf("\n\t\t\t\t\tENTER THE NEW EMPLOYEE'S NAME:");
	fgets(employee.name,sizeof(employee.name)/sizeof(char),stdin);
	getRidofnewline(employee.name);
	printf("\n\t\t\t\t\tWELCOME %s, \n\t\t\t\t\tENTER YOUR USERNAME(NOT YOUR ACTUAL NAME):",employee.name);
	while(TRUE){
		fgets(employee.username,sizeof(employee.username)/sizeof(char),stdin);
		getRidofnewline(employee.username);
		if(checkUsername(employee.username)==TRUE)break;//checks if username already exists
		printf("\n\n\t\t\t\t\tTHAT USERNAME ALREADY EXISTS! ENTER SOMETHING ELSE:");
	}
	while(TRUE){
		printf("\n\t\t\t\t\tENTER YOUR NEW PASSWORD:");
		GetPassword(employee.password,30);
		printf("\n\t\t\t\t\tPLEASE CONFIRM YOUR PASSWORD:");
		GetPassword(confirm_pass,30);
		if(strcmp(employee.password,confirm_pass)==0){//if they match
			break;
		}
		printf("\t\t\t\t\t\tPASSWORDS DO NOT MATCH!\n");
	}
	fwrite(&employee,sizeof(EMPLOYEE),1,filepointer);
	fclose(filepointer);
	printf("\n\n\t\t\t\t\tEMPLOYEE DATA WAS ADDED SUCCESSFULLY.\n\n");
}
int checkUsername(char *username){
	FILE *fp=fopen("employees.dat","rb");
	EMPLOYEE x;
	while(fread(&x,sizeof(EMPLOYEE),1,fp)){
		if(strcmp(x.username,username)==0){//if they match
			return FALSE;//indicating that username already exits
		}
	}
	fclose(fp);
	return TRUE;//username doesn't exist
}
int generateID(char *filename,int size){
	int id;
	FILE *fp=fopen(filename,"rb");
	fseek(fp,0,SEEK_END);//sets file position to end of file
	id=((ftell(fp)/size)+1)*10;//calculates number of records in file and adding 1 then multiplying by 10 to generate new id number
	fclose(fp);
	return id;
}
void display(){
	int entry;
	char *options[4]={"     CUSTOMER      ","     COMPANY       ",
	                  "     EMPLOYEE      "," BACK TO MAIN MENU "};
	char *instructions="\n\n\n\t\t\t\t\t  PLEASE CHOOSE THE TYPE OF DATA YOU WANT TO DISPLAY\n\n\n";
	entry=menu(options,4,"logo2.txt",instructions);
	printFromFile(entry);
}
int printFromFile(int num){
	FILE *fp;
	printLogo("logo2.txt");
	if(num==0){
		CLIENT x;
		fp=fopen("customers.dat","rb");
		if(fp==NULL){
			printf("\n\n\n\t\t\t\t\tOOPS! SEEMS LIKE THERE AREN'T ANY CUSTOMER RECORDS TO DISPLAY.");
			getch();
			return -1;
		}
		while(fread(&x,sizeof(CLIENT),1,fp)){
			printf("\n\n\t\t\t\t\t\t\t\t*ID NUMBER: %d",x.idnum);
			printf("\n\t\t\t\t\t\t\t\t NAME: %s",x.name);
			printf("\n\t\t\t\t\t\t\t\t IDENTIFICATION TYPE: %s",x.idtype);
			printf("\n\t\t\t\t\t\t\t\t COMPANY CODE: %s",(strcmp(activeUser->title,"PUMP ATTENDANT")==0)? "*****":x.companyCode);
			printf("\n\t\t\t\t\t\t\t\t COMPANY IDENTIFICATION: %d",x.companyidnum);
			printf("\n\t\t\t\t\t\t\t\t PERSONAL IDENTIFICATION: %d",x.personalIDnum);
		}
	}else if(num==1){
		COMPANY x;
		fp=fopen("companies.dat","rb");
		if(fp==NULL){
			printf("\n\n\n\t\t\t\t\tOOPS! SEEMS LIKE THERE AREN'T ANY COMPANY RECORDS TO DISPLAY.");
			getch();
			return -1;
		}
		while(fread(&x,sizeof(COMPANY),1,fp)){
			printf("\n\n\t\t\t\t\t\t\t\t*ID NUMBER: %d",x.idnum);
			printf("\n\t\t\t\t\t\t\t\t NAME: %s",x.name);
			printf("\n\t\t\t\t\t\t\t\t EMAIL: %s",x.email);
			printf("\n\t\t\t\t\t\t\t\t PERSON OF CONTACT: %s",x.contactPerson);
			printf("\n\t\t\t\t\t\t\t\t TELEPHONE: 1(876)%d",x.telephone);
		}
	}else if(num==2){
		EMPLOYEE x;
		fp=fopen("employees.dat","rb");
		while(fread(&x,sizeof(EMPLOYEE),1,fp)){
			printf("\n\n\t\t\t\t\t\t\t*ID NUMBER: %d",x.idnum);
			printf("\n\t\t\t\t\t\t\t NAME: %s",x.name);
			printf("\n\t\t\t\t\t\t\t TITLE: %s\n",x.title);
		}
	}
	if(num==0 || num==1 || num==2){
		getch();
	}
	return 0;
}
void getParish(){
	static char parish[15];//static keyword prevents parish from decaying when function ends
	int parish_indicator;
	char *instructions="\n\n\t\t\t\t\tPLEASE SELECT THE PARISH WHERE THIS BRANCH IS LOCATED\n";
	parish_indicator=menu(parishes,14,"logo2.txt",instructions);
	strcpy(parish,parishes[parish_indicator]);
	parish_pointer=parish;
}
void Add(){
	int menu_indicator;
	char *options[4]={"     CUSTOMER      ","     COMPANY       ",
										"     EMPLOYEE      "," BACK TO MAIN MENU "};
	char *instructions="\n\n\n\t\t\t\t\t\tPLEASE SELECT THE TYPE OF RECORD YOU WANT TO ADD\n\n\n";
	if(strcmp(activeUser->title,"MANAGER")!=0){
		printLogo("logo2.txt");
		printf("\n\n\t\t\t\t\t\t\tYOU ARE NOT AUTHORIZED TO MAKE ANY ADDITIONS\n\t\t\t\t\t\t\t\tPLEASE CONSULT MANAGEMENT.\n\n");
	}else{
		menu_indicator=menu(options,4,"logo2.txt",instructions);
		switch(menu_indicator){
			case 0: AddCustomer();
			break;
			case 1: AddCompany();
			break;
			case 2: AddEmployee();
			break;
		}
	}
	if(menu_indicator!=3){
		getch();
		getch();
	}
}
int eagleID(const char *parish){
	int id=0,i;
	for(i=0;i<14;i++){
		if(strcmp(parish,parishes[i])){
			id+=i;
			break;
		}
	}
	id*=10;
	return id;
}
CLIENT *VerifyClient() // VERIFY CLIENT FUNCTION (CHECKS IF CLIENT IS AUTHORIZED)
{
	int personalID;
	char code[15];
	static CLIENT Verify;
	CLIENT *ptr=&Verify;
	FILE *Check;
	Check=fopen("customers.dat","rb");
	if(Check==NULL){
		printLogo("logo2.txt");
		printf("\n\n\t\t\t\t\t\tNO CLIENTS ARE CURRENTLY REGISTERED\n");
		return NULL;
	}
	printLogo("logo2.txt");
	printf("\n\n\t\t\t\t\tCUSTOMER HAS TO BE VERIFIED BEFORE PURCHASES CAN BE MADE.\n\n");
	printf("\n\n\t\t\t\t\tPLEASE ENTER THE COMPANY CODE: ");
	scanf("%s",code);
	printf("\n\n\t\t\t\t\tPLEASE ENTER THE PROVIDED PERSONAL IDENTIFICATION NUMBER: ");
	scanf("%d",&personalID);
	while(fread(&Verify,sizeof(CLIENT),1,Check)){
		if(strcmp(Verify.companyCode,code)==0 && (Verify.personalIDnum==personalID)){
			  fclose(Check);
				return ptr;
		}
	}
	printf("\n\n\t\t\t\t\t\t\tINVALID CREDENTIALS. FAILED.\n");
	getch();
	getch();
	fclose(Check);
	return NULL;
}
void SellPetrol(){
	static CLIENT *ptr;
	ptr=VerifyClient();
	if(ptr!=NULL){
		printf("\n\n\t\t\t\t\t\t\tVERIFICATION SUCCESSFUL.");
		printf("\n\n\t\t\t\t\t\tPRESS ANY KEY TO COMTINUE. . . .");
		getch();
		getch();
		getSaleData(ptr);
	}
	getch();
}
void getSaleData(CLIENT *ptr){
	int entry;
	SALE sale;
	FILE *salefile=fopen("sales.dat","ab");
	FILE *petrolfile;
	char *instructions="\n\n\n\t\t\t\t\t\t   WELCOME TO THE SELL PETROL MENU\n\t\t\t\t\t             USE ARROW KEYS TO NAVIGATE.\n\n\t\t\t\t\t\t    CHOOSE THE DESIRED PETROL TYPE\n\n\n";
	char *options[3]={"         87         ","        90          ",
	                  "      DIESEL        "};
		entry=menu(options,3,"logo2.txt",instructions);
		sale.idnum=generateID("sales.dat",sizeof(SALE));
		sale.EEidnum=eagleID(parish_pointer);
		strcpy(sale.companycode,ptr->companyCode);
		sale.customerID=ptr->personalIDnum;
		switch(entry){
			case 0:strcpy(sale.petroltype,"EIGHTY-SEVEN 87");
			petrolfile=fopen("87.dat","ab");
			break;
			case 1:strcpy(sale.petroltype,"NINETY 90");
			petrolfile=fopen("90.dat","ab");
			break;
			case 2:strcpy(sale.petroltype,"DIESEL");
			petrolfile=fopen("diesel.dat","ab");
			break;
		}
		printf("\n\t\t\t\t\tENTER THE PUMP NUMBER: ");
		scanf("%d",&sale.pumpnum);
		printf("\n\t\t\t\t\tENTER THE COST PER GALLON  FOR %s: ",sale.petroltype);
		while(TRUE){
			scanf("%f",&sale.costPerGal);
			if(sale.costPerGal>0)break;
			printf("\n\t\t\t\t\tPLEASE ENTER SOMETHING VALID! :");
		}
		printf("\n\t\t\t\t\tENTER THE AMOUNT OF PETROL BEING PURCHASED: ");
		while(TRUE){
			scanf("%d",&sale.petrolAmount);
			if(sale.petrolAmount>0)break;
			printf("\n\t\t\t\t\tPLEASE ENTER SOMETHING VALID! : ");
		}
		sale.dos.day=date->day;
		strcpy(sale.dos.month,date->month);
		sale.dos.year=date->year;
		strcpy(sale.customerName,ptr->name);
		sale.value=sale.costPerGal*sale.petrolAmount;
		sale.employeeID=activeUser->idnum;
		strcpy(sale.employeeName,activeUser->name);
		fwrite(&sale,sizeof(SALE),1,petrolfile);
		fwrite(&sale,sizeof(SALE),1,salefile);
		fclose(petrolfile);
		fclose(salefile);
		printLogo("logo2.txt");
		/*
		printf("\n\t\t\t****************************************************************************************");
	  printf("\n\t\t\t-----------------------------EAGLE ENERGY JAMAICA---------------------------------------");
	  printf("\n\t\t\t****************************************************************************************");
		*/
	  printf("\n\n\t\t\t\t\t\t*ID NUMBER: %d",sale.idnum);
	  printf("\n\n\t\t\t\t\t\tEAGLE ENERGY ID NUMBER: %d",sale.EEidnum);
	  printf("\n\n\t\t\t\t\t\tCUSTOMER NAME: %s",sale.customerName);
	  printf("\n\n\t\t\t\t\t\tCUSTOMER COMPANY CODE: %s",sale.companycode);
	  printf("\n\n\t\t\t\t\t\tCUSTOMER ID NUMBER: %d",sale.customerID);
	  printf("\n\n\t\t\t\t\t\tPETROL TYPE PURCHASED: %s",sale.petroltype);
	  printf("\n\n\t\t\t\t\t\tCOST PER GALLON OF PETROL AT TIME OF PURCHASE: $%.2f",sale.costPerGal);
	  printf("\n\n\t\t\t\t\t\tTOTAL AMOUNT OF PETROL PURCHASED: %d",sale.petrolAmount);
	  printf("\n\n\t\t\t\t\t\tVALUE OF SALE: $%.2f",sale.value);
	  printf("\n\n\t\t\t\t\t\tDATE OF SALE: %d/%s/%d",sale.dos.day,sale.dos.month,sale.dos.year);
	  printf("\n\n\t\t\t\t\t\tEMPLOYEE ID: %d",sale.employeeID);
	  printf("\n\n\t\t\t\t\t\tEMPLOYEE NAME: %s",sale.employeeName);
	  printf("\n\n\t\t\t\t\t\tPUMP NUMBER : %d",sale.pumpnum);
	  printf("\n\n\t\t\t************** Thank You For Making It Eagle Energy For Your Fuel Needs****************");
		getch();
		getch();
}
void report(){
	int indicator;
	char *options[4]={"   CREDIT REPORT   ","   PETROL REPORT   ","   CLIENT REPORT   "," BACK TO MAIN MENU "};
	char *instructions="\n\n\n\t\t\t\t\tPLEASE SELECT THE TYPE OF REPORT YOU WANT TO GENERATE\n\n\n";
	while(indicator!=3){
		indicator=menu(options,4,"logo2.txt",instructions);
		switch(indicator){
			case 0: creditRep();
			break;
			case 1:petrolReportMenu();
			break;
			case 2: ClientReport();
			break;
		}
	}
}
void petrolReportMenu(){
	int indicator;
	char month[12];
	FILE *file=fopen("sales.dat","rb");
	char *options[4]={"     87 REPORT     ","     90 REPORT     ","   DIESEL REPORT   ","      GO BACK      "};
	char *instructions="\n\n\n\t\t\t\t\tPLEASE SELECT THE TYPE OF PETROL REPORT YOU WANT TO GENERATE\n\n\n";
	indicator=menu(options,4,"logo2.txt",instructions);
	if(indicator<3){
		if(file==NULL){
			printf("\n\n\n\t\t\t\t\t\tEITHER THERE WAS AN ERROR OPENING FILE OR NO SALES WERE MADE.");
		}else{
			Petrolreport(file,indicator);
		}
	}
}
void Petrolreport(FILE *file,int num){
  SALE sale;
  char *petroltypes[3]={"EIGHTY-SEVEN 87","NINETY 90","DIESEL"};
  float total=0;
  int amountSold=0;
  printLogo("logo2.txt");
  printf(" \n\t\t\t***************************************** %s Report  **************************************",petroltypes[num]);
  printf("\n\n\t\t\t\t\t\t\tTHIS REPORT IS FOR THE MONTH OF %s\n\n\n",date->month);
  printf("|SALE ID\t|EAGLE ENERGY ID\t|COMPANY CODE\t|CUSTOMER ID\t|PETROL NAME\t        |PETROL AMOUNT\t|COST PER GALLON  |VALUE\t|EMPLOYEE ID\n");
  while(fread(&sale,sizeof(SALE),1,file)){
    if(strcmp(sale.dos.month,date->month)==0 && strcmp(sale.petroltype,petroltypes[num])==0){
      total+=sale.value;
      amountSold+=sale.petrolAmount;
      printf(" %d\t\t %d\t\t         %s\t %d\t\t %s\t %d\t         $%0.2f\t   $%0.2f\t %d\n",sale.idnum,sale.EEidnum,(strcmp(activeUser->title,"MANAGER")==0)?sale.companycode:"******",sale.customerID,sale.petroltype,sale.petrolAmount,sale.costPerGal,sale.value,sale.employeeID);
    }
  }
  printf ("\n\n\t\t\t\t\tTHE TOTAL AMOUNT OF PETROL OF TYPE %s SOLD IS: %d GALLONS.\n",petroltypes[num],amountSold);
  printf("\n\t\t\t\t\tTHE TOTAL VALUE OF PETROL SOLD OF TYPE %s IS: $%.2f\n\n",petroltypes[num],total);
  printf("\n\t\t\t\t\t\tPRESS ANY KEY TO CONTINUE . . . . ");
	getch();
	getch();
}
void ClientReport(){
	int id;
	SALE sale;
	FILE *file=fopen("sales.dat","rb");
	int result=FALSE;
	char name[30];
	char month[12];
	int totalAmount=0;
	float totalValue=0;
	printLogo("logo2.txt");
	if(file==NULL){
		printf("\n\n\n\t\t\t\t\t\tEITHER THERE WAS AN ERROR OPENING FILE OR NO SALES WERE MADE.");
	}else{
		printf("\n\n\n\t\t\t\t\t\tENTER THE ID NUMBER OF THE CLIENT: ");
		scanf("%d",&id);
		printLogo("logo2.txt");
		printf("\n\n\n\t\t\t\t\t*********GENERATING REPORT FOR CLIENT WITH ID %d************",id);
		while(fread(&sale,sizeof(SALE),1,file)){
			if(sale.idnum==id && strcmp(sale.dos.month,date->month)==0){
				strcpy(name,sale.customerName);
				printf("\n\n\n\t\t\t\t\tSALE ID: %d",sale.idnum);
				printf("\n\t\t\t\t\tTRANSACTION DATE: %d/%s/%d",sale.dos.day,sale.dos.month,sale.dos.year);
				printf("\n\t\t\t\t\tCLIENT ID NUMBER: %d",sale.customerID);
				printf("\n\t\t\t\t\tNAME OF CLIENT: %s",sale.customerName);
				printf("\n\t\t\t\t\tPETROL TYPE: %s",sale.petroltype);
				printf("\n\t\t\t\t\tAMOUNT OF PETROL PURCHASED: %d GALLONS",sale.petrolAmount);
				printf("\n\t\t\t\t\tCOST PER GALLON: $%.2f",sale.costPerGal);
				printf("\n\t\t\t\t\tVALUE OF SALE: $%.2f",sale.value);
				totalAmount+=sale.petrolAmount;
				totalValue+=sale.value;
				result=TRUE;
			}
		}
		if(result){
			printf("\n\n\t\t\t\tTOTAL AMOUNT OF PETROL PURCHASED BY %s IN THE MONTH OF %s IS: %d GALLONS\n",name,date->month,totalAmount);
			printf("\n\t\t\t\tTOTAL VALUE OF PETROL CREDITED WAS: $%.2f\n",totalValue);
		}else{
			printf("\n\n\t\t\t\t\t\tTHERE ARE NO CLIENTS WITH THAT ID NUMBER!");
		}
		printf("\n\n\t\t\t\t\t\t\tPRESS ANY KEY TO CONTINUE. . .\n");
	}
	getch();
	getch();

}
//
void creditRep(){
	int company_id;
	int NumberOfClients;
	float companyTotal=0,total;
	printLogo("logo2.txt");
	printf("\n\n\n\t\t\t\t\t\tENTER THE ID NUMBER OF THE COMPANY: ");
	scanf("%d",&company_id);
	NumberOfClients=clientcount(company_id);
	int clientIDs[NumberOfClients];
	getID(clientIDs,company_id);
	printf("\n\n\n\t\t\t\t\t*********GENERATING REPORT FOR COMPANY WITH ID %d************",company_id);
	printf("\n\n\n\t\t\t\t\t\tID NUMBER OF CLIENT\t\tCREDIT ACCRUED");
	for(int i=0;i<NumberOfClients;i++){
		total=totalcredit(clientIDs[i]);
		companyTotal+=total;
		printf("\n\n\t\t\t\t\t\t\t%d\t\t\t$%.2f",clientIDs[i],total);
	}
	printf("\n\n\n\t\t\t\t    TOTAL CREDIT OWED BY THE COMPANY FOR THIS MONTH IS CURRENTLY: $%.2f\n",companyTotal);
	printf("\n\n\t\t\t\t\t\t\tPRESS ANY KEY TO CONTINUE. . .\n");
	getch();
	getch();
}
void getID(int clientIDs[],int companyID){
  int i=0;
  FILE *fp=fopen("customers.dat","rb");
  CLIENT client;
  while(fread(&client,sizeof(CLIENT),1,fp)){
    if(client.companyidnum==companyID){
      clientIDs[i++]=client.personalIDnum;
    }
  }
  fclose(fp);
}
int clientcount(int id){
  int count=0;
  FILE *fp=fopen("customers.dat","rb");
  CLIENT client;
  while(fread(&client,sizeof(CLIENT),1,fp)){
    if(client.companyidnum==id){
      count++;
    }
  }
  fclose(fp);
  return count;
}
float totalcredit(int id){
  FILE *fp=fopen("sales.dat","rb");
  SALE sale;
  float total=0;
  while(fread(&sale,sizeof(SALE),1,fp)){
    if(sale.customerID==id && strcmp(sale.dos.month,date->month)==0){
      total+=sale.value;
    }
  }
  fclose(fp);
  return total;
}
//
void searchMain(){
	int indicator;
	int key;
	char *options[5]={"      CLIENT       ","     COMPANY       ","       SALE        ","     EMPLOYEE      "," BACK TO MAIN MENU "};
	char *instructions="\n\n\n\t\t\t\t\t  PLEASE CHOOSE THE TYPE OF RECORD YOU WANT TO SEARCH\n\n\n";
	while(TRUE){
		indicator=menu(options,5,"logo2.txt",instructions);
		if(indicator==4)break;
		printLogo("logo2.txt");
		printf("\n\n\n\t\t\t\t\tENTER THE ID NUMBER FOR THE RECORD YOU ARE SEARCHING FOR:");
		scanf("%d",&key);
		search(key,indicator);
		printf("\n\n\n\n\t\t\t\t\t\t\tPRESS ANY KEY TO CONTINUE. . .");
		getch();
		getch();
	}
}
void update(){
	int indicator;
	char *options[4]={"      CLIENT       ","      COMPANY      ","      EMPLOYEE     "," BACK TO MAIN MENU "};
	char *instructions="\n\n\n\t\t\t\t\t  PLEASE CHOOSE THE TYPE OF RECORD YOU WANT TO UPDATE\n\n\n";
	if(strcmp(activeUser->title,"MANAGER")!=0){
		printLogo("logo2.txt");
		printf("\n\n\t\t\t\t\t\t\tYOU ARE NOT AUTHORIZED TO MAKE ANY UPDATES\n\t\t\t\t\t\t\t\tPLEASE CONSULT MANAGEMENT.\n\n");
	}else{
		indicator=menu(options,4,"logo2.txt",instructions);
		switch(indicator){
			case 0: updateCLI();
			break;
			case 1: updateComp();
			break;
			case 2: updateEMP();
			break;
		}
	}
	getch();
	getch();
}
int search(int key,int num){
  FILE *file;
  if(num==0){
    CLIENT *ptr;
    ptr=malloc(sizeof(CLIENT));
    file=fopen("customers.dat","rb");
    if(file==NULL){
      printf("\n\n\n\t\t\t\t\tOOPS! EITHER THERE AREN'T ANY CLIENTS REGISTERED YET\n\t\t\t\t\t\tOR THERE WAS AN ERROR ACCESSING THE FILE.");
      return FALSE;
    }
    while(fread(ptr,sizeof(CLIENT),1,file)){
      if(ptr->idnum==key){
        printf("\n\n\n\t\t\t\t\t\t\t      RECORD WAS FOUND!");
        printClientInfo(ptr);
        return TRUE;
      }
    }
  }else if(num==1){
    COMPANY *ptr;
    ptr=malloc(sizeof(COMPANY));
    file=fopen("companies.dat","rb");
    if(file==NULL){
      printf("\n\n\n\t\t\t\t\tOOPS! EITHER THERE AREN'T ANY COMPANIES REGISTERED YET\n\t\t\t\t\t\tOR THERE WAS AN ERROR ACCESSING THE FILE.");
      return FALSE;
    }
    while(fread(ptr,sizeof(COMPANY),1,file)){
      if(ptr->idnum==key){
				printf("\n\n\n\t\t\t\t\t\t\t      RECORD WAS FOUND!");
      	printComp(ptr);
        return TRUE;
      }
    }
  }else if(num==2){
    SALE *ptr;
    ptr=malloc(sizeof(SALE));
    file=fopen("sales.dat","rb");
    if(file==NULL){
      printf("\n\n\n\t\t\t\t\tOOPS! EITHER THERE AREN'T ANY SALE RECORDS(NO SALES WERE MADE)\n\t\t\t\t\t\tOR THERE WAS AN ERROR ACCESSING THE FILE.");
      return FALSE;
    }
    while(fread(ptr,sizeof(SALE),1,file)){
      if(ptr->idnum==key){
        printf("\n\n\n\t\t\t\t\t\t      RECORD WAS FOUND!");
        printf("\n\n\t\t\t\t\t\t*ID NUMBER: %d",ptr->idnum);
        printf("\n\t\t\t\t\t\tEAGLE ENERGY ID NUMBER: %d",ptr->EEidnum);
        printf("\n\t\t\t\t\t\tCUSTOMER NAME: %s",ptr->customerName);
        printf("\n\t\t\t\t\t\tCUSTOMER COMPANY CODE: %s",(strcmp(activeUser->title,"PUMP ATTENDANT")==0)? "*****":ptr->companycode);
        printf("\n\t\t\t\t\t\tCUSTOMER ID NUMBER: %d",ptr->customerID);
        printf("\n\t\t\t\t\t\tPETROL TYPE PURCHASED: %s",ptr->petroltype);
        printf("\n\t\t\t\t\t\tCOST PER GALLON OF PETROL AT TIME OF PURCHASE: $%.2f",ptr->costPerGal);
        printf("\n\t\t\t\t\t\tTOTAL AMOUNT OF PETROL PURCHASED: %d",ptr->petrolAmount);
        printf("\n\t\t\t\t\t\tVALUE OF SALE: $%.2f",ptr->value);
      	printf("\n\t\t\t\t\t\tDATE OF SALE: %d/%s/%d",ptr->dos.day,ptr->dos.month,ptr->dos.year);
        printf("\n\t\t\t\t\t\tEMPLOYEE ID: %d",ptr->employeeID);
        printf("\n\t\t\t\t\t\tEMPLOYEE NAME: %s",ptr->employeeName);
        printf("\n\t\t\t\t\t\tPUMP NUMBER : %d",ptr->pumpnum);
        return TRUE;
      }
    }
  }else if(num==3){
    EMPLOYEE *ptr;
    ptr=malloc(sizeof(EMPLOYEE));
    file=fopen("employees.dat","rb");
    while(fread(ptr,sizeof(EMPLOYEE),1,file)){
      if(ptr->idnum==key){
        printf("\n\n\n\t\t\t\t\t\t\t      RECORD WAS FOUND!");
        printEmp(ptr);
        return TRUE;
      }
    }
  }
  printf("\n\n\t\t\t\t\t\tTHAT ID NUMBER DOES NOT EXIST! PLEASE TRY AGAIN");
  return FALSE;
}
void printComp(COMPANY *ptr){
  printf("\n\n\t\t\t\t\t\t\t*ID NUMBER: %d",ptr->idnum);
  printf("\n\t\t\t\t\t\t\t NAME: %s",ptr->name);
  printf("\n\t\t\t\t\t\t\t EMAIL: %s",ptr->email);
  printf("\n\t\t\t\t\t\t\t PERSON OF CONTACT: %s",ptr->contactPerson);
  printf("\n\t\t\t\t\t\t\t TELEPHONE: 1(876)%d\n\n",ptr->telephone);
}
void updateComp(){
  int id,found=FALSE;
  int len,key,i;
  COMPANY *ptr;
  COMPANY company;
  ptr=&company;
  char *options[6]={" ID NUMBER "," NAME "," EMAIL "," PERSON OF CONTACT "," TELEPHONE "," CONTINUE "};
  FILE *file=fopen("companies.dat","rb+");
  printLogo("logo2.txt");
  printf("\n\n\n\t\t\t\t\tENTER THE COMPANY'S ID NUMBER: ");
  scanf("%d",&id);
  while(fread(&company,sizeof(COMPANY),1,file)){
    if(company.idnum==id){
      found=TRUE;
      len=ftell(file);
      break;
    }
  }
  if(found){
    while(TRUE){
      printLogo("logo2.txt");
      printComp(ptr);
      for(i=0;i<6;i++){
        printf("\t\t\t\t\t\t\t%d. %s\n",(i+1),options[i]);
      }
      printf("\n\t\t\tPRESS A NUMBER FROM ABOVE NUMBER TO CORRECT THE CORESPONDING INFORMATION\n\t\t\t\t\tOTHERWHISE 6 TO CONTINUE SAVING....");
      printf("\n\n\t\t\t\tENTER HERE:");
			getch();
      key=getch();
      if(key==54)break;
      switch(key){
        case 49:
        printf("\n\n\t\t\t\t\tENTER THE UPDATED ID NUMBER:");
        scanf("%d",&ptr->idnum);
        break;
        case 50:
        printf("\n\n\t\t\t\t\tENTER THE UPDATED NAME:");
        fgets(ptr->name,sizeof(ptr->name)/sizeof(char),stdin);
        getRidofnewline(ptr->name);
        break;
        case 51:
        printf("\n\n\t\t\t\t\tENTER THE UPDATED EMAIL: ");
        fgets(ptr->email,sizeof(ptr->email)/sizeof(char),stdin);
        getRidofnewline(ptr->email);
        break;
        case 52:
        printf("\n\n\t\t\t\t\tENTER THE UPDATED PERSON OF CONTACT:");
        fgets(ptr->contactPerson,sizeof(ptr->contactPerson)/sizeof(char),stdin);
        getRidofnewline(ptr->contactPerson);
        break;
        case 53:
        printf("\n\n\t\t\t\t\tENTER THE NEW TELEPHONE NUMBER: (876)");
        scanf("%d",&ptr->telephone);
        break;
        default:
        printf("\n\n\t\t\t\tPLEASE ENTER A NUMBER FROM THE LIST!");
      }
    }
    fseek(file,len-sizeof(COMPANY),SEEK_SET);
    fwrite(&company,sizeof(COMPANY),1,file);
    fclose(file);
    printf("\n\n\t\t\t\t\tCOMPANY RECORD WAS UPDATED SUCCESFULLY. . .");
  }else{
    printf("\n\n\t\t\t\t\tTHAT ID NUMBER DOES NOT BELONG TO ANYONE!");
  }
}
void updateEMP(){
  int id,found=FALSE;
  int key,i,len;
  char pass[30];
  char *options[6]={" ID NUMBER "," NAME "," TITLE "," USERNAME "," PASSWORD "," CONTINUE"};
  EMPLOYEE *emp;
  EMPLOYEE employee;
  emp=&employee;
  FILE *file=fopen("employees.dat","rb+");
  printLogo("logo2.txt");
  printf("\n\n\n\t\t\t\t\tENTER THE EMPLOYEE'S ID NUMBER: ");
	scanf("%d",&id);
  while(fread(&employee,sizeof(EMPLOYEE),1,file)){
    if(employee.idnum==id){
      found=TRUE;
      len=ftell(file);
      break;
    }
  }
  if(found){
    while(TRUE){
      printLogo("logo2.txt");
      printEmp(emp);
      for(i=0;i<6;i++){
        printf("\t\t\t\t\t\t%d. %s\n",(i+1),options[i]);
      }
      printf("\n\t\t\tPRESS A NUMBER FROM ABOVE NUMBER TO CORRECT THE CORESPONDING INFORMATION\n\t\t\t\t\tOTHERWHISE 6 TO CONTINUE SAVING....");
      printf("\n\n\t\t\t\tENTER HERE:");
			getch();
        key=getch();
				  if(key==54)break;
        switch(key){
          case 49:
          printf("\n\n\t\t\t\t\tENTER THE UPDATED ID NUMBER:");
          scanf("%d",&emp->idnum);
          break;
          case 50:
          printf("\n\n\t\t\t\t\tENTER THE UPDATED NAME:");
          fgets(emp->name,sizeof(emp->name)/sizeof(char),stdin);
          getRidofnewline(emp->name);
          break;
          case 51:
          printf("\n\n\t\t\t\t\tENTER THE UPDATED TITLE:");
          fgets(emp->title,sizeof(emp->title)/sizeof(char),stdin);
          getRidofnewline(emp->title);
          break;
          case 52:
          printf("\n\n\t\t\t\t\tENTER THE UPDATED USERNAME:");
          fgets(emp->username,sizeof(emp->username)/sizeof(char),stdin);
          getRidofnewline(emp->username);
          break;
          case 53:
          while(TRUE){
            printf("\n\n\t\t\t\t\tENTER THE NEW PASSWORD:");
            GetPassword(emp->password,30);
            printf("\n\t\t\t\t\tPLEASE CONFIRM NEW PASSWORD:");
            GetPassword(pass,30);
            if(strcmp(pass,emp->password)==0)break;
            printf("\n\t\t\t\t\tPASSWORDS DO NOT MATCH!");
          }
					break;
					default:
					printf("\n\n\t\t\t\tPLEASE ENTER A NUMBER FROM THE LIST!");
        }
    }
		fseek(file,len-sizeof(EMPLOYEE),SEEK_SET);
		fwrite(&employee,sizeof(EMPLOYEE),1,file);
		fclose(file);
		printf("\n\n\t\t\t\t\tEMPLOYEE RECORD WAS UPDATED SUCCESFULLY. . .");
  }else{
    printf("\n\n\t\t\t\t\tTHAT ID NUMBER DOES NOT BELONG TO ANYONE!");
  }
}
void printEmp(EMPLOYEE *emp){
  printf("\n\n\t\t\t\t\t\tID NUMBER: %d",emp->idnum);
  printf("\n\t\t\t\t\t\tNAME: %s",emp->name);
  printf("\n\t\t\t\t\t\tTITLE: %s",emp->title);
  printf("\n\t\t\t\t\t\tUSERNAME: %s",emp->username);
  printf("\n\t\t\t\t\t\tPASSWORD: **********\n\n");
}
void updateCLI(){
  int id,found=FALSE;
  int len;
  CLIENT *cli;
  CLIENT client;
  FILE *file=fopen("customers.dat","rb+");
  cli=&client;
  printf("\n\n\n\t\t\t\t\tENTER THE CLIENT'S ID NUMBER: ");
  scanf("%d",&id);
  while(fread(&client,sizeof(CLIENT),1,file)){
    if(client.idnum==id){
      len=ftell(file);
      found=TRUE;
      break;
    }
  }
  if(found){
    CheckClientInfo(cli);
    fseek(file,len-sizeof(CLIENT),SEEK_SET);
    fwrite(&client,sizeof(CLIENT),1,file);
    fclose(file);
    printf("\n\n\t\t\t\t\tCLIENT RECORD WAS UPDATED SUCCESSFULLY. . .");
  }else{
    printf("\n\n\t\t\t\t\tTHAT ID NUMBER DOES NOT BELONG TO ANYONE!");
  }
}
int getch(void)//alternative to getch for linux
{
 int ch;
 struct termios oldt;
 struct termios newt;
 tcgetattr(STDIN_FILENO, &oldt); /*store old settings */
 newt = oldt; /* copy old settings to new settings */
 newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */
 tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */
 ch = getchar(); /* standard getchar call */
 tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */
 return ch; /*return received char */
}
/////////////////////////////////////////////
//CALCULATOR
void Calculator(){
	int option;
	float a, b,res;
	result=&res;
	char *options[7]={"   ADD    "," SUBTRACT "," MULTIPLY ","  DIVIDE  "," EXPONENT ","  CLEAR  "," MAIN MENU"};
	Calc_arrow(options,-1,7);
	printf("[");
	scanf("%f",&a);
	res=a;
	while(TRUE){
		getch();
		option=Calc_menu(options,7);
		if(option==6)break;
		if(option!=5 && option!=6){
			scanf("%f",&b);
		}
		switch(option){
			case 0:res=sum(res,b);
			break;
			case 1:res=diff(res,b);
			break;
			case 2:res=mult(res,b);
			break;
			case 3:res=divide(res,b);
			break;
			case 4:res=pow(res,b);
			break;
			case 5:	Calc_arrow(options,-1,7);
					printf("[");
					scanf("%f",&a);
					res=a;
					continue;
		}
		printf("\n\n\n\t\t\t\tAnswer: %.5f\n",*result);
		getch();
	}
}
float sum(float a, float b){
	float result;
	result=a+b;
	return result;
}
float diff(float a,float b){
	float difference;
	difference=a-b;
	return difference;
}
float divide(float a, float b){
	float result;
	result=a/b;
	return result;
}
float mult(float a,float b){
	float result;
	result=a*b;
	return result;
}
int Calc_menu(char **list,int size){
	int key,count=0;
	Calc_arrow(list,0,size);
	while(key!='\n'){
		key=getch();
		if(key==DOWN){
			if(count<(size-1)){
				count++;
			}
		}else if(key==UP && count>0){
			count--;
		}
		Calc_arrow(list,count,size);
	}
	return count;
}
void Calc_arrow(char **list,int x,int size){
	int i;
	printLogo("calc.txt");
	printf("\n\t\t\t\t\t WELCOME USER, ENTER A VALUE AND CHOOSE THE DESIRED OPERATOR\n\n\n");
	for(i=0;i<size;i++){
		printf("\t\t\t\t\t\t\t%s[%s]%s\n\n",(x==i)?"-->":"   ",list[i],(x==i)?"<--":"   ");
	}
	printf("\n\t\t\t\t");
	switch(x){
		case 0:printf("[%.2f + ",*result);
		break;
		case 1:printf("[%.2f - ",*result);
		break;
		case 2:printf("[%.2f x ",*result);
		break;
		case 3:printf("[%.2f / ",*result);
		break;
		case 4:printf("[%.2f ^ ",*result);
		break;
		case 5:printf("[");
	}
	printf("\n\t\t\t\t");
}
///////////////////////////////////////////////////////
