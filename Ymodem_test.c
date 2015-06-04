#include "Ymodem.h"

/**************************************************************************
 * 当通过[超时]+判断出串口接收到1串完整的包，相结合的方式调用
 * 第一次编译的时候，肯定会有很多错误，因为这里还包含了许多其它的函数包括，但不限于
 * pvPortMalloc()   FreeRTOS的动态内存申请函数
 * vPortFree()      FreeRTOS的动态内存释放函数
 * f_open()         FatFs的文件打开函数
 * f_close()        FatFs的文件关闭函数
 * f_read()         FatFs的文件读函数
 * f_write()        FatFs的文件写函数
 * cstr_create()    网上开源的动态字符串cstr_t的创建函数
 * cstr_free()      网上开源的动态字符串cstr_t的释放函数
 * usart_write()    自己实现的一个串口数据发送程序
***************************************************************************/

#define USART_PROTOCOL_DEFAULT         0
#define USART_PROTOCOL_YMODEM_UPLOAD   1
#define USART_PROTOCOL_YMODEM_DNLOAD   2

extern uint8 usart_protocol_model_cur;  //当前协议（上传/下载）
extern cstr_t        *px_path_cur;      //当前路径

void test( char *data, size_t len )
{
    static int tm_out;
    switch( usart_protocol_model_cur )
    {
    case USART_PROTOCOL_YMODEM_DNLOAD:
        ymodem_rx_put( (char*)data, len );
        tm_out = 0x5ffff;
    break;
    case USART_PROTOCOL_YMODEM_UPLOAD:
        ymodem_tx_put( (char*)data, len );
    break;
    }
    if( usart_protocol_model_cur == USART_PROTOCOL_YMODEM_DNLOAD )
      if( tm_out )
        tm_out--;
      else
      {ymodem_rx_put( NULL, 0 );tm_out = 0x5ffff;}
  }
}
