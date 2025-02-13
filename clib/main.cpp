//#include <iostream>
//#include "cmem_pool.h"
//#include <chrono>
//#include <ctime>
//#include "ctime_elapse.h"
//#include "clib_util.h"
//#include <map>
//#include "clog.h"
//struct ctest
//{
//	int p1;
//	int p2;
//	int p3;
//	int p4;
//	int p5;
//	int p6;
//	int p7;
//	int p8;
//	int p9;
//	int p10;
//};
//
//class ctest_mem_pool
//{
//public:
//	ctest_mem_pool() {}
//	~ctest_mem_pool() {}
//	int get()const;
//	void show() const;
//	void set(int value) { m_value = value; }
//	int m_value;
//	ctest test1;
//	ctest test2;
//	ctest test3;
//	ctest test4;
//	ctest test5;
//	ctest test6;
//	ctest test7;
//	ctest test8;
//	ctest test9;
//	ctest test10;
//	ctest test11;
//	ctest test12;
//	ctest test13;
//	ctest test14;
//
//
//	ctest itest1;
//	ctest itest2;
//	ctest itest3;
//	ctest itest4;
//	ctest itest5;
//	ctest itest6;
//	ctest itest7;
//	ctest itest8;
//	ctest itest9;
//	ctest itest10;
//	ctest itest11;
//	ctest itest12;
//	ctest itest13;
//	ctest itest14;
//};
//#include <signal.h>
//void Stop(int i)
//{
//	using namespace chen;
//	LOG::destroy();
//}
//
//void RegisterSignal()
//{
//	signal(SIGINT, Stop);
//	signal(SIGTERM, Stop);
//}
//
//
//int main(int argc, char *argv[])
//{
//	RegisterSignal();
//	using namespace chen;
//	if (!LOG::init("./", "clib"))
//	{
//		system("PAUSE");
//		return -1;
//	}
//	chen::cmem_pool g_pool(true);
//	if (!g_pool.init())
//	{
//		ERROR_EX_LOG("init fail");
//		//system("PAUSE");
//		return -1;
//	}
//
//	g_pool.set_max_block(500);
//	std::map<int, ctest_mem_pool*>  m_data;
//	std::map<int, ctest*> m_test;
//	static const unsigned int TICK_TIME = 100;
//	
//
//	chen::ctime_elapse time_elapse;
//	unsigned int uDelta = 0;
//	int i = 1;
//	unsigned int test_size = sizeof(ctest_mem_pool);
//	SYSTEM_LOG("[test_size = %d]", test_size);
//	while (true)
//	{
//		uDelta += time_elapse.get_elapse();
//		SYSTEM_LOG("[udelta = %ld][i = %d]\n", uDelta, i);
//		
//		for (int j = 0; j < 1000000; ++j)
//		{
//			void * p = g_pool.alloc(sizeof(ctest_mem_pool));
//			if (!p)
//			{
//				ERROR_EX_LOG("[ERROR] j = %d", j);
//				continue;;
//			}
//			ctest_mem_pool* ptr = new(p) ctest_mem_pool();
//			ptr->set(j);
//			if (!m_data.insert(std::make_pair(j, ptr)).second)
//			{
//				ERROR_EX_LOG("[ERROR insert map fail j = %d]", j);
//				continue;
//			}
//			void * p1 = g_pool.alloc(sizeof(ctest));
//			if (!p1)
//			{
//				ERROR_EX_LOG("[ERROR] j = %d", j);
//				continue;;
//			}
//			ctest* ptr1 = new(p1) ctest();
//			if (!m_test.insert(std::make_pair(j, ptr1)).second)
//			{
//				ERROR_EX_LOG("[ERROR insert map fail j = %d]", j);
//				continue;
//			}
//			//g_pool.show_info();
//		}
//		SYSTEM_LOG("-----------------------------------------------------------------------------------------\n");
//		g_pool.show_info();
//		ctest_mem_pool* ptr = NULL;
//		for (std::map<int, ctest_mem_pool*>::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
//		{
//			if (iter->second)
//			{
//				/*ptr = iter->second;
//				m_data.erase(iter);*/
//				iter->second->get();
//				//ERROR_EX_LOG("[iter->first = %d][iter->second = %d]", iter->first, iter->second->get());
//				//printf("[iter->first = %d]\n", iter->first);
//				g_pool.free(iter->second);
//			}
//		}
//		SYSTEM_LOG("pool free end");
//		m_data.clear();
//
//		for (std::map<int, ctest*>::iterator iter = m_test.begin(); iter != m_test.end(); ++iter)
//		{
//			if (iter->second)
//			{
//				/*ptr = iter->second;
//				m_data.erase(iter);*/
//
//				//printf("[iter->first = %d]\n", iter->first);
//				g_pool.free(iter->second);
//			}
//		}
//		m_test.clear();
//		//g_pool.gc();
//		g_pool.show_info();
//		
//		
//		uDelta = time_elapse.get_elapse();
//
//		if (uDelta < TICK_TIME)
//		{
//			chen::csleep(TICK_TIME - uDelta);
//		}
//	}
//
//
//	LOG::destroy();
//
//
//	return EXIT_SUCCESS;
//}
//
//int ctest_mem_pool::get() const
//{
//	return m_value;
//}
//
//void ctest_mem_pool::show() const
//{
//}
