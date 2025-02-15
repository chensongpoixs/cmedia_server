/***********************************************************************************************
created: 		2019-12-27

author:			chensong

purpose:		cmsg_buffer
	��Ӯ����Ҫ���𰸶�������ʲô�������Ҫ��

	�����ߣ��ٴ�֮����Ҳ��Ψ�з������ܣ�����������ʱ����ʱ����Ӣ�ۣ�Ӣ�۴�����ʱ�����������˵�����񣬿��㱾�����ٰ��� �����ã��Լ�������ͼ��顣


	�ҿ��ܻ������ܶ���ˣ������ǽ���2��Ĺ��£�����д�ɹ��»��ɸ裬����ѧ���ĸ���������������
Ȼ�󻹿�����һ����������һ�������ҵĹ�������ܻᱻ��˧����������ֻᱻ��������ں���������ĳ�������ҹ������ȫ����͸Ȼ��Ҫ������С��ס�������ϵ�ʪ�·���
3Сʱ���������������ʵ��������ҵĹ�������Ը���򸸻���顣�Ҳ����Ѹ��������Ǵ�Խ�����������ǣ���ʼ��Ҫ�ص��Լ������硣
Ȼ���ҵ���Ӱ������ʧ���ҿ������������ˮ������ȴû���κ�ʹ�࣬�Ҳ�֪����ԭ���ҵ��ı������ˣ�������ȫ�����ԭ�򣬾���Ҫ�һ��Լ��ı��ġ�
�����ҿ�ʼ����Ѱ�Ҹ��ָ���ʧȥ�ĵ��ˣ��ұ��һ��שͷ��һ������һ��ˮ��һ����ƣ�ȥ�����Ϊʲô��ʧȥ�Լ��ı��ġ�
�ҷ��֣��ճ����ı��������Ļ��ڣ������ģ����ǵı��ľͻ���ʧ���յ��˸��ֺڰ�֮�����ʴ��
��һ�����ۣ������ʺͱ��ߣ�����ί����ʹ�࣬�ҿ���һֻֻ���ε��֣������ǵı��ĳ��飬�ɱΣ�͵�ߣ���Ҳ�ز������˶���ߡ�
�ҽ����������֡��������Ǻ�����ͬ�ڵļ��� �����Ҳ������£�����ϸ�����Լ�ƽ����һ�� Ѱ�ұ������ֵĺۼ���
�����Լ��Ļ��䣬һ�����ĳ�����������������֣��ҵı��ģ�����д�����ʱ�򣬻������
��������Ȼ����������ҵ�һ�У�д��������ұ��Ļع����÷�ʽ���һ�û�ҵ��������֣��������ţ�˳�������������һ����˳�����ϣ�������������
*********************************************************************/
#ifndef _C_MSG_BUFFER_H_
#define _C_MSG_BUFFER_H_
#include "cnoncopyable.h"
#include "cnet_type.h"
namespace chen {
	class cmsg_send_buffer : private cnoncopyable
	{
	public:
		cmsg_send_buffer(uint32 max_size);
		~cmsg_send_buffer();
	public:
		static cmsg_send_buffer*alloc_me(uint32 size);
		static void				free_me(cmsg_send_buffer * ptr);

		uint32					append(const void *p, uint32 len);
		uint32					avail() { return m_max_size - m_size; }
		void					deducted(uint32 size) { m_size -= size; }
		bool					empty() const { return m_size == 0; }
		char *					get_buf() { return m_buffer; }
		uint32					size() const { return m_size ; }

		void					reset();
		void					set_send_size(uint32 value) { m_send_size += value; }
		uint32					get_send_size() const { return m_send_size; }
	private:
		uint32					m_max_size;    // 
		uint32					m_size; // buf size
		uint32					m_send_size; // send buf size
		char 					m_buffer[1];
	};


	class cmsg_receive_buffer : private cnoncopyable
	{
	public:
		cmsg_receive_buffer(uint32 size);
		~cmsg_receive_buffer();
	public:
		static cmsg_receive_buffer * alloc_me(uint32 size);
		static void				free_me(cmsg_receive_buffer * ptr);
		uint32					max_size() const { return m_size; }
		uint32					append(void *p, uint32 len);
		uint32					get_size() const { return m_data_size; }
		uint32					avail() { return m_size - m_data_size; }
		char *					get_buf() { return m_buffer; }


		void					resert();
	private:
		uint32					m_size;
		uint32					m_data_size;
		char					m_buffer[1];
	};
} // !namespace chen

#endif // !#define _C_MSG_BUFFER_H_