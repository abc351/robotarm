#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
using namespace std;
pthread_t thr[3];
char serial_name[100];
char serial2_name[100];
int sock0,sockw;
int __port;
void* network(void* dat);
void* motion(void* dat);

int memfd;
volatile unsigned int *systimer, *CLO, *CHI,*gpio, *GPFSEL, *GPSET, *GPCLR, *GPLEV;

int main(int argc,char* argv[]){
	//usleep(10000000u);
	if(argc<3) return 0;
	sprintf(serial_name,"/dev/%s",argv[1]);
	sprintf(serial2_name,"/dev/%s",argv[3]);
	__port=atoi(argv[2]);
	cout<<serial_name<<endl;
	cout<<"init"<<endl;

	pthread_create(thr,NULL,network,reinterpret_cast<void*>(0));
	pthread_create(thr+1,NULL,motion,reinterpret_cast<void*>(0));
	getchar();
	cout<<"stop"<<endl;
	close(sock0);
	close(sockw);
	system("sudo killall -9 motion\n");
	cout<<"end"<<endl;
}
void* motion(void* dat){
	system("sudo motion\n");
	cout<<"motion running"<<endl;
	return 0;
}

void* network(void* dat){
	char sport[100];
	termios otm,tm,otm2,tm2;
	int port,port2;
	port=open(serial_name,O_RDWR|O_NOCTTY);
	if(!port){
		printf("port err\n");
		return 0;
	}
	tcgetattr(port,&otm);
	tcgetattr(port,&tm);
	tm.c_iflag=IGNPAR;
	tm.c_oflag=0;
	tm.c_lflag=0;
	tm.c_cflag=B9600|CS8|CLOCAL|CREAD;
	tcsetattr(port,TCSAFLUSH,&tm);
	tcflush(port,TCIFLUSH);
	port2=open(serial2_name,O_RDWR|O_NOCTTY);
	if(!port2){
		printf("port2 err\n");
		//return 0;
	}
	tcgetattr(port2,&otm2);
	tcgetattr(port2,&tm2);
	tm2.c_iflag=IGNPAR;
	tm2.c_oflag=0;
	tm2.c_lflag=0;
	tm2.c_cflag=B9600|CS8|CLOCAL|CREAD;
	tcsetattr(port2,TCSAFLUSH,&tm2);
	tcflush(port2,TCIFLUSH);
	char recv[10240];
	const char html_upp[]=
	"HTTP/1.0 200 OK\n"\
	"Content-type:text/html\n"\
	"\n"\
	"<!DOCTUPE html>\n"\
	"<head>\n"\
	"</head>\n"\
	"<body>\n";
	const char html_low[]=
	"</body>\n"\
	"</html>";
	char html[10000];
	int recv_len;
	sockaddr_in addr,client;
	socklen_t sockaddr_in_size=sizeof(sockaddr_in);
	sock0=socket(AF_INET,SOCK_STREAM,0);
	int opt=1;
	setsockopt(sock0,SOL_SOCKET,SO_REUSEADDR,&opt,4);
	if(sock0==-1){
		cout<<"sock init fail"<<endl;
		return 0;
	}
	addr.sin_family=AF_INET;
	addr.sin_port=htons(__port);
	addr.sin_addr.s_addr=INADDR_ANY;
	if(bind(sock0,(sockaddr*) &addr,sizeof(addr))!=0){
		cout<<"bind err"<<endl;
		return 0;
	}
	if(listen(sock0,5)!=0){
		cout<<"listen err"<<endl;
		return 0;
	}
	while(1){
		int len=sizeof(client);
		sockw=accept(sock0,(sockaddr*) &client,&sockaddr_in_size);
		if(sockw==-1){
			cout<<"accept err"<<endl;
			break;
		}
		memset(recv,0,10240);
		recv_len=recvfrom(sockw,recv,10240,0,(sockaddr*)&client,&sockaddr_in_size);
		recv[recv_len-1]=0;
		cout<<recv<<endl;
		if(strstr(recv,"/arm")!=0){
			char* t;
			double x,y,z,th;
			int m1,m2;
			int g;
			
			t=strstr(recv,"x=");
			if(t!=0) x=atof(t+2);
			t=strstr(recv,"y=");
			if(t!=0) y=atof(t+2);
			t=strstr(recv,"z=");
			if(t!=0) z=atof(t+2);
			t=strstr(recv,"t=");
			if(t!=0) th=atof(t+2);
			t=strstr(recv,"g=");
			if(t!=0) g=atoi(t+2);
			sprintf(html,"%s\nx=%f y=%f z=%f t=%f g=%d\n%s",html_upp,x,y,z,th,g,html_low);
			int len=sprintf(sport,"%f %f %f %f %d\n",x,y,z,th,g);
			printf("%s\n",sport);
			write(port,sport,len);
			t=strstr(recv,"m1=");
			if(t!=0) m1=atoi(t+3);
			
			t=strstr(recv,"m2=");
			if(t!=0) m2=atoi(t+3);
			len=sprintf(sport,"A%d %d",m1,m2);
			write(port2,sport,strlen(sport));
			
		}	
		else{
			sprintf(html,"%s\n%s",html_upp,html_low);
		}
		
		send(sockw,html,strlen(html),0);
		close(sockw);
	}
	close(sock0);
	cout<<"sock closed"<<endl;
	tcsetattr(port, TCSAFLUSH,&otm);
	tcsetattr(port2,TCSAFLUSH,&otm2);
	return 0;
}
