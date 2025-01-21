/***********************************************************************************************
created: 		2019-12-27

author:			chensong

purpose:		cnet_msg_queue
************************************************************************************************/
#include "cnet_msg.h"
#include "cnet_msg_queue.h"

namespace chen {
	cnet_msg_queue::cnet_msg_queue()
		: m_head_ptr (NULL)
		, m_tail_ptr (NULL)
	{
	}
	cnet_msg_queue::~cnet_msg_queue()
	{
	}
	void cnet_msg_queue::enqueue(cnet_msg * p)
	{
		clock_guard guard(m_lock);
		if (!m_head_ptr)
		{
			m_head_ptr = m_tail_ptr = p;
		}
		else
		{
			m_head_ptr->set_next_msg(p);
			m_head_ptr = p;
		}
	}
	cnet_msg * cnet_msg_queue::dequeue()
	{
		clock_guard guard(m_lock);
		if (!m_tail_ptr)
		{
			return NULL;
		}
		cnet_msg * ptr = m_tail_ptr;
		m_tail_ptr = m_head_ptr = NULL;
		return ptr;
	}
	void cnet_msg_queue::clear()
	{
		clock_guard guard(m_lock);
		cnet_msg* msg_ptr = NULL;
		while (m_tail_ptr)
		{
			msg_ptr = m_tail_ptr->get_next_msg();
			m_tail_ptr->free_me();
			m_tail_ptr = msg_ptr;
		}
		m_head_ptr = m_tail_ptr =  NULL;
	}
} // !namespace chen 
