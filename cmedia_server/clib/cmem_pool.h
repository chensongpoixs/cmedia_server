/***********************************************************************************************
created: 		2019-05-12

author:			chensong

purpose:		cmem_pool
************************************************************************************************/

#ifndef _C_MEM_POOL_H_
#define _C_MEM_POOL_H_
#include <mutex>
namespace chen {
	namespace cmem_util_pool
	{
		class cblock_pool;
	}
	class cmem_pool
	{
	private:
		typedef std::mutex										clock_type;
	public:
		explicit cmem_pool(bool safe);
		bool	init(unsigned int max_block = 10, unsigned int block_size = 1024 *1024, unsigned int start_size = 32 );
		void * alloc(unsigned int bytes);
		void free(void * const ptr);
		void set_max_block(unsigned int block_size);
		void gc();
		void destroy();
		void show_info() const;
		unsigned int get_current_size() const;
		unsigned int get_use_size() const;
		unsigned int get_block_count() const;
		unsigned int get_free_size() const;
	private:
		int			_sizeindex(unsigned int& realsize);
		void		_gc_one();
	private:
		cmem_util_pool::cblock_pool*	m_pool;
		unsigned int					m_max_block_size; // ���block�ĸ���ʱ gc
		unsigned int					m_block_size; // ÿ��block�Ĵ�С
		unsigned int					m_start_size; // ��ʼλ��
		unsigned int					m_pool_size; //���ӵĸ���
		unsigned int					m_current_size; //����ʹ�õ��ڴ��С
		unsigned int					m_block_count; //�������ڴ�Ŀ���
		unsigned int					m_use_size; //�Ѿ����ڴ�Ĵ�С
		unsigned int					m_free_size; //�����ڴ�������
		clock_type						m_lock;
		bool							m_safe;
	};
} // namespace chen


#endif // #define _C_MEM_POOL_H_
