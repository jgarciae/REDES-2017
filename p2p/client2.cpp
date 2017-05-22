// g++ -std=c++11 -pthread -o client client.cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

char buffer_send[256];
char buffer_recieved[256];
char buffer_opt[4];
char new_buffer_send[256];
string server_message;
int t_port;
int t_name;

void actions(int SocketFD)
{
    int n=0;
    bzero(buffer_opt,4);
    bzero(buffer_recieved,256);
    bzero(buffer_send,256);
    int opt;

    do{
        cout<<"Commandos:"<<endl;
        cout<<"  1 Login"<<endl;
        cout<<"  2 Register"<<endl;
        cout<<"  3 Exit"<<endl;

        cout<<"Numero: ";
        cin>>opt;
        cout<<endl;
    }while(opt>3);

    buffer_opt[0]=(char)opt;

    n = write(SocketFD,buffer_opt,4); //numero de pedido
    if (n < 0){
        perror("ERROR escribiendo socket");
        return;
    }

    n = read(SocketFD,buffer_opt,4); //confirma
    if (n < 0){
        perror("ERROR escribiendo socket");
        return;
    }
    cout<<"Confirmado"<<endl<<endl;

    switch(opt)
    {
        case 1:
            cout<<"Logearse: ";
            cin>>buffer_send;
            cout<<endl;
            n = write(SocketFD,buffer_send,256); //manda nombre de usuario
            if (n < 0){
                perror("ERROR escribiendo socket");
                return;
            }

            n = read(SocketFD,buffer_recieved,256); //confirma
            if (n < 0){
                perror("ERROR escribiendo socket");
                return;
            }

            do{
                cout<<"Commandos:"<<endl;
                cout<<"  1 Logout"<<endl;
                cout<<"  2 Get"<<endl;
                cout<<"  3 Put"<<endl;
                cout<<"  4 Jugar"<<endl;

                cout<<"Numero: ";
                cin>>opt;
                cout<<endl;
            }while(opt>4);

            buffer_opt[0]=(char)opt;
            n = write(SocketFD,buffer_opt,4); //numero de pedido
            if (n < 0){
                perror("ERROR escribiendo socket");
                return;
            }

            n = read(SocketFD,buffer_opt,4); //confirma
            if (n < 0){
                perror("ERROR escribiendo socket");
                return;
            }

            switch(opt)
            {
                case 1:
                    actions(SocketFD);
                    return;
                    break;

                case 2:
                    //get
                    break;

                case 3:
                    //put
                    break;
            }

            break;

        case 2:
            cout<<"Registrar: ";
            cin>>buffer_send;
            cout << buffer_send;
            cout<<endl;
            cout <<"ens "<<buffer_send[0]<<endl;
            t_port = (int)buffer_send[0];
            t_name = (int)buffer_send[1];
            cout <<"port : "<<t_port<<endl;
            n = write(SocketFD,buffer_send,1256); //nuevo nombre de usuario
            if (n < 0){
                perror("ERROR escribiendo socket");
                return;
            }

            n = read(SocketFD,buffer_recieved,256); //confirma
            for(int i=0; i<23; i++){
              server_message[i] += buffer_recieved[i];
            }
            cout<<server_message<<endl;
            if(strcmpn (buffer_recieved,"Ingresa otro usuario")){

                cout <<"mensaje de servidor : "<<"go"<<endl;
                actions(SocketFD);

                break;
            }
            if (n < 0){
                perror("ERROR escribiendo socket");
                return;
            }

            actions(SocketFD);
            return;
            break;

        case 3:
            cout<<"Closed"<<endl;
            shutdown(SocketFD, SHUT_RDWR);
            close(SocketFD);
            break;
    }
}


int main(void)
{
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;

    if (-1 == SocketFD)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

    if (0 > Res)
    {
        perror("error: first parameter is not a valid address family");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
        perror("char string (second parameter does not contain valid ipaddress");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }


    actions(SocketFD);


    /*shutdown(SocketFD, SHUT_RDWR);

    close(SocketFD);*/

    return 0;
}
