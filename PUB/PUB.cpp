// PUB.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "zmq.h"
#include "zmq_utils.h"
#include "zhelpers.h"

#pragma commnet(lib, "libzmq-v90-mt-4_0_4.lib")
int main()
{
	void *context = zmq_init(1);
	//订阅者会告知我们它已就绪
	void *sync = zmq_socket(context, ZMQ_PULL);
	zmq_bind(sync, "tcp://127.0.0.1:5564");

	//使用套接字发送消息
	void * publisher = zmq_socket(context, ZMQ_PUSH);

	//避免慢持久化订阅者消息溢出的问题
	uint64_t hwm = 1;
	zmq_setsockopt(publisher, ZMQ_SNDHWM, &hwm, sizeof(hwm));

	//设置交换区大小，供所有订阅者使用
	uint64_t swap = 25000000;
	zmq_setsockopt(publisher, ZMQ_SNDBUF, &swap, sizeof(swap));
	zmq_bind(publisher, "tcp://127.0.0.1:5565");

	//等待同步消息
	char * string = s_recv(sync);
	free(string);

	//发布10条消息，一秒一条
	int update_nbr;
	for (update_nbr = 0; update_nbr < 10; update_nbr ++)	
	{
		char string[20];
		sprintf(string, "Update %d", update_nbr);
		s_send(publisher, string);
		s_sleep(1);
	}
	
    return 0;
}

