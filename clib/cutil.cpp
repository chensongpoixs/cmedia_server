/***********************************************************************************************
    created:         2019-03-06
    
    author:            chensong
                    
    purpose:        ccrypto
************************************************************************************************/
#include "cutil.h"
#include <sstream>
#include <Windows.h>
#include <winnt.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <windows.h>
#include <tchar.h>
#include "cbase64.h"

#include <windows.h>
namespace chen
{
	namespace cutil
	{

		static const char HEX[16] = {
			'0', '1', '2', '3',
			'4', '5', '6', '7',
			'8', '9', 'a', 'b',
			'c', 'd', 'e', 'f'
		};

		std::string get_hex_str(const void *_buf, int num)
		{
			std::string str;
			str.reserve(num << 1);
			const unsigned char* buf = (const unsigned char*)_buf;

			unsigned char tmp;
			for (int i = 0; i < num; ++i)
			{
				tmp = buf[i];
				str.append(1, HEX[tmp / 16]);
				str.append(1, HEX[tmp % 16]);
			}
			return str;
		}
	}
	namespace system_info
	{
        /**
         * @brief GetDiskByCmd    获取windows下的硬盘序列号
         * @param ider            获取到的硬盘序列号的字符串变量
         * @return                true：获取成功  false：获取失败
        */
        bool GetDiskByCmd(std::string& ider)
        {
            //硬盘序列号
            const long MAX_COMMAND_SIZE = 64;                          //命令行输出缓冲大小
            CHAR szFetCmd[] = "wmic diskdrive get serialnumber";     //获取CPU序列号命令行
            const std::string strEnSearch = "SerialNumber";                 //CPU序列号前导信息
            BOOL bret = FALSE;
            HANDLE hReadPipe = NULL;                                   //读取管道
            HANDLE hWritePipe = NULL;                                  //写入管道
            PROCESS_INFORMATION pi;                                    //进程信息
            STARTUPINFO si;                                            //控制命令行窗口信息
            SECURITY_ATTRIBUTES sa;                                    //安全属性

            char szBuffer[MAX_COMMAND_SIZE + 1] = { 0 };                 //放置命令行结果的输出缓存区
            std::string strBuffer;
            unsigned long count = 0;
            long ipos = 0;

            memset(&pi, 0, sizeof(pi));
            memset(&si, 0, sizeof(si));
            memset(&sa, 0, sizeof(sa));

            //初始化管道
            pi.hProcess = NULL;
            pi.hThread = NULL;
            si.cb = sizeof(STARTUPINFO);
            sa.nLength = sizeof(SECURITY_ATTRIBUTES);
            sa.lpSecurityDescriptor = NULL;
            sa.bInheritHandle = TRUE;

            //创建管道
            bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
            if (!bret) {
                CloseHandle(hWritePipe);
                CloseHandle(hReadPipe);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            //设置命令行窗口的信息为指定的读写管道
            GetStartupInfo(&si);
            si.hStdError = hWritePipe;
            si.hStdOutput = hWritePipe;
            si.wShowWindow = SW_HIDE;           //隐藏命令行窗口
            si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

            //创建获取命令行的进程
            bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
            if (!bret) {
                CloseHandle(hWritePipe);
                CloseHandle(hReadPipe);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            //读取返回的数据
            WaitForSingleObject(pi.hProcess, 500);
            bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
            if (!bret) {
                CloseHandle(hWritePipe);
                CloseHandle(hReadPipe);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            //查找CPU序列号
            bret = FALSE;
            strBuffer = szBuffer;
            ipos = strBuffer.find(strEnSearch);
            if (ipos < 0) {	//没有找到
                CloseHandle(hWritePipe);
                CloseHandle(hReadPipe);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            else {
                strBuffer = strBuffer.substr(ipos + strEnSearch.length());
            }
            memset(szBuffer, 0x00, sizeof(szBuffer));
            strcpy_s(szBuffer, strBuffer.c_str());

            //去掉中间的空格\r\n
            char temp[512];
            memset(temp, 0, sizeof(temp));
            int index = 0;
            for (size_t i = 0; i < strBuffer.size(); i++) {
                if (strBuffer[i] != ' ' && strBuffer[i] != '\n' && strBuffer[i] != '\r') {
                    temp[index] = strBuffer[i];
                    index++;
                }
            }
            ider = temp;
            return true;
        }

        /**
         * @brief GetCpuByCmd     获取windows下的CPU序列号
         * @param ider            获取到的CPU序列号的字符串变量
         * @return                true：获取成功  false：获取失败
        */
        bool GetCpuByCmd(std::string& ider)
        {
            //CPU序列号
            const long MAX_COMMAND_SIZE = 64;                   //命令行输出缓冲大小
            CHAR szFetCmd[] = "wmic cpu get processorid";     //获取CPU序列号命令行
            const std::string strEnSearch = "ProcessorId";           //CPU序列号前导信息
            BOOL bret = FALSE;
            HANDLE hReadPipe = NULL;                            //读取管道
            HANDLE hWritePipe = NULL;                           //写入管道
            PROCESS_INFORMATION pi;                             //进程信息
            STARTUPINFO si;                                     //控制命令行窗口信息
            SECURITY_ATTRIBUTES sa;                             //安全属性

            char szBuffer[MAX_COMMAND_SIZE + 1] = { 0 };          //放置命令行结果的输出缓存区
            std::string strBuffer;
            unsigned long count = 0;
            long ipos = 0;

            memset(&pi, 0, sizeof(pi));
            memset(&si, 0, sizeof(si));
            memset(&sa, 0, sizeof(sa));

            //初始化管道
            pi.hProcess = NULL;
            pi.hThread = NULL;
            si.cb = sizeof(STARTUPINFO);
            sa.nLength = sizeof(SECURITY_ATTRIBUTES);
            sa.lpSecurityDescriptor = NULL;
            sa.bInheritHandle = TRUE;

            //创建管道
            bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
            if (!bret) {
                CloseHandle(hWritePipe);
                CloseHandle(hReadPipe);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            //设置命令行窗口的信息为指定的读写管道
            GetStartupInfo(&si);
            si.hStdError = hWritePipe;
            si.hStdOutput = hWritePipe;
            si.wShowWindow = SW_HIDE;           //隐藏命令行窗口
            si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

            //创建获取命令行的进程
            bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
            if (!bret) {
                CloseHandle(hWritePipe);
                CloseHandle(hReadPipe);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            //读取返回的数据
            WaitForSingleObject(pi.hProcess, 500);
            bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
            if (!bret) {
                CloseHandle(hWritePipe);
                CloseHandle(hReadPipe);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            //查找CPU序列号
            bret = FALSE;
            strBuffer = szBuffer;
            ipos = strBuffer.find(strEnSearch);
            if (ipos < 0) {	//没有找到
                CloseHandle(hWritePipe);
                CloseHandle(hReadPipe);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            else {
                strBuffer = strBuffer.substr(ipos + strEnSearch.length());
            }
            memset(szBuffer, 0x00, sizeof(szBuffer));
            strcpy_s(szBuffer, strBuffer.c_str());

            //去掉中间的空格\r\n
            char temp[512];
            memset(temp, 0, sizeof(temp));
            int index = 0;
            for (size_t i = 0; i < strBuffer.size(); i++) {
                if (strBuffer[i] != ' ' && strBuffer[i] != '\n' && strBuffer[i] != '\r') {
                    temp[index] = strBuffer[i];
                    index++;
                }
            }
            ider = temp;
            return true;
        }
        bool GetCpuDiskNumber(std::string& number)
        {
            std::string cpu_number;
            std::string disk_number;
            if (system_info::GetCpuByCmd(cpu_number) &&
                system_info::GetDiskByCmd(disk_number))
            {
               std::string new_number = cpu_number + disk_number;
               std::string _number = base64_encode(new_number);
               number = _number.substr((_number.length() / 2 -1), _number.length() / 2);
               number = _number.substr(0, _number.length() / 2);
                return true;
            }
            return false;
        }
	}
}
