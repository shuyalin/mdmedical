#ifndef __SERIAL_H_
#define __SERIAL_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <termios.h>
	#include <unistd.h>

	#include <sys/select.h>

	#include <sys/time.h>
	#include <sys/types.h>



	#include <sys/stat.h>
	#include <fcntl.h>
	#include <sys/ioctl.h>
//}


//using namespace std;


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


#define DEV "/dev/ttyS5"



typedef struct CReturnDataArch
{
	char datalen;
	char *p_datafiled;
	char returnstatus;

}CRETURNDATAARCH;

class CSerial
{
	public:
		CSerial();
		~CSerial();
        bool openSerial(const char *name,int bandrate);
		
		void ConstructionSendData(char command,char datalen,char *data);
		
		void SetAddress(char addr,char realDataLen);
		
		void SetBaudrate(char bandrateIndex,char realDataLen);
		
		void SetSerNum(char serialNum[8],char realDataLen);
		
        void GetSerNum(char realDataLen);
		
        void CureComand(const char *power,char realDataLen);
		
		void WriteUserInfo(char areaNum,char writeDataLen,char realDataLen);
		
		void ReadUserInfo(char areaNum,char readDataLen,char realDataLen);
		
		void GetVersionNum(char realDataLen);
		
		void ControlLed(char period,char cycleNum,char realDataLen);
		
		void ControlBuzzer(char period,char cycleNum,char realDataLen);
		
		void IdleSend(char realDataLen);
		
		void PrepareSend(char realDataLen);

        void PowerSend(char realDataLen);
		
		void PwmdSend(char realDataLen);
		
		void PwmiSend(char realDataLen);

        void PwmKeepSend(char realDataLen);

        void CureClosed(char realDataLen);

        bool UpdateSend(char realDataLen);
		
		bool SendData();
		
		bool RecvData();
		
		bool ParseRecvData(unsigned char command);
		
		void GetRealDataField();
		
		bool ParseSetAddressReturnData();
		
		bool ParseSetBaudrateReturnData();
		
		bool ParseSetSerNumReturnData();
		
		bool ParseGetSerNumReturnData();
		
		bool ParseCureComandReturnData();	
		
		bool ParseWriteUserInfoReturnData();
		
		bool ParseReadUserInfoReturnData();
		
		bool ParseGetVersionNumReturnData();
		
		bool ParseControlLedReturnData();
		
		bool ParseControlBuzzerReturnData();
		
		bool ParsePrepareSendReturnData();

        bool ParsePowerSendReturnData();
		
		bool ParsePwmdSendReturnData();

        bool ParseIdleSendReturnData();

        bool ParseCureSendReturnData();

        bool ParseUpdateSendReturnData();


        /*以下是使用Ymode模式更新单片机固件的函数*/
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

        int BreakForTransFile();

        int GetVersionNumber(char *version);

        unsigned char* my_itoa(int32_t value,uint8_t *str,int32_t radix);

        int m_fd;
        int m_updateprocess;
        unsigned short impence;
        char m_recvdata[128];
        char powerrate[3];




	private:

		char m_senddata[128];
        char m_sendlen;
        char serialname[16];
		CRETURNDATAARCH realDataFiled;
};



#endif
