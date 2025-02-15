/********************************************************************
created:	2019-05-01

author:		chensong

purpose:	system log color

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

#include "clog_color.h"
#include <cstdio>
#if defined(_MSC_VER)
#include <windows.h>
#endif

namespace chen {
	clog_color::clog_color()
	{
#if defined(_MSC_VER)
		m_color_fg[ELCT_White] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		m_color_fg[ELCT_Red] = FOREGROUND_RED | FOREGROUND_INTENSITY;
		m_color_fg[ELCT_Pink] = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		m_color_fg[ELCT_Green] = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		m_color_fg[ELCT_Blue] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		m_color_fg[ELCT_Yellow] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

#elif defined(__GNUC__)

		enum ANSIFgTextAttr
		{
			FG_BLACK = 30, FG_RED, FG_GREEN, FG_YELLOW, FG_BLUE,
			FG_MAGENTA, FG_CYAN, FG_WHITE,
		};

		m_color_fg[ELCT_White] = FG_WHITE;
		m_color_fg[ELCT_Red] = FG_RED;
		m_color_fg[ELCT_Pink] = FG_CYAN;
		m_color_fg[ELCT_Green] = FG_GREEN;
		m_color_fg[ELCT_Blue] = FG_BLUE;
		m_color_fg[ELCT_Yellow] = FG_YELLOW;

#else
#pragma error "unknow platform!!!"

#endif
	}

	void clog_color::set_color(ELogColorType color)
	{
#if defined(_MSC_VER)
		::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), m_color_fg[color]);
#elif defined(__GNUC__)
		//fprintf(stdout, "\033[0;40;%dm", m_color_fg[color]);
		//static const X_CHAR* colorString[TBLUE+1] = {"","\033[22;31m","\033[22;32m","\033[01;33m","\033[0m","\033[01;37m","\033[01;34m",};
		//fputs(colorString[color],stdout); 
#endif
	}

} //namespace chen