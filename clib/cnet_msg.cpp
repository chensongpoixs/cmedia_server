/***********************************************************************************************
created: 		2019-05-19

author:			chensong

purpose:		cnet_msg
************************************************************************************************/

#include "cnet_msg.h"
#include "clog.h"
namespace chen {
	cnet_msg::cnet_msg(cmem_pool * pool, uint16 msg_id, uint32 msg_size)
		: m_pool_ptr(pool)
		, m_next_msg_ptr(NULL)
		, m_session_id(0)
		, m_msg_id(msg_id)
		, m_size(msg_size)
		, m_data_size(0)
		, m_send_size(0)
		, m_referces(1)
	{
	}
	cnet_msg::~cnet_msg()
	{
	}
	cnet_msg * cnet_msg::alloc_me(cmem_pool* pool, uint16 msg_id, uint32 msg_size)
	{
		void* p = pool->alloc(sizeof(cnet_msg) + msg_size);
		if (!p)
		{
			//WARNING_LOG();
			return NULL;
		}
		cnet_msg *q = new(p) cnet_msg(pool, msg_id, msg_size);
		return q;
	}
	void cnet_msg::free_me()
	{
		this->~cnet_msg();
		m_pool_ptr->free(this);
	}
	void cnet_msg::sub_referces()
	{
		if(m_referces.fetch_sub(1) == 1)
		{
			free_me();
		}
	}
	
	void cnet_msg::fill(const void * p, uint32 len)
	{
		memcpy(m_buffer, p, len);
	}
} //namespace chen