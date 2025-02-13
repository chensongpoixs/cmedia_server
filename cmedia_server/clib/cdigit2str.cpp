/********************************************************************
created:	2019-03-24

author:		chensong

purpose:	�ַ�����������
//__pragma(warning(suppress:6334))(sizeof ((_Val) + (float)0) == sizeof (float) ? 'f' : sizeof ((_Val) + (double)0) == sizeof (double) ? 'd' : 'l')
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
#include "cdigit2str.h"
#include <algorithm>
#include <cstdio>

#ifdef _MSC_VER
#pragma warning (disable:4996)
#endif
namespace chen
{
	template<>
	int32 digit2str_dec<int8>(char* buf, int32 buf_size, int8 value)
	{
		int8 i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		if (value < 0)
		{
			*p++ = '-';
		}
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}
	template<>
	int32 digit2str_dec<uint8>(char* buf, int32 buf_size, uint8 value)
	{
		uint8 i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		/*if (value < 0)
		{
		*p++ = '-';
		}*/
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}
	template<>
	int32 digit2str_dec<int16>(char* buf, int32 buf_size, int16 value)
	{
		int16 i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		if (value < 0)
		{
			*p++ = '-';
		}
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}
	template<>
	int32 digit2str_dec<uint16>(char* buf, int32 buf_size, uint16 value)
	{
		uint16 i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		/*if (value < 0)
		{
		*p++ = '-';
		}*/
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}
	template<>
	int32 digit2str_dec<int32>(char* buf, int32 buf_size, int32 value)
	{
		int32 i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		if (value < 0)
		{
			*p++ = '-';
		}
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}

	template<>
	int32 digit2str_dec<uint32>(char* buf, int32 buf_size, uint32 value)
	{
		uint32 i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		/*if (value < 0)
		{
		*p++ = '-';
		}*/
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}

	template<>
	int32 digit2str_dec<signed long >(char* buf, int32 buf_size, signed long value)
	{
		signed long i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		if (value < 0)
		{
			*p++ = '-';
		}
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}
	template<>
	int32 digit2str_dec<unsigned long>(char* buf, int32 buf_size, unsigned long value)
	{
		unsigned long i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		/*if (value < 0)
		{
		*p++ = '-';
		}*/
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}
	template<>
	int32 digit2str_dec<signed long  long>(char* buf, int32 buf_size, signed long long value)
	{
		signed long long i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		if (value < 0)
		{
			*p++ = '-';
		}
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}
	template<>
	int32 digit2str_dec<unsigned long long>(char* buf, int32 buf_size, unsigned long long value)
	{
		unsigned long long i = value;
		char* p = buf;
		int num = 0;
		do {
			i /= 10;
			++num;
		} while (i != 0);
		if (num > buf_size)
		{
			return 0;
		}
		i = value;
		do
		{
			*p++ = '0' + (i % 10);
			i /= 10;
		} while (i != 0);

		/*if (value < 0)
		{
		*p++ = '-';
		}*/
		*p = '\0';
		std::reverse(buf, p);

		return static_cast<int>(p - buf);
	}
	template<>
	int digit2str_dec<float>(char* buf, int buf_size, float value)
	{
		if (6 > buf_size)
		{
			return 0;
		}

		return sprintf(buf, "%.2g", value);
	}
	/**
	* ��ѧ������
	* stata �� ������ 1.333e+09 ����ʲô��˼
	* ��������̫���ˣ��ÿ�ѧ��������ʾ��
	* ����  �����ݴη��������1.333e-09����˵��������0�� ��1.33*10�ĸ�9�η�
	* ��1330000000��Ҳ���� 1.33 * (10^9)
	*/
	template<>
	int digit2str_dec<double>(char* buf, int buf_size, double value)
	{
		if (6 > buf_size)
		{
			return 0;
		}
		return sprintf(buf, "%.2g", value);
	}


} // namespace chen