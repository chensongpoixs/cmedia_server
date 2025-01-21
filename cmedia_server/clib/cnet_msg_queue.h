/***********************************************************************************************
created: 		2019-12-27

author:			chensong

purpose:		cnet_msg_queue
************************************************************************************************/
#ifndef _C_MSG_NET_QUEUE_H_
#define _C_MSG_NET_QUEUE_H_
#include <mutex>

namespace chen {
	class cnet_msg;
	class cnet_msg_queue
	{
	private:
		typedef std::mutex								clock_type;
		typedef std::lock_guard<clock_type>				clock_guard;
	public:
		 cnet_msg_queue();
		~cnet_msg_queue();
		void enqueue(cnet_msg*p);
		cnet_msg* dequeue();
		void clear();
	private:
		cnet_msg_queue(const cnet_msg_queue&);
		cnet_msg_queue& operator=(const cnet_msg_queue&);
	private:
		cnet_msg*			m_head_ptr;
		cnet_msg*			m_tail_ptr;
		clock_type		m_lock;
	};
}//!namespace chen


#endif // !#define _C_MSG_NET_QUEUE_H_