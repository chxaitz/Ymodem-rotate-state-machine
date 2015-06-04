# Ymodem -基于状态机轮转的单片机传输库

------
# 1.介绍
Ymodem是一个基于状态机轮转的非死循环的传输协议。你所要做的就是，在完成几个底层接口之后，只需要在上层，接收数据之后，根据当前是下载数据还是上传数据，相应的调用ymodem_rx_put( char *buf, size_t len);或ymodem_tx_put( char *buf, size_t len);函数即可。他的主要特征

- 标准C，方便移植；
- 非死循环，可用于非OS的嵌入式环境下，在后台完成传输操作；

# 2.使用
##2.1 需要实现的底层接口
```C
//顶层接口
    //当需要Ymodem接收协议时
uint8 ymodem_rx_header( char* fil_nm, size_t fil_sz );//接收到包头时被调用
uint8 ymodem_rx_finish( uint8 status );//接收结束或异常时被调用
uint8 ymodem_rx_pac_get( char *buf, size_t seek, size_t size );//正常包时被调用
    //当需要Ymodem发送协议时
uint8 ymodem_tx_header( char** fil_nm, size_t *fil_sz );//发送包头时被调用
uint8 ymodem_tx_finish( uint8 status ); //发送结束或异常时被调用
uint8 ymodem_tx_pac_get( char *buf, size_t offset, size_t size );//正常包时被调用
//底层接口
void __putchar( char ch );              //发送一个字符
void __putbuf( char *buf, size_t len ); //发送一个字符数组
```
##2.2 如何调用那2个应用层函数
```C
void ymodem_rx_put( char *buf, size_t len );
void ymodem_rx_put( char *buf, size_t len );

void main()
{
    if( get_paced( buf, &len ) )
    {
        switch( ymodem_direction_cur )
        {
        case USART_PROTOCOL_YMODEM_DNLOAD:
            ymodem_rx_put( (char*)data, len );
            tm_out = 0x5ffff;
        break;
        case USART_PROTOCOL_YMODEM_UPLOAD:
            ymodem_tx_put( (char*)data, len );
        break;
        default:
        break;
        }
    }
    //在接收模式下，无数据接收时，发送‘C’字符
    if( ymodem_direction_cur == USART_PROTOCOL_YMODEM_DNLOAD )
      if( tm_out )
        tm_out--;
      else
      {ymodem_rx_put( NULL, 0 );tm_out = 0x5ffff;}
  }
}
```

# 3.许可
The MIT License (MIT)

Copyright (c) 2015 chxaitz 345473553@qq.com
