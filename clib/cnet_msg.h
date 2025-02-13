/***********************************************************************************************
created: 		2019-05-19

author:			chensong

purpose:		cnet_msg
************************************************************************************************/
#ifndef _C_NET_MSG_H_
#define _C_NET_MSG_H_
#include "cnet_type.h"
#include "cmem_pool.h"
#include <atomic>
#include <cstring>
namespace chen {

	class cnet_msg
	{
	private:
		typedef std::atomic_ushort 				catomic_short;
	public:
		explicit	cnet_msg(cmem_pool* pool, uint16 msg_id, uint32 msg_size);
		~cnet_msg();
	public:
		static cnet_msg* alloc_me(cmem_pool* pool, uint16 msg_id, uint32 msg_size);
		void free_me();
	public:
		cnet_msg * get_next_msg() const  { return m_next_msg_ptr; }
		void set_next_msg(cnet_msg * ptr) { m_next_msg_ptr = ptr; }

		const uint32 get_session_id() const { return m_session_id; }
		void set_session_id(uint32 value) { m_session_id = value; }

		
		uint16 get_msg_id() const { return m_msg_id; }
		void set_msg_id(uint16 value) { m_msg_id = value; }
		void set_referces(uint32 value) { m_referces.fetch_add(value); }
		void add_referces() { m_referces.fetch_add(1); }
		void sub_referces();
		uint32 get_size() const { return m_size; }
		uint32 get_data_size() const { return m_data_size; }
		const char* get_buf() const { return m_buffer; }
		char* get_buf() { return m_buffer; }
		void fill(const void *p, uint32 len);
	private:
		cnet_msg(const cnet_msg&);
		cnet_msg& operator=(const cnet_msg&);
	private:
		cmem_pool*			m_pool_ptr;
		cnet_msg*			m_next_msg_ptr;
		uint32				m_session_id;
		uint16				m_msg_id;
		uint32				m_size;
		uint32				m_data_size;
		uint32				m_send_size;
		catomic_short		m_referces;
		char				m_buffer[1];
	};

}//namespace chen


#endif // !#define _C_NET_MSG_H_