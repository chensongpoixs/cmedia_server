/***********************************************************************************************
created: 		2019-03-06

author:			chensong

purpose:		net_mgr
************************************************************************************************/
#ifndef _C_NET_MGR_H_
#define _C_NET_MGR_H_
#include "cnoncopyable.h"
//#include "creactor.h"
#include "cnet_type.h"
//#include "cacceptor.h"
#include <iostream>
#include <vector>
#include "cnet_session.h"
#include <atomic>
//#include "cqueue.h"
#include <queue>
#include <thread>
#include "cnet_msg.h"
#include "cnet_msg_queue.h"
#include "cnet_define.h"
//#include "csafe_queue.h"
#include <functional>


#include <boost/asio.hpp>

namespace chen {
	class cnet_session;

	class cnet_mgr :public cnoncopyable
	{
	public:
		//连接回调
		typedef std::function<void(uint32 session_id, uint32 para, const char* buf)> cconnect_cb;
		//断开连接回调
		typedef std::function<void(uint32 session_id)> 								cdisconnect_cb;
		//新消息回调
		typedef std::function<void(uint32 session_id, uint16 msg_id, const void* p, uint32 size)> cmsg_cb;
	

	private:
		typedef boost::asio::io_service					cservice;
		typedef boost::asio::io_service::work			cwork;
		typedef boost::asio::ip::tcp::socket			csocket_type;
		typedef boost::asio::ip::tcp::acceptor			cacceptor;
		typedef boost::asio::ip::tcp::endpoint			cendpoint;
		typedef boost::asio::deadline_timer				ctimer;
		typedef boost::system::error_code				cerror_code;
	private:

		typedef std::vector<cnet_session*> 											csessions;
		typedef std::vector<std::thread> 											cthreads;
		typedef std::atomic_bool 													catomic_bool;
		typedef std::atomic_uint64_t												catomic_uint;
		typedef std::mutex															clock_type;
		typedef std::lock_guard<clock_type>											clock_guard;
	private:
		struct cconnect_para
		{
			cconnect_para(cservice& io_service);
			std::string		ip;
			uint16			port;
			ctimer			timer;
			catomic_bool	busy;
		};

		typedef std::vector<cconnect_para*> cparas;
	public:
		explicit	cnet_mgr();
		~cnet_mgr();
	public:
		static cnet_mgr *	construct();
		static void			destroy(cnet_mgr * ptr);
	public:
		/**
		* @param name: 服务的名称
		* @param client_session:   作为客户端是转入 
		* @param max_session : 作为服务端 是转入
		* @param send_buff_size: 发送缓冲区的大小
		* @param recv_buff_size: 接收缓冲区的大小
		**/
		bool 		init(const std::string& name, int32 client_session, int32 max_session
			, uint32 send_buff_size, uint32 recv_buff_size, uint32 pool_size);
		/**
		* 关服操作
		**/
		void 		shutdown();
		void 		destroy();
	public:
		/**
		*  @param  thread_num io线程数
		*  @param  ip
		*  @param  port
		**/
		bool 		startup(uint32 thread_num, const char *ip, uint16 port);
	public:
		//消息id秘钥
		void set_msg_id_key(uint16 value) { m_msg_id_key = value; }
		
		//消息size秘钥
		void set_msg_size_key(uint32 value) { m_msg_size_key = value; }
		
		//最大接收消息大小
		void set_max_msg_size(uint32 value) { m_max_received_msg_size = value; }
		
		
		//自动重连时间
		void set_reconnet_second(uint32 seconds) { m_reconnect_second = seconds; }

		//是否外网
		void set_wan() { m_wan = true; }

	public:
		// client
		bool 		connect_to(uint32 index, const std::string& ip_address, uint16 port);
	public:
		//连接回调
		void set_connect_callback(cconnect_cb callback) { m_connect_callback = callback; }

		//断开连接回调
		void set_disconnect_callback(cdisconnect_cb callback) { m_disconnect_callback = callback; }

		//消息回调
		void set_msg_callback(cmsg_cb callback) { m_msg_callback = callback; }
	public:
		//// 设置连接超时时间[心跳包] 单位 秒
		//void set_times_second(uint32 msleep);
		////// 断线重连时间 单位 秒
		//void set_reconnection(uint32 msleep);
	public:
		void 		process_msg();

		bool		transfer_msg(uint32 sessionId, cnet_msg & msg);

		/**
		*  @param  sessionId : 回话id
		*  @param  msg_id    : 消息id
		*  @param  msg_ptr   : 消息的数据
		*  @param  msg_size  : 消息的大小
		**/
		bool 		send_msg(uint32 sessionId, uint16 msg_id, const void* msg_ptr, uint32 msg_size);

		// 指定服务器发送
		bool 		send_msg(uint32 sessionId, uint16 msg_id, const void* msg_ptr, uint32 msg_size, int32 extra);
		/**
		*  @param sessionId  : 关闭指定的回话id
		**/
		void 		close(uint32 sessionId);
		// 连接客户端的数量和数据 信息  
		void		show_info();
	public:
		/*
		*   消息包
		*/
		cnet_msg*	create_msg(uint16 msg_id, uint32 msg_size);
		void		msg_push(cnet_msg * msg_ptr);
		
		void		post_disconnect(cnet_session * p);
	public:

		cmem_pool * get_pool() const { return m_pool_ptr; }
		uint16		get_msg_id_key() const { return m_msg_id_key; }
		uint32		get_msg_size_key() const { return m_msg_size_key; }
		uint32		get_max_msg_size() const { return m_max_received_msg_size; }
		const std::string& get_name() const { return m_name; }
		bool		is_wan() const { return m_wan; }
	private:
		void 		_worker_thread();
		///**
		//* @param  psession : 清除指定得到session
		//**/
		//void		_clearup_session(cnet_session * psession);

		//// 得到一个新的客户端连接
		//void		_new_connect();
	private:
		// server api
		// 服务器异步监听是否有新的客户端连接
		//void		_listen_start(const char *ip, uint16 port);
		bool _start_listen(const std::string& ip, uint16 port, uint32 thread_num);
		void _post_accept();
		void _handle_accept(const cerror_code& error, cnet_session* session_ptr);
		cnet_msg* _create_accept_msg(const csocket_type& socket);
	private:
		// client api
		// 同步连接服务器
		bool		_connect_start(uint32 index, const char *ip, uint16 port);
	private:
		void		_handle_connect(const cerror_code& error, uint32 index);
		void		_handle_reconnect(uint32 index);
		cnet_msg*	_create_connect_msg(uint32 error_code);
	
	private:
		// 取得一个可用的会话
		cnet_session* _get_available_session();

		// 归还一个关闭的会话
		void _return_session(cnet_session* session_ptr);
	private:
		void _handle_close(cnet_session* session_ptr);
		bool _is_server_index(uint32 index) const;
	private:
		std::string						m_name;
		
		//creactor*						m_reactor;          // 反应堆
															// callback	
		cservice						m_io_service;
		cwork							m_work;
		// 
		cmem_pool*						m_pool_ptr;

		// callback
		cconnect_cb						m_connect_callback;
		cmsg_cb							m_msg_callback;
		cdisconnect_cb					m_disconnect_callback;
		
		csessions						m_sessions;               //所有会话
		clock_type						m_avail_session_mutex;
		csessions						m_available_sessions;     //开使用的会话
		catomic_bool					m_shuting;
		cthreads 						m_threads;


		cacceptor*						m_acceptor_ptr;		// 监听socket
		catomic_uint					m_listening_num;
		uint32							m_listening_max;
		//csafe_queue						m_msgs;
		//config
		uint16							m_msg_id_key;
		uint32							m_msg_size_key;
		uint32							m_max_received_msg_size;
		bool							m_wan;  //是否是网关

												//client
		uint32							m_reconnect_second;
		cparas							m_paras;
		
		//uint32							m_active_num; //反应堆的数量
		cnet_msg_queue*					m_msgs;
	};
} // chen 

#endif // _C_NET_MGR_H_
