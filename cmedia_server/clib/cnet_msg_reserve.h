/***********************************************************************************************
created: 		2019-05-17

author:			chensong

purpose:		cnet_msg_reserve
************************************************************************************************/
#ifndef _C_NET_MSG_RESERVE_H_
#define _C_NET_MSG_RESERVE_H_
#include "cnet_type.h"
namespace chen {

	enum EMsgIDReserve
	{
		EMIR_Close = 0,
		EMIR_Connect, //建立连接
		EMIR_Disconnect,

		EMIR_Max,
	};


	struct cmsg_connect
	{
		uint32 para;
		char buf[1];
	};
}//namespace chen

#endif // !#define _C_NET_MSG_RESERVE_H_
