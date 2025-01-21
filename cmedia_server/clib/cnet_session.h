/***********************************************************************************************
created: 		2019-03-01

author:			chensong

purpose:		net_session
************************************************************************************************/

#ifndef _C_NET_SESSION_H_
#define _C_NET_SESSION_H_
#include "cnoncopyable.h"
#include "cnet_type.h"
#include "cmsg_buffer.h"
#include <mutex>
#include "cnet_define.h"
#include <atomic>
namespace chen {

	class cnet_mgr;
	class cnet_msg;
	class cnet_session :private cnoncopyable
	{
	private:
		typedef std::atomic_bool 													catomic_bool;
		typedef std::mutex															clock_type;
		typedef std::lock_guard<clock_type>											clock_guard;
		typedef boost::asio::io_service												cservice;
		typedef boost::asio::ip::tcp::socket										csocket_type;
	private:
		enum cENetSessionStatus
		{
			ENSS_None = 0,
			ENSS_Init,
			ENSS_Accept,
			ENSS_ConnectTo,
			ENSS_Open,
			ENSS_Shut,
			ENSS_Close,
		};
	public:

		explicit cnet_session(cservice& service, cnet_mgr&  _ptr, uint32 session_id);
		virtual ~cnet_session();
	public:
		/**
		* @param   send_buff_size : 发送缓冲区的大小
		* @param   recv_buff_size : 接收缓冲区的大小
		**/
		bool 	init(uint32 send_buff_size, uint32 recv_buff_size);
		void 	destroy();
	public:
		bool	is_status_init() const { return m_status == ENSS_Init; }
		void	set_status_init();
		void	set_accept_status();
		void	set_connect_status();

		void	connected(cnet_msg *msg);

		void	reset();
	public:
		// 读取数据
		bool	process_msg();
		/**
		* 发送数据包缓存到发送缓冲区中
		* @param   msg_ptr	: 发送数据包
		* @param   size		: 发送数据包的大小
		**/
		bool		send_msg(uint32 session_id, uint16 msg_id, const void * msg_ptr, uint32 msg_size);

	
		bool		push_queue_msg(uint16 msg_id, uint32 msg_size,const void * msg_ptr);
		uint32 		get_session_id() const { return m_session_id; }

		csocket_type&	get_socket() { return m_socket; }
		void		close(uint32 session_id);

	private:
		
		void			_handle_read(const boost::system::error_code& error, size_t bytes_transferred);
		void			_handle_write(const boost::system::error_code& error, size_t bytes_transferred);
	private:
		void	_close();
		void	_release();
	private:
		bool	_push_to_queue(const void * msg_ptr, uint32 msg_size, uint16 msg_id);
		void	_push_queue_msg(cnet_msg * msg_ptr);
		void	_msg_queue_buf();
		void	_free_wait_queue_msg();
		bool	_parse_msg();
	private:
		cnet_mgr&				m_session_mgr;
		uint32					m_session_id;		// 网络层的会话id
		cENetSessionStatus		m_status;
		csocket_type			m_socket;
		catomic_bool			m_reading_atomic;
		catomic_bool			m_writeing_atomic;
		clock_type 				m_mutex;
		cmsg_receive_buffer*	m_recv_buf_ptr;
		cnet_msg*				m_recv_ptr;
		uint32					m_recv_size;			//接收缓冲区大小
		// send 
		cnet_msg*				m_wait_send_head_ptr;
		cnet_msg*				m_wait_send_tail_ptr;
		cmsg_send_buffer*		m_send_buf_ptr;
		cmsg_send_buffer*		m_sending_buf_ptr;
		uint32					m_send_size;         //发送缓冲区的大小
		//char					m_buf[1024];

	};

}  //namespace chen

#endif // !_C_NET_SESSION_H_