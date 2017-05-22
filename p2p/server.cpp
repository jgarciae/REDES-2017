// g++ -std=c++11 -pthread -o server server.cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>       
#include <thread>        
#include <chrono>         
#include <string>
#include <sstream>
#include <cstdlib>
#include <map>


using namespace std;


char buffer_send[256];
char buffer_recieved[256];
char buffer_opt[4];

map<string,string> data;

void print_data()
{
    cout<<"Datos completos"<<endl;
    //for (map<char,int>::iterator it=data.begin(); it!=data.end(); ++it)
    //    cout << it->first << " => " << it->second << '\n';
}

void action(int socketP) 
{
    int n=0;

    string user;
    
    bzero(buffer_opt,4);
    bzero(buffer_recieved,256);
    bzero(buffer_send,256);
    int opt;
    
    //usuario selecciona numero

    n = read(socketP,buffer_opt,4);
    if (n < 0) 
        perror("ERROR leyendo socket");
    opt=(int)buffer_opt[0];
    cout<<"Opcion recibida: "<<opt<<endl;

    n = write(socketP,buffer_opt,4);
    if (n < 0) 
        perror("ERROR escribiendo socket");

    switch(opt)
    {
        case 1:
            bzero(buffer_recieved,256);
            n = read(socketP,buffer_recieved,256);
            if (n < 0) 
                perror("ERROR leyendo socket");
            user=buffer_recieved;
            cout<<"Entrando como "<<user<<endl;

            bzero(buffer_send,256);
            n = write(socketP,buffer_send,256);
            if (n < 0){
                perror("ERROR escribiendo socket");
                return;
            }

            //usuario selecciona numero

            n = read(socketP,buffer_opt,4); //recibe pedido
            if (n < 0) 
                perror("ERROR leyendo socket");
            opt=(int)buffer_opt[0];
            cout<<"Opcion recibida por "<<user<<": "<<opt<<endl;

            n = write(socketP,buffer_opt,4);
            if (n < 0) 
                perror("ERROR escribiendo socket");

            switch(opt) //opciones seleccionadas
            {
                case 1:
                    cout<<"Saliendo como "<<user<<endl;
                    user="";
                    action(socketP);
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
            n = read(socketP,buffer_recieved,256);
            if (n < 0) 
                perror("ERROR leyendo socket");
            /*string buff(buffer_recieved);
            string tmp = " ";
            data.insert(pair<string,string>(buff,tmp));*/
            print_data();
            n = write(socketP,buffer_send,256); //nuevo nombre de usuario
            if (n < 0){
                perror("ERROR escribiendo socket");
                return;
            }
            action(socketP);
            break;

        case 3:
            shutdown(socketP, SHUT_RDWR);
            close(socketP);
            break;
    }



    
}

 
int main(void)
{
    struct sockaddr_in stSockAddr, client_addr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int n;
 
    if(-1 == SocketFD)
    {
        perror("can not create socket");
        exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("error bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
 
    if(-1 == listen(SocketFD, 10))
    {
        perror("error listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
 
    socklen_t len;

    for(;;)
    {
        len = sizeof(client_addr);
        //int ConnectFD = accept(SocketFD, NULL, NULL);
        int ConnectFD = accept(SocketFD, (struct sockaddr *)&client_addr, &len);
   
        if(0 > ConnectFD)
        {
            perror("error accept failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }

  	    else thread(action,ConnectFD).detach(); 
    }
 
    close(SocketFD);
    return 0;
}
