//#define PACK_STRUCT_BEGIN __packed // struct前的__packed
#define ERRNO
#define LWIP_PROVIDE_ERRNO

#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

//typedef unsigned long   mem_ptr_t;
typedef int sys_prot_t;
