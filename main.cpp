#include <iostream>
#include<windows.h>
#include<mysql.h>
#include<sstream>
#include<stdlib.h>
#include<string>
#include<bits/stdc++.h>

using namespace std;
HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);

class users{
    string user,pass;
    public:
    void putuser(string u){
        user=u;
    }
    void putpass(string p){
        pass=p;
    }
    string getusern(){
        return user;
    }
    string getpass(){
        return pass;
    }
};

class db{
    const char* hostname="localhost";
    const char* username="root";
    const char* password="";
    const char* database="mars";
    unsigned int port =3306;
    const char* unixsocket=NULL;
    unsigned clientflag=0;

    public:
    MYSQL* connecteddatabase(){
        MYSQL* conn;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn,hostname,username,password,database,port,unixsocket,clientflag);
        if(conn){
            cout<<"Connected"<<"   "<<conn<<endl;
            system("pause");
            return conn;
        }
        else{
            cout<<"Not Connected"<<"   "<<conn<<endl;
            system("pause");
            return conn;
        }
    }
};

users u;

bool testusername(MYSQL* conn, string un){
    stringstream ss;
    MYSQL_ROW row;
    MYSQL_RES* res;
    ss<<"SELECT * FROM users WHERE username='"+un+"' ";
    string query=ss.str();
    const char* q=query.c_str();
    mysql_query(conn,q);
    res=mysql_store_result(conn);
    if(row=mysql_fetch_row(res)){
        return true;
    }
    return false;
}

string itos(int num){
    ostringstream strg;
    strg<< num;
    string s1 = strg.str();
    return s1;
}

void createuser(MYSQL* conn){
    int qstate;
    string s,p,cp,phno;
    label:
    stringstream ss,usercheck;
    system("cls");
    SetConsoleTextAttribute(h,2);
    cout<<"Enter username: "<<endl;
    SetConsoleTextAttribute(h,6);
    cin>>s;
    if(testusername(conn,s)){
        cout<<"Username already exists! \nTry a different username"<<endl;
        system("pause");
        system("cls");
        createuser(conn);
    }
    else{
    SetConsoleTextAttribute(h,2);
    cout<<"Enter Password: "<<endl;
    SetConsoleTextAttribute(h,6);
    cin>>p;
    SetConsoleTextAttribute(h,2);
    cout<<"Confirm Password: "<<endl;
    SetConsoleTextAttribute(h,6);
    cin>>cp;
    SetConsoleTextAttribute(h,2);
    cout<<"Phone number: "<<endl;
    SetConsoleTextAttribute(h,6);
    cin>>phno;
    if(p==cp){
        ss<<"INSERT INTO users(username,password,phno) VALUES ('"+s+"','"+p+"','"+phno+"')";
        string query=ss.str();
        const char* q=query.c_str();
        qstate=mysql_query(conn,q);
        if(qstate==0){
            SetConsoleTextAttribute(h,12);
            cout<<"Created!\nYou have received 100/- Welcome bonus"<<endl;
            //user=s;
            u.putuser(s);
            //pass=p;
            u.putpass(p);
            stringstream ss1;
            ss1<<"INSERT INTO stocks(username,item1,item2,item3,item4,item5,item6,item7,item8,item9,item10,password,balance) VALUES ('"+s+"',0,0,0,0,0,0,0,0,0,0,'"+p+"',100)";
            string query=ss1.str();
            const char* q=query.c_str();
            mysql_query(conn,q);
            system("pause");
//            return true;
        }
    }
    else{
        cout<<"Passwords doesnt match!"<<endl;
        system("pause");
        goto label;
    }
//    return false;
    }
}

bool checkup(MYSQL* conn, string un,string pw){
    stringstream ss;
    MYSQL_ROW row;
    MYSQL_RES* res;
    ss<<"SELECT * FROM users WHERE username='"+un+"' AND password='"+pw+"' ";
    string query=ss.str();
    const char* q=query.c_str();
    mysql_query(conn,q);
    res=mysql_store_result(conn);
    if(row=mysql_fetch_row(res)){
        return true;
    }
    return false;
}

bool getuser(MYSQL* conn){
    string user,pass;
    system("cls");
    SetConsoleTextAttribute(h,2);
    cout<<"Type username:"<<endl;
    SetConsoleTextAttribute(h,6);
    cin>>user;
    SetConsoleTextAttribute(h,2);
    cout<<"Enter Password: "<<endl;
    SetConsoleTextAttribute(h,6);
    cin>>pass;
    if(!checkup(conn,user,pass)){
        cout<<"User name or password doesn't match!"<<endl;
        system("pause");
        return false;
    }
    u.putuser(user);
    u.putpass(pass);
    return true;
}

void displaystocks(MYSQL* conn){
    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate=mysql_query(conn,"SELECT * FROM cost");
    if(!qstate){
        res=mysql_store_result(conn);
        cout<<"ItemName\t\tcost\n";
        cout<<"----------------------------------------"<<endl;
        while(row=mysql_fetch_row(res)){
            cout<<row[0]<<"\t\t\t"<<row[1]<<endl;
        }
        cout<<"----------------------------------------\n"<<endl;
    }
}

void displayuserinfo(MYSQL* conn){
    stringstream ss;
    string user=u.getusern();
    string pass=u.getpass();
    ss<<"SELECT item1,item2,item3,item4,item5,item6,item7,item8,item9,item10,balance FROM stocks WHERE username='"+user+"' AND password='"+pass+"' ";
    string query=ss.str();
    const char* q=query.c_str();
    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate=mysql_query(conn,q);
    if(!qstate){
        res=mysql_store_result(conn);
        row=mysql_fetch_row(res);
        cout<<"Item\t\t\tNo.\n";
        cout<<"----------------------------------------"<<endl;
        for(int i=0;i<10;i++){
            cout<<"item"<<i+1<<"\t:\t\t"<<row[i]<<endl;
        }
        cout<<"Balance: "<<row[10]<<endl;
        cout<<"----------------------------------------\n"<<endl;
    }
    else{
        cout<<"Error printing user stocks"<<endl;
    }
}

void invest(MYSQL* conn){
    int c,n;
    string user=u.getusern();
    string pass=u.getpass();
    cout<<"Stocks Available: "<<endl;
    cout<<"-----------------"<<endl;
    displaystocks(conn);
    cout<<"User's Stocks: "<<endl;
    cout<<"-------------"<<endl;
    displayuserinfo(conn);
    cout<<"Type on which item to invest: "<<endl;
    cin>>c;
    cout<<"Type quantity: "<<endl;
    cin>>n;
    stringstream ss;
    ss<<"SELECT balance FROM stocks WHERE username='"+user+"' AND password='"+pass+"' ";
    string query=ss.str();
    const char* q=query.c_str();
    MYSQL_ROW row;
    MYSQL_RES* res;
    mysql_query(conn,q);
    res=mysql_store_result(conn);
    row=mysql_fetch_row(res);
    int balance=atoi(row[0]);

    stringstream ss1;
    ss1<<"SELECT cost FROM cost WHERE itemname='item"+itos(c)+"' ";
    query=ss1.str();
    q=query.c_str();
//    cout<<q<<endl;
    mysql_query(conn,q);
    res=mysql_store_result(conn);
    row=mysql_fetch_row(res);
    int cost=n*atoi(row[0]);
//    cout<<row[0];

    if(balance-cost>=0){
        stringstream ss1;
        int newbalance=balance-cost;
        ss1<<"UPDATE stocks SET item"+itos(c)+"=item"+itos(c)+"+"+itos(n)+", balance="+itos(newbalance)+" WHERE username='"+user+"' AND password='"+pass+"' ";
        string query=ss1.str();
        q=query.c_str();
//        cout<<q<<endl;
        int querystatus=mysql_query(conn,q);
        if(!querystatus){
            cout<<"Transaction Successful!"<<endl;
            system("pause");
            system("cls");
        }
    }
    else{
        cout<<"Insufficient balance!"<<endl;
        system("pause");
        system("cls");
    }
}

void sell(MYSQL* conn){
    int i,n;
    string user=u.getusern();
    string pass=u.getpass();
    cout<<"Stocks Available: "<<endl;
    cout<<"----------------"<<endl;
    displaystocks(conn);
    cout<<"User's Stocks: "<<endl;
    cout<<"-------------"<<endl;
    displayuserinfo(conn);

    cout<<"Type item number to sell: "<<endl;
    cin>>i;
    cout<<"Type quantity: "<<endl;
    cin>>n;

    stringstream ss;
    ss<<"SELECT item"+itos(i)+",balance FROM stocks WHERE username='"+user+"' AND password='"+pass+"' ";
    string query=ss.str();
    const char* q=query.c_str();
    MYSQL_ROW row;
    MYSQL_RES* res;
//    int qstate=
    mysql_query(conn,q);
    res=mysql_store_result(conn);
    row=mysql_fetch_row(res);
    int ai=atoi(row[0]);
    int balance=atoi(row[1]);
    if(n<=ai){
        stringstream ss1,ss2;
        ss1<<"SELECT cost FROM cost WHERE itemname='item"+itos(i)+"' ";
        query=ss1.str();
        q=query.c_str();
//        cout<<q<<endl;
        MYSQL_ROW row;
        MYSQL_RES* res;
        mysql_query(conn,q);
        res=mysql_store_result(conn);
        row=mysql_fetch_row(res);
        int ic=atoi(row[0]);
        int newbalance=balance+n*ic;
        ai=ai-n;
        ss2<<"UPDATE stocks SET item"+itos(i)+"="+itos(ai)+",balance="+itos(newbalance)+" WHERE username='"+user+"' AND password='"+pass+"' ";
        query=ss2.str();
        q=query.c_str();
//        cout<<q<<endl;
        mysql_query(conn,q);
        cout<<"Transaction successful!"<<endl;
        system("pause");
    }
    else{
        cout<<"Insufficient No. of stocks"<<endl;
        system("pause");
    }
}

void selectshares(MYSQL* conn){
    cout<<"User's Stocks: "<<endl;
    displayuserinfo(conn);
    system("pause");
    system("cls");
}

void deposit(MYSQL* conn){
    int i;
    string user=u.getusern();
    string pass=u.getpass();
    cout<<"Type amount to be deposited: "<<endl;
    cin>>i;
    stringstream ss;
    ss<<"SELECT balance FROM stocks WHERE username='"+user+"' AND password='"+pass+"' ";
    string query=ss.str();
    const char* q=query.c_str();
    MYSQL_ROW row;
    MYSQL_RES* res;
    mysql_query(conn,q);
    res=mysql_store_result(conn);
    row=mysql_fetch_row(res);
    int newbalance=atoi(row[0])+i;
    stringstream ss2;
    ss2<<"UPDATE stocks SET balance="+itos(newbalance)+" WHERE username='"+user+"' AND password='"+pass+"' ";
    query=ss2.str();
    q=query.c_str();
    mysql_query(conn,q);
    cout<<"Transaction Successful!"<<endl;
    system("pause");
}

int main(){
    db connect;
    SetConsoleTextAttribute(h,13);
    int c;
    MYSQL* conn=connect.connecteddatabase();
    loginpage:
    while(1){
        system("cls");
        SetConsoleTextAttribute(h,6);
        cout<<"\n\n**********************************************************"<<endl;
        cout<<"|||=======================o0o==========================|||"<<endl;
        cout<<"|||                                                    |||"<<endl;
        cout<<"|||                WELCOME TO MARS                     |||"<<endl;
        cout<<"|||                                                    |||"<<endl;
        cout<<"|||=======================o0o==========================|||"<<endl;
        cout<<"**********************************************************"<<endl;
        cout<<"\n 1)Login \n\n Didn't have a account? \n 2)Sign-up\n 3)Exit\n";
        cout<<"----------------------------------------------------------"<<endl;
        cin>>c;
        system("cls");
        if(c==1){
            if(getuser(conn))
                goto account;
        }
        else if(c==2){
            createuser(conn);
                goto account;
        }
        else if(c==3){
            exit(1);
        }
        else{
            cout<<"Invalid option"<<endl;
            system("pause");
            system("cls");
            continue;
        }
    }

    account:
    while(1){
        SetConsoleTextAttribute(h,3);
        system("cls");
        cout<<"\n\n*****************************************************************"<<endl;
        cout<<"*******                                                   *******"<<endl;
        cout<<"*******   1)Invest 2)Check A/C 3)Sell 4)Deposit 5)exit    *******"<<endl;
        cout<<"*******                                                   *******"<<endl;
        cout<<"*****************************************************************"<<endl;
        cin>>c;
        SetConsoleTextAttribute(h,4);
        if(c==1){
            system("cls");
            invest(conn);
        }
        else if(c==2){
            system("cls");
            selectshares(conn);
        }
        else if(c==3){
            system("cls");
            sell(conn);
        }
        else if(c==4){
            system("cls");
            deposit(conn);
        }
        else if(c==5){
            goto loginpage;
//            exit(1);
        }
        else{
            system("cls");
            cout<<"Invalid option"<<endl;
            system("pause");
            system("cls");
            continue;
        }
    }
    return 0;
}
