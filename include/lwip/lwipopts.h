#ifndef __LWIPOPTS_MY_H__
#define __LWIPOPTS_MY_H__

#define NO_SYS 0 // 无操作系统

#define LWIP_NETCONN 1
#define LWIP_SOCKET 1
#define LWIP_STATS 1

#define MEM_ALIGNMENT 4 // STM32单片机是32位的单片机, 因此是4字节对齐的

#define SYS_LIGHTWEIGHT_PROT 1 // 不进行临界区保护

// 配置DHCP
#define LWIP_DHCP 1
#define LWIP_NETIF_HOSTNAME 1

// 配置DNS
#define LWIP_DNS 1
#define LWIP_RAND() ((u32_t)rand())

// WPA/WPA2认证需要用到lwip中的arc4, md5和sha1函数
// 需要修改各文件的第42行, 注释掉条件编译宏
#define LWIP_INCLUDED_POLARSSL_ARC4 1
#define LWIP_INCLUDED_POLARSSL_MD5 1
#define LWIP_INCLUDED_POLARSSL_SHA1 1

#endif
