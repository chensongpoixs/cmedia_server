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
		//���ӻص�
		typedef std::function<void(uint32 session_id, uint32 para, const char* buf)> cconnect_cb;
		//�Ͽ����ӻص�
		typedef std::function<void(uint32 session_id)> 								cdisconnect_cb;
		//����Ϣ�ص�
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
		* @param name: ���������
		* @param client_session:   ��Ϊ�ͻ�����ת�� 
		* @param max_session : ��Ϊ����� ��ת��
		* @param send_buff_size: ���ͻ������Ĵ�С
		* @param recv_buff_size: ���ջ������Ĵ�С
		**/
		bool 		init(const std::string& name, int32 client_session, int32 max_session
			, uint32 send_buff_size, uint32 recv_buff_size, uint32 pool_size);
		/**
		* �ط�����
		**/
		void 		shutdown();
		void 		destroy();
	public:
		/**
		*  @param  thread_num io�߳���
		*  @param  ip
		*  @param  port
		**/
		bool 		startup(uint32 thread_num, const char *ip, uint16 port);
	public:
		//��Ϣid��Կ
		void set_msg_id_key(uint16 value) { m_msg_id_key = value; }
		
		//��Ϣsize��Կ
		void set_msg_size_key(uint32 value) { m_msg_size_key = value; }
		
		//��������Ϣ��С
		void set_max_msg_size(uint32 value) { m_max_received_msg_size = value; }
		
		
		//�Զ�����ʱ��
		void set_reconnet_second(uint32 seconds) { m_reconnect_second = seconds; }

		//�Ƿ�����
		void set_wan() { m_wan = true; }

	public:
		// client
		bool 		connect_to(uint32 index, const std::string& ip_address, uint16 port);
	public:
		//���ӻص�
		void set_connect_callback(cconnect_cb callback) { m_connect_callback = callback; }

		//�Ͽ����ӻص�
		void set_disconnect_callback(cdisconnect_cb callback) { m_disconnect_callback = callback; }

		//��Ϣ�ص�
		void set_msg_callback(cmsg_cb callback) { m_msg_callback = callback; }
	public:
		//// �������ӳ�ʱʱ��[������] ��λ ��
		//void set_times_second(uint32 msleep);
		////// ��������ʱ�� ��λ ��
		//void set_reconnection(uint32 msleep);
	public:
		void 		process_msg();

		bool		transfer_msg(uint32 sessionId, cnet_msg & msg);

		/**
		*  @param  sessionId : �ػ�id
		*  @param  msg_id    : ��Ϣid
		*  @param  msg_ptr   : ��Ϣ������
		*  @param  msg_size  : ��Ϣ�Ĵ�С
		**/
		bool 		send_msg(uint32 sessionId, uint16 msg_id, const void* msg_ptr, uint32 msg_size);

		// ָ������������
		bool 		send_msg(uint32 sessionId, uint16 msg_id, const void* msg_ptr, uint32 msg_size, int32 extra);
		/**
		*  @param sessionId  : �ر�ָ���Ļػ�id
		**/
		void 		close(uint32 sessionId);
		// ���ӿͻ��˵����������� ��Ϣ  
		void		show_info();
	public:
		/*
		*   ��Ϣ��
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
		//* @param  psession : ���ָ���õ�session
		//**/
		//void		_clearup_session(cnet_session * psession);

		//// �õ�һ���µĿͻ�������
		//void		_new_connect();
	private:
		// server api
		// �������첽�����Ƿ����µĿͻ�������
		//void		_listen_start(const char *ip, uint16 port);
		bool _start_listen(const std::string& ip, uint16 port, uint32 thread_num);
		void _post_accept();
		void _handle_accept(const cerror_code& error, cnet_session* session_ptr);
		cnet_msg* _create_accept_msg(const csocket_type& socket);
	private:
		// client api
		// ͬ�����ӷ�����
		bool		_connect_start(uint32 index, const char *ip, uint16 port);
	private:
		void		_handle_connect(const cerror_code& error, uint32 index);
		void		_handle_reconnect(uint32 index);
		cnet_msg*	_create_connect_msg(uint32 error_code);
	
	private:
		// ȡ��һ�����õĻỰ
		cnet_session* _get_available_session();

		// �黹һ���رյĻỰ
		void _return_session(cnet_session* session_ptr);
	private:
		void _handle_close(cnet_session* session_ptr);
		bool _is_server_index(uint32 index) const;
	private:
		std::string						m_name;
		
		//creactor*						m_reactor;          // ��Ӧ��
															// callback	
		cservice						m_io_service;
		cwork							m_work;
		// 
		cmem_pool*						m_pool_ptr;

		// callback
		cconnect_cb						m_connect_callback;
		cmsg_cb							m_msg_callback;
		cdisconnect_cb					m_disconnect_callback;
		
		csessions						m_sessions;               //���лỰ
		clock_type						m_avail_session_mutex;
		csessions						m_available_sessions;     //��ʹ�õĻỰ
		catomic_bool					m_shuting;
		cthreads 						m_threads;


		cacceptor*						m_acceptor_ptr;		// ����socket
		catomic_uint					m_listening_num;
		uint32							m_listening_max;
		//csafe_queue						m_msgs;
		//config
		uint16							m_msg_id_key;
		uint32							m_msg_size_key;
		uint32							m_max_received_msg_size;
		bool							m_wan;  //�Ƿ�������

												//client
		uint32							m_reconnect_second;
		cparas							m_paras;
		
		//uint32							m_active_num; //��Ӧ�ѵ�����
		cnet_msg_queue*					m_msgs;
	};
} // chen 

#endif // _C_NET_MGR_H_
