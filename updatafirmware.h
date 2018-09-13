#ifndef __UPDATA_FIRMWARE_
#define __UPDATA_FIRMWARE_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#include <sys/types.h>


#include <sys/ioctl.h>




#define  uint8_t unsigned char
#define  uint16_t unsigned short
#define  uint32_t  unsigned int
#define  int32_t   int



#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)

#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CA                      (0x18)  /* two of these in succession aborts transfer */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */





#define USER_FLASH_END_ADDRESS        0x0800FFFF /* 64 KBytes */
#define FLASH_PAGE_SIZE               0x400      /* 1 Kbytes */

#define APPLICATION_ADDRESS     (uint32_t)0x08003000


#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)


#define SERIALDEV  "/dev/ttyS1"

#if 0
#define DIGI_TRACE(Level,Msg...) \
    do{\
        {\
            time_t timep;\
            struct tm *p;\
            time(&timep); \
            p = localtime(&timep); \
            qDebug("%d/%02d/%02d %02d:%02d:%02d | %s:%s:%04d | %s:",1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec,filename(__FILE__),__FUNCTION__,__LINE__,Level);\
            qDebug()<<Msg<<endl;\
        }\
    }while(0);

#endif


class UpdataFirmware
{
	public:
		UpdataFirmware();
		~UpdataFirmware();
        int Open_SerialPort(const char *devname);
        void Close_SerialPort();
		uint32_t Send_Byte (uint8_t c);
		void Ymodem_SendPacket(uint8_t *data, uint16_t length);
		uint8_t Ymodem_Transmit (uint8_t *buf, const uint8_t* sendFileName, uint32_t sizeFile);
		void Int2Str(uint8_t* str, int32_t intnum);
		uint8_t USART_ReceiveData();
		uint8_t CalChecksum(const uint8_t* data, uint32_t size);
        int32_t Receive_Byte (uint8_t *c, uint32_t timeout);
		uint16_t Cal_CRC16(const uint8_t* data, uint32_t size);
		uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte);
		void Ymodem_PrepareIntialPacket(uint8_t *data, const uint8_t* fileName, uint32_t *length);
		void Ymodem_PreparePacket(uint8_t *SourceBuf, uint8_t *data, uint8_t pktNo, uint32_t sizeBlk);
		int Read_File(const char *filename,uint8_t *buff,int32_t filelen);
		int Get_File_Size(const char *filename);
		int RecvData();	
		int _RecvData();
		unsigned char* my_itoa(int32_t value,uint8_t *str,int32_t radix); 
	


	private:
		int fd;
		char *file_buff;
};

#endif
