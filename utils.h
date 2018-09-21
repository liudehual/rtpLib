#ifndef __UTIL_H__
#define __UTIL_H__


typedef struct bit_buffer
{
	int i_size;   
    int i_data;  
    unsigned i_mask; // 二进制：0x10000000 这里是为了后面对一个字节的每一位进行操作，避免大小端夸字节字序错乱  
    unsigned char *p_data;  
}bits_buffer_s;


/*** 
 *@remark:  讲传入的数据按地位一个一个的压入数据 
 *@param :  buffer   [in]  压入数据的buffer 
 *          count    [in]  需要压入数据占的位数 
 *          bits     [in]  压入的数值 
 */  
#define bitsWrite(buffer, count, bits)\  
{\  
    bits_buffer_s *p_buffer = (buffer);\  
    int i_count = (count);\  
    uint64_t i_bits = (bits);\  
    while( i_count > 0 )\  
    {\  
        i_count--;\  
        if( ( i_bits >> i_count )&0x01 )\  
        {\  
            p_buffer->p_data[p_buffer->i_data] |= p_buffer->i_mask;\  
        }\  
        else\  
        {\  
            p_buffer->p_data[p_buffer->i_data] &= ~p_buffer->i_mask;\  
        }\  
        p_buffer->i_mask >>= 1;         /*操作完一个字节第一位后，操作第二位*/\  
        if( p_buffer->i_mask == 0 )     /*循环完一个字节的8位后，重新开始下一位*/\  
        {\  
            p_buffer->i_data++;\  
            p_buffer->i_mask = 0x80;\  
        }\  
    }\  
}  

long long milliSeconds();
long getObjectId(void *objectPtr);

#define my_min(a, b) (((a) < (b))? (a) : (b))
#define my_max(a, b) (((a) < (b))? (b) : (a))
unsigned int stringIpToUInt32(char * ip);
void UInt32ToStringIp(unsigned int uIntIp,char *strIp);

#endif
