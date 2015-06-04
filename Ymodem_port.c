#include "Ymodem.h"

//实现的接口函数
  //用户接口
static  FIL    *fdst;
static  size_t fil_rx_sz;
static  char   *fil_tx_nm;      //传输的文件名称
//static  size_t fil_tx_sz;       //传输的文件大小
  //接收
uint8 ymodem_rx_header( char* fil_nm, size_t fil_sz )
{
  uint8 ans = YMODEM_ERR;
  FRESULT res;
  cstr_t *px_cstr;
  if( fdst = (FIL*)pvPortMalloc(sizeof(FIL)) )
  {
    if( px_cstr = cstr_create( "%s\\%s", px_path_cur->string, fil_nm ) )
    {
      res = f_open( fdst, px_cstr->string, FA_WRITE|FA_CREATE_ALWAYS );
      cstr_free( px_cstr );
      if( FR_OK == res )
      {
        fdst->fsize = fil_sz;
        fil_rx_sz = fil_sz;
        ans = YMODEM_OK;
      }
    }
  }
  if( YMODEM_ERR == ans )
  {vPortFree( fdst );fdst = NULL;}
  return ans;
}
uint8 ymodem_rx_finish( uint8 status )
{
  if( fdst )
  {
    f_close( fdst );
    vPortFree( fdst );fdst = NULL;
  }
  return YMODEM_OK;
}
uint8 ymodem_rx_pac_get( char *buf, size_t seek, size_t size )
{
  uint8 ans = YMODEM_ERR;
  UINT wd;
  if( fdst )
  {
    if( FR_OK == f_lseek( fdst, seek ) )
    {
      if( fil_rx_sz < seek + size )
        size = fil_rx_sz - seek;
      if( FR_OK == f_write( fdst, buf, size, &wd ) )
      {
        if( wd == size )
          ans = YMODEM_OK;
      }
    }
  }
  return ans;
}
  //发送
/*********************************************************************
 * @fn      ymodem_tx_set_fil : 自己随便实现的一个函数，用来设置待传送的文件
 * @param   fil_nm : 文件名 
 */ 
uint8 ymodem_tx_set_fil( char* fil_nm )           //此函数由[用户主动调用]，启动文件传输
{
  uint8 ans = YMODEM_ERR;
  int fil_nm_len;
  fil_nm_len = strlen( fil_nm );
  if( fil_tx_nm = pvPortMalloc( fil_nm_len+1 ) )      //文件名空间申请成功
  {
    memcpy( fil_tx_nm, fil_nm, fil_nm_len );
    fil_tx_nm[fil_nm_len]='\0';
    ans = YMODEM_OK;
  }
  return ans;
}
/*********************************************************************
 * @fn      ymodem_tx_header : 系统调用，用来获取文件名和大小
 * @param   fil_nm : 文件名 fil_sz : 文件大小 
 */ 
uint8 ymodem_tx_header( char** fil_nm, size_t *fil_sz )
{
  uint8 ans = YMODEM_ERR;
  FRESULT res;
  cstr_t *px_cstr;
  if( NULL != fil_tx_nm )       //待传输的文件名不为空
  {
    if( fdst = (FIL*)pvPortMalloc(sizeof(FIL)) )//文件指针申请成功
    {
      //打开文件
      if( px_cstr = cstr_create( "%s\\%s", px_path_cur->string, fil_tx_nm ) )
      {
        res = f_open( fdst, px_cstr->string, FA_READ|FA_OPEN_EXISTING );
        cstr_free( px_cstr );
        if( FR_OK == res )
        {
          *fil_nm = fil_tx_nm;        //设置文件名
          *fil_sz = fdst->fsize;      //设置文件大小
          ans = YMODEM_OK;      //文件发送准备已完成
        }
      }
    }
  }
  return ans;
}
/*********************************************************************
 * @fn      ymodem_tx_finish : 当传输结束时，会被调用
 * @param   status : 关闭的原因 YMODEM_OK 或 YMODEM_ERR 
 */ 
uint8 ymodem_tx_finish( uint8 status )                         //返回结束原因，成功还是出错
{
  if( fdst )
  {
    f_close( fdst );
    vPortFree( fdst );fdst = NULL;
    vPortFree( fil_tx_nm ); fil_tx_nm = NULL;
  }
  return YMODEM_OK;
}
/*********************************************************************
 * @fn      ymodem_tx_pac_get : 调用此来读取文件中的相应数据
 * @param   buf : 待写入的缓冲区地址 offset : 数据的偏移 size : 数据的大小 
 */ 
uint8 ymodem_tx_pac_get( char *buf, size_t offset, size_t size ) 
{
  uint8 ans = YMODEM_ERR;
  UINT wd;
  if( fdst )
  {
    if( FR_OK == f_lseek( fdst, seek ) )
    {
      if( fil_rx_sz < seek + size )
        size = fil_rx_sz - seek;
      if( FR_OK == f_read( fdst, buf, size, &wd ) )
      {
        ans = YMODEM_OK;
      }
    }
  }
  return ans;
}
//底层接口
void __putchar( char ch )
{
  usart_write(0,&ch,1);
}
void __putbuf( char *buf, size_t len )
{
  usart_write(0, buf, len );
}
