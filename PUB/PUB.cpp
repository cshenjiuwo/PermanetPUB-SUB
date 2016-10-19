// PUB.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "zmq.h"
#include "zmq_utils.h"
#include "zhelpers.h"

#pragma commnet(lib, "libzmq-v90-mt-4_0_4.lib")
int main()
{
	void *context = zmq_init(1);
	//�����߻��֪�������Ѿ���
	void *sync = zmq_socket(context, ZMQ_PULL);
	zmq_bind(sync, "tcp://127.0.0.1:5564");

	//ʹ���׽��ַ�����Ϣ
	void * publisher = zmq_socket(context, ZMQ_PUSH);

	//�������־û���������Ϣ���������
	uint64_t hwm = 1;
	zmq_setsockopt(publisher, ZMQ_SNDHWM, &hwm, sizeof(hwm));

	//���ý�������С�������ж�����ʹ��
	uint64_t swap = 25000000;
	zmq_setsockopt(publisher, ZMQ_SNDBUF, &swap, sizeof(swap));
	zmq_bind(publisher, "tcp://127.0.0.1:5565");

	//�ȴ�ͬ����Ϣ
	char * string = s_recv(sync);
	free(string);

	//����10����Ϣ��һ��һ��
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

