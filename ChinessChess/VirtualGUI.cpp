#include <sys/types.h>
#include <sys/stat.h>
//for mkfifo
#include <unistd.h>
//for read and write
#include <fcntl.h>
//for open
#include <cstring>
//for memset
#include "UBString.h"
#include "UBUCCI.h"
#include <iostream>
using std::cout;
using std::endl;
#define FIFONAME "/tmp/UCCIEngineFIFO"
#define BUFFERSIZE	200
int main()
{
	if(access(FIFONAME,F_OK) == -1)
	{
		mkfifo(FIFONAME,0777);
	}
	int fifoHandle;
	char buffer[BUFFERSIZE];
	UBString bufferStr="ucci";
	int bufferSize=bufferStr.length();
	int ret=0;
	fifoHandle=open(FIFONAME,O_WRONLY);
	write(fifoHandle,bufferStr.getData(),bufferSize);
	close(fifoHandle);
	memset(buffer,0,BUFFERSIZE);
	fifoHandle=open(FIFONAME,O_RDONLY);
	read(fifoHandle,buffer,BUFFERSIZE);
	close(fifoHandle);
	cout<<"RCV:"<<buffer<<endl;

	bufferStr="position startpos";
	bufferSize=bufferStr.length();
	fifoHandle=open(FIFONAME,O_WRONLY);
	write(fifoHandle,bufferStr.getData(),bufferSize);
	close(fifoHandle);
	memset(buffer,0,BUFFERSIZE);
	fifoHandle=open(FIFONAME,O_RDONLY);
	read(fifoHandle,buffer,BUFFERSIZE);
	close(fifoHandle);
	cout<<"RCV:"<<buffer<<endl;

	bufferStr="go";
	bufferSize=bufferStr.length();
	fifoHandle=open(FIFONAME,O_WRONLY);
	write(fifoHandle,bufferStr.getData(),bufferSize);
	close(fifoHandle);
	memset(buffer,0,BUFFERSIZE);
	fifoHandle=open(FIFONAME,O_RDONLY);
	read(fifoHandle,buffer,BUFFERSIZE);
	close(fifoHandle);
	cout<<"RCV:"<<buffer<<endl;

	BESTMOVE_REPLY_STRUCT bestmove_reply;
	bestmove_reply.setMember(UBString(buffer));
	bufferStr="position startpos moves ";
	bufferStr+=bestmove_reply.bestMove;
	bufferStr+=' ';
	bufferStr+="a9a8";
	bufferSize=bufferStr.length();
	fifoHandle=open(FIFONAME,O_WRONLY);
	write(fifoHandle,bufferStr.getData(),bufferSize);
	close(fifoHandle);
	memset(buffer,0,BUFFERSIZE);
	fifoHandle=open(FIFONAME,O_RDONLY);
	read(fifoHandle,buffer,BUFFERSIZE);
	close(fifoHandle);
	cout<<"RCV:"<<buffer<<endl;

	bufferStr="go";
	bufferSize=bufferStr.length();
	fifoHandle=open(FIFONAME,O_WRONLY);
	write(fifoHandle,bufferStr.getData(),bufferSize);
	close(fifoHandle);
	memset(buffer,0,BUFFERSIZE);
	fifoHandle=open(FIFONAME,O_RDONLY);
	read(fifoHandle,buffer,BUFFERSIZE);
	close(fifoHandle);
	cout<<"RCV:"<<buffer<<endl;

	bufferStr="quit";
	bufferSize=bufferStr.length();
	fifoHandle=open(FIFONAME,O_WRONLY);
	write(fifoHandle,bufferStr.getData(),bufferSize);
	close(fifoHandle);
	memset(buffer,0,BUFFERSIZE);
	fifoHandle=open(FIFONAME,O_RDONLY);
	read(fifoHandle,buffer,BUFFERSIZE);
	close(fifoHandle);
	cout<<"RCV:"<<buffer<<endl;
}
