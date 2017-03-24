#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include "protobuf/package.pb.h"
#include "protobuf/package.pb.h"
#include "vcs/event_base/event_base.h"
#include "vcs/util/vc_codec.h"
#include "common.h"

#define SERVER_HOST		"127.0.0.1"
#define SERVER_PORT	    7001	

using namespace std;
using namespace vcs;

int g_fd;

void printHelp(){/*{{{*/
	cout << "------------input cmd---------------------" << endl;
	cout << "help: print this" << endl;
	cout << "login: login to server" << endl;
	cout << "create: create room" << endl;
	cout << "join: join room" << endl;
	cout << "quit: quit room" << endl;
	cout << "-------------------------------------------" << endl;
	cout << endl;
}/*}}}*/

void readResp(Buffer* buffer){/*{{{*/
	VCCodec::PACKAGE_HEAD head;
	cout << "wait resp..." << endl;
	char buf[1024];
	while(true){
		
		int curr_read = 0;
		if((curr_read = read(g_fd, buf, 1024)) > 0){
			buffer->write(buf, curr_read);
			cout << "recved " << buffer->size() << " bytes" << endl;
		}

		if(buffer->size() < sizeof(head)){
			continue;
		}

		buffer->peek((char*)&head, sizeof(head));
		head.length = ntohs(head.length);
		if(buffer->size() < head.length){
			continue;
		}
		break;

	}
	cout << "recv done" << endl;	
}/*}}}*/

void login(){/*{{{*/
    uint32_t uid;
	cout << "input uid:";
	cin >> uid;

	cout << "login with uid " << uid << endl;

    pb::c2s_login request;
	request.set_uid(uid);
	string str;
	request.SerializeToString(&str);
	
    Buffer* writeBuffer = VCCodec::encode(SERVER_VERSION, C2S_LOGIN, str);
	if(!Helper::Socket::write(g_fd, writeBuffer)){
		cout << "send fail" << endl;
	} else {
		cout << "send succ, begin to read" << endl;
	}
    delete writeBuffer;

	Buffer readBuffer;
	readResp(&readBuffer);
    VCCodec::Package* p = VCCodec::decode(&readBuffer);
    if(p->status == VCCodec::PACKAGE_STATUS_PARTIAL){
        cout << "recv partial package" << endl;
        delete p;
        return ;
    } else if(p->status == VCCodec::PACKAGE_STATUS_CORRUPT){
        cout << "decode fail, data corrupt";
        delete p;
        return ;
    }

    pb::s2c_login resp;
	resp.ParseFromString(p->body);
	cout << "login result:" << resp.result() << endl;
}
/*}}}*/

void createRoom(){/*{{{*/
	cout << "begin to create room" << endl;

    pb::c2s_create_room request;
	request.set_type(1);
	string str;
	request.SerializeToString(&str);
	
    Buffer* writeBuffer = VCCodec::encode(SERVER_VERSION, C2S_CREATE_ROOM, str);
	if(!Helper::Socket::write(g_fd, writeBuffer)){
		cout << "send fail" << endl;
	} else {
		cout << "send succ, begin to read" << endl;
	}
    delete writeBuffer;

	Buffer readBuffer;
	readResp(&readBuffer);
    VCCodec::Package* p = VCCodec::decode(&readBuffer);
    if(p->status == VCCodec::PACKAGE_STATUS_PARTIAL){
        cout << "recv partial package" << endl;
        delete p;
        return ;
    } else if(p->status == VCCodec::PACKAGE_STATUS_CORRUPT){
        cout << "decode fail, data corrupt";
        delete p;
        return ;
    }

    pb::s2c_create_room resp;
	resp.ParseFromString(p->body);
	cout << "create result:" << resp.result() << ",room id:" << resp.room_id() << endl;
}
/*}}}*/

int connect(const char* addr, int port){/*{{{*/
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = inet_addr(addr);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	connect(fd, (struct sockaddr *)&sin, sizeof(sin));

	return fd;
}/*}}}*/

int main(int argc, char* argv[]){/*{{{*/
	g_fd = connect(SERVER_HOST, SERVER_PORT);
	printHelp();
	if(argc <= 1){
		string input;
		while(cin >> input){
			if(input.compare("help") == 0){
				printHelp();
			} else if(input.compare("login") == 0){
                login();
			} else if(input.compare("create") == 0){
                createRoom();
			} else if(input.compare("get") == 0){
			} else if(input.compare("join") == 0){
			} else if(input.compare("quit") == 0){
			} else if(input.compare("recv_msg") == 0) {
			} else {
				printHelp();
			}

			input = "";
			cout << endl;
		}
	} else {
		int cnt = atoi(argv[1]);
		for(int i = 0; i < cnt; ++i){
		}
	}
}/*}}}*/

