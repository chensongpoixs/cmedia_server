/***********************************************************************************************
created: 		2019-05-15

author:			chensong

purpose:		net_msg_header
************************************************************************************************/


#ifndef _C_NET_MSG_HEADER_H_
#define _C_NET_MSG_HEADER_H_
#include "cnet_type.h"
namespace chen {
	/**
	** 信息包头信息
	**/
#pragma pack(push, 1)
	struct cmsg_header
	{
		uint16	m_msg_id;
		uint32	m_msg_size;
		cmsg_header()
			:m_msg_id(0)
			, m_msg_size(0)
		{}
	};
#pragma pack(pop)


	static const uint32 MSG_ID_SIZE = sizeof(uint16);
	static const uint32 MSG_HEADER_SIZE = sizeof(cmsg_header);

	inline uint32 get_session_index(uint32 id) { return id & 0x0000FFFF; }

	inline uint32 get_session_serial(uint32 id) { return (id & 0xFFFF0000) >> 16; }
}//namespace chen

#endif // !#define _C_NET_MSG_HEADER_H_