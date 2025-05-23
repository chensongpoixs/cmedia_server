﻿/**

#include <iphlpapi.h>
#pragma comment ( lib, "iphlpapi.lib")



使用GetIfTable()获取各个端口信息的时候用到的 _MIB_IFTABLE结构:



typedef struct _MIB_IFTABLE {
  DWORD     dwNumEntries; //得到的端口个数
  MIB_IFROW table[ANY_SIZE]; //得到的各个端口的信息,这个结构才是重点
} MIB_IFTABLE, *PMIB_IFTABLE;





typedef struct _MIB_IFROW {
  WCHAR wszName[MAX_INTERFACE_NAME_LEN];
  DWORD dwIndex;
  DWORD dwType;
  DWORD dwMtu;
  DWORD dwSpeed;
  DWORD dwPhysAddrLen;
  BYTE  bPhysAddr[MAXLEN_PHYSADDR];
  DWORD dwAdminStatus;
  DWORD dwOperStatus;
  DWORD dwLastChange;
  DWORD dwInOctets;
  DWORD dwInUcastPkts;
  DWORD dwInNUcastPkts;
  DWORD dwInDiscards;
  DWORD dwInErrors;
  DWORD dwInUnknownProtos;
  DWORD dwOutOctets;
  DWORD dwOutUcastPkts;
  DWORD dwOutNUcastPkts;
  DWORD dwOutDiscards;
  DWORD dwOutErrors;
  DWORD dwOutQLen;
  DWORD dwDescrLen;
  BYTE  bDescr[MAXLEN_IFDESCR];
} MIB_IFROW, *PMIB_IFROW;


wzsName:包含了该接口的名字(多字节的),具体也不知道哈意思,就是一串数字,有懂的和我说一下

dwIndex:该接口的索引值,比如有多个网卡的时候,每个网卡都有一个索引值,是会随着网卡正在被使用的个数变化的

dwType:该接口的类型,这个类型是被IANA(是个什么协会吧)定义的,有以下几种:



Value	Meaning
IF_TYPE_OTHER 1
Some other type of network interface.

IF_TYPE_ETHERNET_CSMACD 6
An Ethernet network interface.

IF_TYPE_ISO88025_TOKENRING 9
A token ring network interface.

IF_TYPE_PPP 23
A PPP network interface.

IF_TYPE_SOFTWARE_LOOPBACK 24
A software loopback network interface.

IF_TYPE_ATM 37
An ATM network interface.

IF_TYPE_IEEE80211 71
An IEEE 802.11 wireless network interface.

IF_TYPE_TUNNEL 131
A tunnel type encapsulation network interface.

IF_TYPE_IEEE1394 144
An IEEE 1394 (Firewire) high performance serial bus network interface.





其中24是网络回路的网卡(我自己是这样叫的),就是127.0.0.1那个,应该是每个机子都有的吧

一般我们用的是6.

dwMtu:百度一下MTU就知道了,就是该接口的最大传输单元,理解为该通信协议的某一层上面能通过的最大的数据包的大小(以字节为单位)

dwSpeed:该接口最大的传输速率,可是看成是这个接口每秒最多传多大的数据的一个规格,我刚开始的时候以为这是该接口的即时传输速度呢,郁闷

dwPhysAddrLen: bPhysAddr指向的地址的长度

bPhysAddr:指向该接口地址的指针

dwAdminStatus:该接口的管理状态,按我的理解就是人为设定的那个状态:启用/禁用

dwOperStatus:该接口的操作状态,它可以取以下的值,看了下面的值就知道是什么意思了

0 MIB_IF_OPER_STATUS_NON_OPERATIONAL 网络适配器被禁止的状态;

1 MIB_IF_OPER_STATUS_UNREACHABLE 没有连接的状态;

2 MIB_IF_OPER_STATUS_DISCONNECTED 电缆未连接的状态;

3 MIB_IF_OPER_STATUS_CONNECTING 广域网适配器连接中的状态;

4 MIB_IF_OPER_STATUS_CONNECTED 广域网适配器连接上远程对等点的状态;

5 MIB_IF_OPER_STATUS_OPERATIONAL 局域网适配器默认的连接状态;

dwLastChange: 适配器状态最后一次改变的时间;

dwInOctets: 该接口总的收到的数据大小;

dwInUcastPkts As Long '总共收到(unicast包)
dwInNUcastPkts As Long '总共收到(non-unicast包)，包括广播包和多点传送包dwInDiscards As Long '收到后丢弃包总数（即使没有错误）
dwInErrors As Long '收到出错包总数
dwInUnknownProtos As Long '收到后因协议不明而丢弃的包总数
dwOutOctets As Long '总共发送(字节)
dwOutUcastPkts As Long '总共发送(unicast包)
dwOutNUcastPkts As Long '总共发送(non-unicast包)，包括广播包和多点传送包
dwOutDiscards As Long '发送丢弃包总数（即使没有错误）
dwOutErrors As Long '发送出错包总数
dwOutQLen As Long '发送队列长度
dwDescrLen As Long ' bDescr部分有效长度
bDescr(0 To 255) As Byte '接口描述 也就是在设备管理器上看的到名字





用一秒钟前后得到的dwInOctets数据相减,就是这一秒中该接口的流量,用这个方法就可以计算机子的即时流量了
*/

#ifndef SIGAR_H
#define SIGAR_H

/* System Information Gatherer And Reporter */

#include <limits.h>

#ifndef MAX_INTERFACE_NAME_LEN
#define MAX_INTERFACE_NAME_LEN 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_LP64)         || \
    defined(__LP64__)      || \
    defined(__64BIT__)     || \
    defined(__powerpc64__) || \
    defined(__osf__)
#define SIGAR_64BIT
#endif

/* for printf sigar_uint64_t */
#ifdef SIGAR_64BIT
# define SIGAR_F_U64 "%lu"
#else
# define SIGAR_F_U64 "%Lu"
#endif

#if defined(WIN32)

typedef unsigned __int32 sigar_uint32_t;

typedef unsigned __int64 sigar_uint64_t;

typedef __int32 sigar_int32_t;

typedef __int64 sigar_int64_t;

#elif ULONG_MAX > 4294967295UL

typedef unsigned int sigar_uint32_t;

typedef unsigned long sigar_uint64_t;

typedef int sigar_int32_t;

typedef long sigar_int64_t;

#else

typedef unsigned int sigar_uint32_t;

typedef unsigned long long sigar_uint64_t;

typedef int sigar_int32_t;

typedef long long sigar_int64_t;

#endif

#define SIGAR_FIELD_NOTIMPL -1

#define SIGAR_OK 0
#define SIGAR_START_ERROR 20000
#define SIGAR_ENOTIMPL       (SIGAR_START_ERROR + 1)
#define SIGAR_OS_START_ERROR (SIGAR_START_ERROR*2)

#ifdef WIN32
#   define SIGAR_ENOENT ERROR_FILE_NOT_FOUND
#   define SIGAR_EACCES ERROR_ACCESS_DENIED
#   define SIGAR_ENXIO  ERROR_BAD_DRIVER_LEVEL
#else
#   define SIGAR_ENOENT ENOENT
#   define SIGAR_EACCES EACCES
#   define SIGAR_ENXIO  ENXIO
#endif

#ifdef WIN32
#   define SIGAR_DECLARE(type) \
     type   // __declspec(dllexport) type __stdcall
#else
#   define SIGAR_DECLARE(type) type
#endif

#if defined(PATH_MAX)
#   define SIGAR_PATH_MAX PATH_MAX
#elif defined(MAXPATHLEN)
#   define SIGAR_PATH_MAX MAXPATHLEN
#else
#   define SIGAR_PATH_MAX 4096
#endif

#ifdef WIN32
typedef sigar_uint64_t sigar_pid_t;
typedef unsigned long sigar_uid_t;
typedef unsigned long sigar_gid_t;
#else
#include <sys/types.h>
typedef pid_t sigar_pid_t;
typedef uid_t sigar_uid_t;
typedef gid_t sigar_gid_t;
#endif

typedef struct sigar_t sigar_t;

SIGAR_DECLARE(int) sigar_open(sigar_t **sigar);

SIGAR_DECLARE(int) sigar_close(sigar_t *sigar);

SIGAR_DECLARE(sigar_pid_t) sigar_pid_get(sigar_t *sigar);

SIGAR_DECLARE(int) sigar_proc_kill(sigar_pid_t pid, int signum);

SIGAR_DECLARE(int) sigar_signum_get(char *name);

SIGAR_DECLARE(char *) sigar_strerror(sigar_t *sigar, int err);

/* system memory info */

typedef struct {
    sigar_uint64_t
        ram,
        total,
        used, 
        free,
        actual_used,
        actual_free;
    double used_percent;
    double free_percent;
} sigar_mem_t;

SIGAR_DECLARE(int) sigar_mem_get(sigar_t *sigar, sigar_mem_t *mem);

typedef struct {
    sigar_uint64_t
        total,
        used, 
        free,
        page_in,
        page_out;
} sigar_swap_t;

SIGAR_DECLARE(int) sigar_swap_get(sigar_t *sigar, sigar_swap_t *swap);

typedef struct {
    sigar_uint64_t
        user, 
        sys,
        nice,
        idle,
        wait,
        irq,
        soft_irq,
        stolen,
        total;
} sigar_cpu_t;

SIGAR_DECLARE(int) sigar_cpu_get(sigar_t *sigar, sigar_cpu_t *cpu);

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_cpu_t *data;
} sigar_cpu_list_t;

SIGAR_DECLARE(int) sigar_cpu_list_get(sigar_t *sigar, sigar_cpu_list_t *cpulist);

SIGAR_DECLARE(int) sigar_cpu_list_destroy(sigar_t *sigar,
                                          sigar_cpu_list_t *cpulist);

typedef struct {
    char vendor[128];
    char model[128];
    int mhz;
    int mhz_max;
    int mhz_min;
    sigar_uint64_t cache_size;
    int total_sockets;
    int total_cores;
    int cores_per_socket;
} sigar_cpu_info_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_cpu_info_t *data;
} sigar_cpu_info_list_t;

SIGAR_DECLARE(int)
sigar_cpu_info_list_get(sigar_t *sigar,
                        sigar_cpu_info_list_t *cpu_infos);

SIGAR_DECLARE(int)
sigar_cpu_info_list_destroy(sigar_t *sigar,
                            sigar_cpu_info_list_t *cpu_infos);

typedef struct {
    double uptime;
} sigar_uptime_t;

SIGAR_DECLARE(int) sigar_uptime_get(sigar_t *sigar,
                                    sigar_uptime_t *uptime);

typedef struct {
    double loadavg[3];
} sigar_loadavg_t;

SIGAR_DECLARE(int) sigar_loadavg_get(sigar_t *sigar,
                                     sigar_loadavg_t *loadavg);

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_pid_t *data;
} sigar_proc_list_t;

typedef struct {
    /* RLIMIT_CPU */
    sigar_uint64_t cpu_cur, cpu_max;
    /* RLIMIT_FSIZE */
    sigar_uint64_t file_size_cur, file_size_max;
    /* PIPE_BUF */
    sigar_uint64_t pipe_size_cur, pipe_size_max;
    /* RLIMIT_DATA */
    sigar_uint64_t data_cur, data_max;
    /* RLIMIT_STACK */
    sigar_uint64_t stack_cur, stack_max;
    /* RLIMIT_CORE */
    sigar_uint64_t core_cur, core_max;
    /* RLIMIT_RSS */
    sigar_uint64_t memory_cur, memory_max;
    /* RLIMIT_NPROC */
    sigar_uint64_t processes_cur, processes_max;
    /* RLIMIT_NOFILE */
    sigar_uint64_t open_files_cur, open_files_max;
    /* RLIMIT_AS */
    sigar_uint64_t virtual_memory_cur, virtual_memory_max;
} sigar_resource_limit_t;

SIGAR_DECLARE(int) sigar_resource_limit_get(sigar_t *sigar,
                                            sigar_resource_limit_t *rlimit);

SIGAR_DECLARE(int) sigar_proc_list_get(sigar_t *sigar,
                                       sigar_proc_list_t *proclist);

SIGAR_DECLARE(int) sigar_proc_list_destroy(sigar_t *sigar,
                                           sigar_proc_list_t *proclist);

typedef struct {
    sigar_uint64_t total;
    sigar_uint64_t sleeping;
    sigar_uint64_t running;
    sigar_uint64_t zombie;
    sigar_uint64_t stopped;
    sigar_uint64_t idle;
    sigar_uint64_t threads;
} sigar_proc_stat_t;

SIGAR_DECLARE(int) sigar_proc_stat_get(sigar_t *sigar,
                                       sigar_proc_stat_t *procstat);

typedef struct {
    sigar_uint64_t
        size,
        resident,
        share,
        minor_faults,
        major_faults,
        page_faults;
} sigar_proc_mem_t;

SIGAR_DECLARE(int) sigar_proc_mem_get(sigar_t *sigar, sigar_pid_t pid,
                                      sigar_proc_mem_t *procmem);

typedef struct {
     sigar_uint64_t 
        bytes_read,
        bytes_written,
        bytes_total;
} sigar_proc_disk_io_t;

SIGAR_DECLARE(int) sigar_proc_disk_io_get(sigar_t *sigar, sigar_pid_t pid,
                                          sigar_proc_disk_io_t *proc_disk_io);

typedef struct {
    sigar_uint64_t
        bytes_read,
        bytes_written,
        bytes_total;
    sigar_uint64_t last_time;
    sigar_uint64_t
        bytes_read_diff,
        bytes_written_diff,
        bytes_total_diff;
} sigar_cached_proc_disk_io_t;


typedef struct {
    sigar_uint64_t
        bytes_read,
        bytes_written,
        bytes_total;
} sigar_proc_cumulative_disk_io_t;

SIGAR_DECLARE(int) sigar_proc_cumulative_disk_io_get(sigar_t *sigar, sigar_pid_t pid,
                                          sigar_proc_cumulative_disk_io_t *proc_cumulative_disk_io);


typedef struct  { 
    sigar_uint64_t dummy;
}sigar_dump_pid_cache_t;

SIGAR_DECLARE(int) sigar_dump_pid_cache_get(sigar_t *sigar, sigar_dump_pid_cache_t *info);


typedef struct {
    sigar_uid_t uid;
    sigar_gid_t gid;
    sigar_uid_t euid;
    sigar_gid_t egid;
} sigar_proc_cred_t;

SIGAR_DECLARE(int) sigar_proc_cred_get(sigar_t *sigar, sigar_pid_t pid,
                                       sigar_proc_cred_t *proccred);

#define SIGAR_CRED_NAME_MAX 512

typedef struct {
    char user[SIGAR_CRED_NAME_MAX];
    char group[SIGAR_CRED_NAME_MAX];
} sigar_proc_cred_name_t;

SIGAR_DECLARE(int)
sigar_proc_cred_name_get(sigar_t *sigar, sigar_pid_t pid,
                         sigar_proc_cred_name_t *proccredname);

typedef struct {
    sigar_uint64_t
        start_time,
        user,
        sys,
        total;
} sigar_proc_time_t;

SIGAR_DECLARE(int) sigar_proc_time_get(sigar_t *sigar, sigar_pid_t pid,
                                       sigar_proc_time_t *proctime);

typedef struct {
    /* must match sigar_proc_time_t fields */
    sigar_uint64_t
        start_time,
        user,
        sys,
        total;
    sigar_uint64_t last_time;
    double percent;
} sigar_proc_cpu_t;

SIGAR_DECLARE(int) sigar_proc_cpu_get(sigar_t *sigar, sigar_pid_t pid,
                                      sigar_proc_cpu_t *proccpu);

#define SIGAR_PROC_STATE_SLEEP  'S'
#define SIGAR_PROC_STATE_RUN    'R'
#define SIGAR_PROC_STATE_STOP   'T'
#define SIGAR_PROC_STATE_ZOMBIE 'Z'
#define SIGAR_PROC_STATE_IDLE   'D'

#define SIGAR_PROC_NAME_LEN 128

typedef struct {
    char name[SIGAR_PROC_NAME_LEN];
    char state;
    sigar_pid_t ppid;
    int tty;
    int priority;
    int nice;
    int processor;
    sigar_uint64_t threads;
} sigar_proc_state_t;

SIGAR_DECLARE(int) sigar_proc_state_get(sigar_t *sigar, sigar_pid_t pid,
                                        sigar_proc_state_t *procstate);

typedef struct {
    unsigned long number;
    unsigned long size;
    char **data;
} sigar_proc_args_t;

SIGAR_DECLARE(int) sigar_proc_args_get(sigar_t *sigar, sigar_pid_t pid,
                                       sigar_proc_args_t *procargs);

SIGAR_DECLARE(int) sigar_proc_args_destroy(sigar_t *sigar,
                                           sigar_proc_args_t *procargs);

typedef struct {
    void *data; /* user data */

    enum {
        SIGAR_PROC_ENV_ALL,
        SIGAR_PROC_ENV_KEY
    } type;

    /* used for SIGAR_PROC_ENV_KEY */
    const char *key;
    int klen;
    
    int (*env_getter)(void *, const char *, int, char *, int);
} sigar_proc_env_t;

SIGAR_DECLARE(int) sigar_proc_env_get(sigar_t *sigar, sigar_pid_t pid,
                                      sigar_proc_env_t *procenv);

typedef struct {
    sigar_uint64_t total;
    /* XXX - which are files, sockets, etc. */
} sigar_proc_fd_t;

SIGAR_DECLARE(int) sigar_proc_fd_get(sigar_t *sigar, sigar_pid_t pid,
                                     sigar_proc_fd_t *procfd);

typedef struct {
    char name[SIGAR_PATH_MAX+1];
    char cwd[SIGAR_PATH_MAX+1];
    char root[SIGAR_PATH_MAX+1];
} sigar_proc_exe_t;

SIGAR_DECLARE(int) sigar_proc_exe_get(sigar_t *sigar, sigar_pid_t pid,
                                      sigar_proc_exe_t *procexe);

typedef struct {
    void *data; /* user data */

    int (*module_getter)(void *, char *, int);
} sigar_proc_modules_t;

SIGAR_DECLARE(int) sigar_proc_modules_get(sigar_t *sigar, sigar_pid_t pid,
                                          sigar_proc_modules_t *procmods);

typedef struct {
    sigar_uint64_t user;
    sigar_uint64_t sys;
    sigar_uint64_t total;
} sigar_thread_cpu_t;

SIGAR_DECLARE(int) sigar_thread_cpu_get(sigar_t *sigar,
                                        sigar_uint64_t id,
                                        sigar_thread_cpu_t *cpu);
                                            
typedef enum {
    SIGAR_FSTYPE_UNKNOWN,
    SIGAR_FSTYPE_NONE,
    SIGAR_FSTYPE_LOCAL_DISK,
    SIGAR_FSTYPE_NETWORK,
    SIGAR_FSTYPE_RAM_DISK,
    SIGAR_FSTYPE_CDROM,
    SIGAR_FSTYPE_SWAP,
    SIGAR_FSTYPE_MAX
} sigar_file_system_type_e;

#define SIGAR_FS_NAME_LEN SIGAR_PATH_MAX
#define SIGAR_FS_INFO_LEN 256

typedef struct {
    char dir_name[SIGAR_FS_NAME_LEN];
    char dev_name[SIGAR_FS_NAME_LEN];
    char type_name[SIGAR_FS_INFO_LEN];     /* e.g. "local" */
    char sys_type_name[SIGAR_FS_INFO_LEN]; /* e.g. "ext3" */
    char options[SIGAR_FS_INFO_LEN];
    sigar_file_system_type_e type;
    unsigned long flags;
} sigar_file_system_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_file_system_t *data;
} sigar_file_system_list_t;

SIGAR_DECLARE(int)
sigar_file_system_list_get(sigar_t *sigar,
                           sigar_file_system_list_t *fslist);

SIGAR_DECLARE(int)
sigar_file_system_list_destroy(sigar_t *sigar,
                               sigar_file_system_list_t *fslist);

typedef struct {
    sigar_uint64_t reads;
    sigar_uint64_t writes;
    sigar_uint64_t write_bytes;
    sigar_uint64_t read_bytes;
    sigar_uint64_t rtime;
    sigar_uint64_t wtime;
    sigar_uint64_t qtime;
    sigar_uint64_t time;
    sigar_uint64_t snaptime;
    double service_time;
    double queue;
} sigar_disk_usage_t;

/* XXX for sigar_file_system_usage_t compat */
#define disk_reads disk.reads
#define disk_writes disk.writes
#define disk_write_bytes disk.write_bytes
#define disk_read_bytes disk.read_bytes
#define disk_queue disk.queue
#define disk_service_time disk.service_time

typedef struct {
    sigar_disk_usage_t disk;
    double use_percent;
    sigar_uint64_t total;
    sigar_uint64_t free;
    sigar_uint64_t used;
    sigar_uint64_t avail;
    sigar_uint64_t files;
    sigar_uint64_t free_files;
} sigar_file_system_usage_t;

#undef SIGAR_DISK_USAGE_T

SIGAR_DECLARE(int)
sigar_file_system_usage_get(sigar_t *sigar,
                            const char *dirname,
                            sigar_file_system_usage_t *fsusage);

SIGAR_DECLARE(int) sigar_disk_usage_get(sigar_t *sigar,
                                        const char *name,
                                        sigar_disk_usage_t *disk);

SIGAR_DECLARE(int)
sigar_file_system_ping(sigar_t *sigar,
                       sigar_file_system_t *fs);

typedef struct {
    enum {
        SIGAR_AF_UNSPEC,
        SIGAR_AF_INET,
        SIGAR_AF_INET6,
        SIGAR_AF_LINK
    } family;
    union {
        sigar_uint32_t in;
        sigar_uint32_t in6[4];
        unsigned char mac[8];
    } addr;
} sigar_net_address_t;

#define SIGAR_INET6_ADDRSTRLEN 46

#define SIGAR_MAXDOMAINNAMELEN 256
#define SIGAR_MAXHOSTNAMELEN 256

typedef struct {
    char default_gateway[SIGAR_INET6_ADDRSTRLEN];
    char default_gateway_interface[MAX_INTERFACE_NAME_LEN];
    char host_name[SIGAR_MAXHOSTNAMELEN];
    char domain_name[SIGAR_MAXDOMAINNAMELEN];
    char primary_dns[SIGAR_INET6_ADDRSTRLEN];
    char secondary_dns[SIGAR_INET6_ADDRSTRLEN];
} sigar_net_info_t;

SIGAR_DECLARE(int)
sigar_net_info_get(sigar_t *sigar,
                   sigar_net_info_t *netinfo);

#define SIGAR_RTF_UP      0x1
#define SIGAR_RTF_GATEWAY 0x2
#define SIGAR_RTF_HOST    0x4

typedef struct {
    sigar_net_address_t destination;
    sigar_net_address_t gateway;
    sigar_net_address_t mask;
    sigar_uint64_t
        flags,
        refcnt,
        use,
        metric,
        mtu,
        window,
        irtt;
    char ifname[MAX_INTERFACE_NAME_LEN];
} sigar_net_route_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_net_route_t *data;
} sigar_net_route_list_t;

SIGAR_DECLARE(int) sigar_net_route_list_get(sigar_t *sigar,
                                            sigar_net_route_list_t *routelist);

SIGAR_DECLARE(int) sigar_net_route_list_destroy(sigar_t *sigar,
                                                sigar_net_route_list_t *routelist);

/*
 * platforms define most of these "standard" flags,
 * but of course, with different values in some cases.
 */
#define SIGAR_IFF_UP          0x1
#define SIGAR_IFF_BROADCAST   0x2
#define SIGAR_IFF_DEBUG       0x4
#define SIGAR_IFF_LOOPBACK    0x8
#define SIGAR_IFF_POINTOPOINT 0x10
#define SIGAR_IFF_NOTRAILERS  0x20
#define SIGAR_IFF_RUNNING     0x40
#define SIGAR_IFF_NOARP       0x80
#define SIGAR_IFF_PROMISC     0x100
#define SIGAR_IFF_ALLMULTI    0x200
#define SIGAR_IFF_MULTICAST   0x800
#define SIGAR_IFF_SLAVE       0x1000
#define SIGAR_IFF_MASTER      0x2000
#define SIGAR_IFF_DYNAMIC     0x4000

#define SIGAR_NULL_HWADDR "00:00:00:00:00:00"

/* scope values from linux-2.6/include/net/ipv6.h */
#define SIGAR_IPV6_ADDR_ANY        0x0000
#define SIGAR_IPV6_ADDR_UNICAST    0x0001
#define SIGAR_IPV6_ADDR_MULTICAST  0x0002
#define SIGAR_IPV6_ADDR_LOOPBACK   0x0010
#define SIGAR_IPV6_ADDR_LINKLOCAL  0x0020
#define SIGAR_IPV6_ADDR_SITELOCAL  0x0040
#define SIGAR_IPV6_ADDR_COMPATv4   0x0080

typedef struct {
    char name[MAX_INTERFACE_NAME_LEN];
    char type[64];
    char description[256];
    sigar_net_address_t hwaddr;
    sigar_net_address_t address;
    sigar_net_address_t destination;
    sigar_net_address_t broadcast;
    sigar_net_address_t netmask;
    sigar_net_address_t address6;
    int prefix6_length;
    int scope6;
    sigar_uint64_t
        flags,
        mtu,
        metric;
    int tx_queue_len;
} sigar_net_interface_config_t;

SIGAR_DECLARE(int)
sigar_net_interface_config_get(sigar_t *sigar,
                               const char *name,
                               sigar_net_interface_config_t *ifconfig);

SIGAR_DECLARE(int)
sigar_net_interface_config_primary_get(sigar_t *sigar,
                                       sigar_net_interface_config_t *ifconfig);

typedef struct {
    sigar_uint64_t
        /* received */
        rx_packets,
        rx_bytes,
        rx_errors,
        rx_dropped,
        rx_overruns,
        rx_frame,
        /* transmitted */
        tx_packets,
        tx_bytes,
        tx_errors,
        tx_dropped,
        tx_overruns,
        tx_collisions,
        tx_carrier,
        speed;
} sigar_net_interface_stat_t;

SIGAR_DECLARE(int)
sigar_net_interface_stat_get(sigar_t *sigar,
                             const char *name,
                             sigar_net_interface_stat_t *ifstat);

typedef struct {
    unsigned long number;
    unsigned long size;
    char **data;
} sigar_net_interface_list_t;

SIGAR_DECLARE(int)
sigar_net_interface_list_get(sigar_t *sigar,
                             sigar_net_interface_list_t *iflist);

SIGAR_DECLARE(int)
sigar_net_interface_list_destroy(sigar_t *sigar,
                                 sigar_net_interface_list_t *iflist);

#define SIGAR_NETCONN_CLIENT 0x01
#define SIGAR_NETCONN_SERVER 0x02

#define SIGAR_NETCONN_TCP  0x10
#define SIGAR_NETCONN_UDP  0x20
#define SIGAR_NETCONN_RAW  0x40
#define SIGAR_NETCONN_UNIX 0x80

enum {
    SIGAR_TCP_ESTABLISHED = 1,
    SIGAR_TCP_SYN_SENT,
    SIGAR_TCP_SYN_RECV,
    SIGAR_TCP_FIN_WAIT1,
    SIGAR_TCP_FIN_WAIT2,
    SIGAR_TCP_TIME_WAIT,
    SIGAR_TCP_CLOSE,
    SIGAR_TCP_CLOSE_WAIT,
    SIGAR_TCP_LAST_ACK,
    SIGAR_TCP_LISTEN,
    SIGAR_TCP_CLOSING,
    SIGAR_TCP_IDLE,
    SIGAR_TCP_BOUND,
    SIGAR_TCP_UNKNOWN
};

typedef struct {
    unsigned long local_port;
    sigar_net_address_t local_address;
    unsigned long remote_port;
    sigar_net_address_t remote_address;
    sigar_uid_t uid;
    unsigned long inode;
    int type;
    int state;
    unsigned long send_queue;
    unsigned long receive_queue;
} sigar_net_connection_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_net_connection_t *data;
} sigar_net_connection_list_t;

SIGAR_DECLARE(int)
sigar_net_connection_list_get(sigar_t *sigar,
                              sigar_net_connection_list_t *connlist,
                              int flags);

SIGAR_DECLARE(int)
sigar_net_connection_list_destroy(sigar_t *sigar,
                                  sigar_net_connection_list_t *connlist);

typedef struct sigar_net_connection_walker_t sigar_net_connection_walker_t;

/* alternative to sigar_net_connection_list_get */
struct sigar_net_connection_walker_t {
    sigar_t *sigar;
    int flags;
    void *data; /* user data */
    int (*add_connection)(sigar_net_connection_walker_t *walker,
                          sigar_net_connection_t *connection);
};

SIGAR_DECLARE(int)
sigar_net_connection_walk(sigar_net_connection_walker_t *walker);

typedef struct {
    int tcp_states[SIGAR_TCP_UNKNOWN];
    sigar_uint32_t tcp_inbound_total;
    sigar_uint32_t tcp_outbound_total;
    sigar_uint32_t all_inbound_total;
    sigar_uint32_t all_outbound_total;
} sigar_net_stat_t;

SIGAR_DECLARE(int)
sigar_net_stat_get(sigar_t *sigar,
                   sigar_net_stat_t *netstat,
                   int flags);

SIGAR_DECLARE(int)
sigar_net_stat_port_get(sigar_t *sigar,
                        sigar_net_stat_t *netstat,
                        int flags,
                        sigar_net_address_t *address,
                        unsigned long port);

/* TCP-MIB */
typedef struct {
    sigar_uint64_t active_opens;
    sigar_uint64_t passive_opens;
    sigar_uint64_t attempt_fails;
    sigar_uint64_t estab_resets;
    sigar_uint64_t curr_estab;
    sigar_uint64_t in_segs;
    sigar_uint64_t out_segs;
    sigar_uint64_t retrans_segs;
    sigar_uint64_t in_errs;
    sigar_uint64_t out_rsts;
} sigar_tcp_t;

SIGAR_DECLARE(int)
sigar_tcp_get(sigar_t *sigar,
              sigar_tcp_t *tcp);

typedef struct {
    sigar_uint64_t null;
    sigar_uint64_t getattr;
    sigar_uint64_t setattr;
    sigar_uint64_t root;
    sigar_uint64_t lookup;
    sigar_uint64_t readlink;
    sigar_uint64_t read;
    sigar_uint64_t writecache;
    sigar_uint64_t write;
    sigar_uint64_t create;
    sigar_uint64_t remove;
    sigar_uint64_t rename;
    sigar_uint64_t link;
    sigar_uint64_t symlink;
    sigar_uint64_t mkdir;
    sigar_uint64_t rmdir;
    sigar_uint64_t readdir;
    sigar_uint64_t fsstat;
} sigar_nfs_v2_t;

typedef sigar_nfs_v2_t sigar_nfs_client_v2_t;
typedef sigar_nfs_v2_t sigar_nfs_server_v2_t;

SIGAR_DECLARE(int)
sigar_nfs_client_v2_get(sigar_t *sigar,
                        sigar_nfs_client_v2_t *nfs);

SIGAR_DECLARE(int)
sigar_nfs_server_v2_get(sigar_t *sigar,
                        sigar_nfs_server_v2_t *nfs);

typedef struct {
    sigar_uint64_t null;
    sigar_uint64_t getattr;
    sigar_uint64_t setattr;
    sigar_uint64_t lookup;
    sigar_uint64_t access;
    sigar_uint64_t readlink;
    sigar_uint64_t read;
    sigar_uint64_t write;
    sigar_uint64_t create;
    sigar_uint64_t mkdir;
    sigar_uint64_t symlink;
    sigar_uint64_t mknod;
    sigar_uint64_t remove;
    sigar_uint64_t rmdir;
    sigar_uint64_t rename;
    sigar_uint64_t link;
    sigar_uint64_t readdir;
    sigar_uint64_t readdirplus;
    sigar_uint64_t fsstat;
    sigar_uint64_t fsinfo;
    sigar_uint64_t pathconf;
    sigar_uint64_t commit;
} sigar_nfs_v3_t;

typedef sigar_nfs_v3_t sigar_nfs_client_v3_t;
typedef sigar_nfs_v3_t sigar_nfs_server_v3_t;

SIGAR_DECLARE(int)
sigar_nfs_client_v3_get(sigar_t *sigar,
                        sigar_nfs_client_v3_t *nfs);

SIGAR_DECLARE(int)
sigar_nfs_server_v3_get(sigar_t *sigar,
                        sigar_nfs_server_v3_t *nfs);

SIGAR_DECLARE(int)
sigar_net_listen_address_get(sigar_t *sigar,
                             unsigned long port,
                             sigar_net_address_t *address);

typedef struct {
    char ifname[MAX_INTERFACE_NAME_LEN];
    char type[64];
    sigar_net_address_t hwaddr;
    sigar_net_address_t address;
    sigar_uint64_t flags;
} sigar_arp_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_arp_t *data;
} sigar_arp_list_t;

SIGAR_DECLARE(int) sigar_arp_list_get(sigar_t *sigar,
                                      sigar_arp_list_t *arplist);

SIGAR_DECLARE(int) sigar_arp_list_destroy(sigar_t *sigar,
                                          sigar_arp_list_t *arplist);

typedef struct {
    char user[32];
    char device[32];
    char host[256];
    sigar_uint64_t time;
} sigar_who_t;

typedef struct {
    unsigned long number;
    unsigned long size;
    sigar_who_t *data;
} sigar_who_list_t;

SIGAR_DECLARE(int) sigar_who_list_get(sigar_t *sigar,
                                      sigar_who_list_t *wholist);

SIGAR_DECLARE(int) sigar_who_list_destroy(sigar_t *sigar,
                                          sigar_who_list_t *wholist);

SIGAR_DECLARE(int) sigar_proc_port_get(sigar_t *sigar, 
                                       int protocol, unsigned long port,
                                       sigar_pid_t *pid);

typedef struct {
    const char *build_date;
    const char *scm_revision;
    const char *version;
    const char *archname;
    const char *archlib;
    const char *binname;
    const char *description;
    int major, minor, maint, build;
} sigar_version_t;

SIGAR_DECLARE(sigar_version_t *) sigar_version_get(void);

#define SIGAR_SYS_INFO_LEN SIGAR_MAXHOSTNAMELEN /* more than enough */

typedef struct {
    char name[SIGAR_SYS_INFO_LEN]; /* canonicalized sysname */
    char version[SIGAR_SYS_INFO_LEN]; /* utsname.release */
    char arch[SIGAR_SYS_INFO_LEN];
    char machine[SIGAR_SYS_INFO_LEN];
    char description[SIGAR_SYS_INFO_LEN];
    char patch_level[SIGAR_SYS_INFO_LEN];
    char vendor[SIGAR_SYS_INFO_LEN];
    char vendor_version[SIGAR_SYS_INFO_LEN];
    char vendor_name[SIGAR_SYS_INFO_LEN];  /* utsname.sysname */
    char vendor_code_name[SIGAR_SYS_INFO_LEN];
} sigar_sys_info_t;

SIGAR_DECLARE(int) sigar_sys_info_get(sigar_t *sigar, sigar_sys_info_t *sysinfo);

#define SIGAR_FQDN_LEN 512

SIGAR_DECLARE(int) sigar_fqdn_get(sigar_t *sigar, char *name, int namelen);

SIGAR_DECLARE(int) sigar_rpc_ping(char *hostname,
                                  int protocol,
                                  unsigned long program,
                                  unsigned long version);

SIGAR_DECLARE(char *) sigar_rpc_strerror(int err);

SIGAR_DECLARE(char *) sigar_password_get(const char *prompt);

#ifdef __cplusplus
}
#endif

#endif
