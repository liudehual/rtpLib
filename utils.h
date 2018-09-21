#ifndef __UTIL_H__
#define __UTIL_H__


typedef struct bit_buffer
{
	int i_size;   
    int i_data;  
    unsigned i_mask; // �����ƣ�0x10000000 ������Ϊ�˺����һ���ֽڵ�ÿһλ���в����������С�˿��ֽ��������  
    unsigned char *p_data;  
}bits_buffer_s;


/*** 
 *@remark:  ����������ݰ���λһ��һ����ѹ������ 
 *@param :  buffer   [in]  ѹ�����ݵ�buffer 
 *          count    [in]  ��Ҫѹ������ռ��λ�� 
 *          bits     [in]  ѹ�����ֵ 
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
        p_buffer->i_mask >>= 1;         /*������һ���ֽڵ�һλ�󣬲����ڶ�λ*/\  
        if( p_buffer->i_mask == 0 )     /*ѭ����һ���ֽڵ�8λ�����¿�ʼ��һλ*/\  
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
