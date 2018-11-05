#include "serial.h" 


CSerial::CSerial()
{
	m_fd = -1;
    m_sendlen = 0;
    impence = 0;
    m_updateprocess = 0;
	memset(m_senddata,0,128);
	memset(m_recvdata,0,128);
    memset(serialname,0,16);
}
CSerial::~CSerial()
{
	
}

bool CSerial::openSerial(const char *name,int baudrate)
{
	struct termios options;
    if(strlen(name) > 15)
    {
        printf("%s name error!\n",name);
    }
    else
    {
        strcpy(serialname,name);
    }
    if((m_fd = open(name,O_RDWR|O_NOCTTY|O_NDELAY))==-1)
    {
        printf("Open %s device fail\n",name);
        return false;
    }
	tcgetattr(m_fd,&options);
    if(baudrate == 9600)
    {
        cfsetispeed(&options,B9600);
        cfsetospeed(&options,B9600);
    }
    else if(baudrate == 19200)
    {
        cfsetispeed(&options,B19200);
        cfsetospeed(&options,B19200);
    }
    else if(baudrate == 38400)
    {
        cfsetispeed(&options,B38400);
        cfsetospeed(&options,B38400);
    }
    else if(baudrate == 57600)
    {
        cfsetispeed(&options,B57600);
        cfsetospeed(&options,B57600);
    }
    else if(baudrate == 115200)
    {
        cfsetispeed(&options,B115200);
        cfsetospeed(&options,B115200);
    }
    else
    {
        printf("not support baudrate!\n");
    }
	options.c_cflag|=(CLOCAL|CREAD); 
	options.c_cflag &= ~PARENB;        
	options.c_iflag &= ~(IGNCR | INLCR | ICRNL | IXON);
	options.c_cflag &= ~CSTOPB;  
	options.c_cflag&=~CSIZE; 
	options.c_cflag|=CS8;    
	options.c_lflag&=~(ICANON|ECHO|ISIG); 
	options.c_oflag&=~OPOST;  

	tcflush(m_fd,TCIFLUSH);
	tcsetattr(m_fd,TCSANOW,&options);
    return true;
}

/*构造要发送的数据*/
void CSerial::ConstructionSendData(char command,char datalen,char *data)
{
    unsigned char checkcode,i;
	memset(m_senddata,0,sizeof(m_senddata));
	m_senddata[0] = 0xAA;
	m_senddata[1] = 0x0;
	m_senddata[2] = datalen;
	m_senddata[3] = command;

//    char *tmp = data;
//    int ii = 0;
//    for(ii=0;ii<datalen-1;ii++)
//    {
//        printf("  %x  ",*tmp);
//        tmp++;
//    }
//    printf("\n");
    if(datalen != 1)
        memcpy(m_senddata+4,data,datalen-1);

	checkcode = m_senddata[1];
	
	for(i=2;i<4+datalen-1;i++)
	{
		checkcode ^= m_senddata[i];
	}
	m_senddata[4+datalen-1] = checkcode;
	m_senddata[4+datalen] = 0xBB;

    m_sendlen = datalen + 5;

}



void CSerial::SetAddress(char addr,char realDataLen)
{

	char buff[16] = {0};
	buff[0] = addr;
	ConstructionSendData(0x80,realDataLen+1,buff);	
} 

void CSerial::SetBaudrate(char bandrateIndex,char realDataLen)
{
	char buff[16] = {0};
	#if 0
	if(bandrateIndex == 0x00)
	{
		buff[0] = addr;
	}
	else if(bandrateIndex == 0x01)
	{
		buff[0] = addr;
	}
	else if(bandrateIndex == 0x02)
	{
		buff[0] = addr;
	}
	else if(bandrateIndex == 0x03)
	{
		buff[0] = addr;
	}
	else if(bandrateIndex == 0x04)
	{
		buff[0] = addr;
	}
	else
	{
		buff[0] = addr;
	}
	#endif
	buff[0] = bandrateIndex;
	ConstructionSendData(0x81,realDataLen+1,buff);	
}

void CSerial::SetSerNum(char serialNum[8],char realDataLen)
{
	char buff[16] = {0};
    memcpy(buff,serialNum,8);
	ConstructionSendData(0x82,realDataLen+1,buff);	
} 

void CSerial::GetSerNum(char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x83,realDataLen+1,buff);	
}


void CSerial::CureComand(const char *power,char realDataLen)
{
	char buff[16] = {0};
    memcpy(buff,power,strlen(power));
	ConstructionSendData(0x93,realDataLen+1,buff);	
}


void CSerial::WriteUserInfo(char areaNum,char writeDataLen,char realDataLen)
{
	if(writeDataLen > 120)
	{
		printf("the number of reading is request less than 120!\n");
		return ;
	}
	char buff[16] = {0};
	buff[0] = areaNum;
	buff[1] = writeDataLen;
	ConstructionSendData(0x84,realDataLen+1,buff);	
} 

void CSerial::ReadUserInfo(char areaNum,char readDataLen,char realDataLen)
{
	if(readDataLen > 120)
	{
		printf("the number of reading is request less than 120!\n");
		return ;
	}
	char buff[16] = {0};
	buff[0] = areaNum;
	buff[1] = readDataLen;
	ConstructionSendData(0x85,realDataLen+1,buff);	
} 

void CSerial::GetVersionNum(char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x92,realDataLen+1,buff);
}

void CSerial::ControlLed(char period,char cycleNum,char realDataLen)
{
	char buff[16] = {0};
	buff[0] = period;
	buff[1] = cycleNum;
	ConstructionSendData(0x88,realDataLen+1,buff);	
} 

void CSerial::ControlBuzzer(char period,char cycleNum,char realDataLen)
{
	char buff[16] = {0};
	buff[0] = period;
	buff[1] = cycleNum;
	ConstructionSendData(0x89,realDataLen+1,buff);	
}

void CSerial::IdleSend(char realDataLen)
{
	char buff[16] = {0};
    ConstructionSendData(0x91,realDataLen+1,buff);
}


void CSerial::PrepareSend(char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x92,realDataLen+1,buff);
}

void CSerial::PowerSend(char realDataLen)
{
    char buff[16] = {0};
    ConstructionSendData(0x9b,realDataLen+1,buff);
}

void CSerial::PwmdSend(char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x94,realDataLen+1,buff);
}

void CSerial::PwmiSend(char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x96,realDataLen+1,buff);
}
void CSerial::PwmKeepSend(char realDataLen)
{
    char buff[16] = {0};
    ConstructionSendData(0x98,realDataLen+1,buff);
}
void CSerial::CureClosed(char realDataLen)
{
    char buff[16] = {0};
    ConstructionSendData(0x97,realDataLen+1,buff);
}

bool CSerial::UpdateSend(char realDataLen)
{
    char buff[16] = {0};
    ConstructionSendData(0x9c,realDataLen+1,buff);
//    printf(" ********************************************\n ");
//    for(i = 0;i < m_senddata[2]+5;i++)
//        printf("  %x  ",m_senddata[i]);
//    printf(" +++++++++++++++++++++++++++++++++++++++++++++++\n ");
    return true;
}

bool CSerial::SendData()
{
	tcflush(m_fd, TCIOFLUSH);

    //int j;
#if 0
    char k;
    printf("senddata is : ");
    for(j=0;j<m_sendlen;j++)
    {
        k = m_senddata[j];
        printf(" %x  ",k);
    }
    printf("\n");
#endif



    if(write(m_fd,m_senddata,m_sendlen) < 0)
	{
		printf("Write Serial fail\n");
		return false;
	} 
	return true;
}

int CSerial::RecvData()
{
	
	struct timeval tv;	
    int i=0;
	fd_set rdfds; 
	int maxfd = -1;
	int ret;
    char buff[512];
    tv.tv_sec = 1;
	tv.tv_usec = 0; 	
	fflush(stdout);

	while(1)
	{
        tv.tv_sec = 1;
        tv.tv_usec = 0;

		FD_ZERO(&rdfds);   
		FD_SET(0,&rdfds);

        maxfd = m_fd>0?m_fd:0;
        FD_SET(maxfd, &rdfds);


        maxfd = maxfd > 0 ? maxfd:0;
		ret = select(maxfd + 1, &rdfds, NULL, NULL, &tv);
		if(ret < 0)
		{
            return -1;
		}
		else if(ret == 0)
		{
            printf("%s  receve data timeout!\n",serialname);
            return -2;
		}
		else
		{  
			if(FD_ISSET(m_fd, &rdfds)) 
			{		
                memset(buff, 0, 512);
				i = 0;
				while((read(m_fd,&buff[i++],1) > 0)&&(i<511));
				buff[i] = '\0';
#if 0
               //printf("receve data id : ");
               for(j=0;j<i;j++)
                   printf("  %x ",buff[j]);
               printf("\n");
               //printf("%s\n",buff);
#endif
				memcpy(m_recvdata,buff,i);
				fflush(stdout);
                return 0;
			}
		}
	}

}



bool CSerial::ParseRecvData(unsigned char command)
{
	switch(command)
	{
		case 0x80:
			 ParseSetAddressReturnData();	
			 break;
		case 0x81:
			ParseSetBaudrateReturnData();
			 break;
		case 0x82:
			ParseSetSerNumReturnData();
			 break;
		case 0x83:
			ParseGetSerNumReturnData();
			 break;
		case 0x93:;break;
		case 0x84:
			ParseWriteUserInfoReturnData();
			 break;
		case 0x85:
			ParseReadUserInfoReturnData();
			 break;
		case 0x86:
			ParseGetVersionNumReturnData();
			 break;
		case 0x88:
			ParseControlLedReturnData();
			 break;
		case 0x89:
			ParseControlBuzzerReturnData();
			 break;
		case 0x91:;break;
		case 0x92:
			ParsePrepareSendReturnData();
			 break;
		case 0x94:
			ParsePwmdSendReturnData();
			 break;
		default :;break;			

	}
	return true;
}

//??????????????????
bool CSerial::GetRealDataField()
{
	realDataFiled.p_datafiled = NULL;
    realDataFiled.datalen = m_recvdata[2] - 1;
    if(realDataFiled.datalen != 0)
    {
        realDataFiled.p_datafiled = (char *)malloc(sizeof(char)*realDataFiled.datalen);
        if(realDataFiled.p_datafiled)
            memcpy(realDataFiled.p_datafiled,&m_recvdata[4],realDataFiled.datalen);
        else
            return false;
    }
    realDataFiled.returnstatus = m_recvdata[3];
    return true;
}


bool CSerial::ParseSetAddressReturnData()
{
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseSetBaudrateReturnData()
{
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseSetSerNumReturnData()
{
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		if(*p_tmp == 0x80)
		{
			printf("operation is succesful!\n");	
		}
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		else if(*p_tmp == 0x81)
		{
			printf("operation is failed!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseGetSerNumReturnData()
{
	char *p_tmp;
	char serialNum[8]={0};
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		memcpy(serialNum,p_tmp+1,8);
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

//bool CSerial::ParseCureComandReturnData()
//{
	
//}

bool CSerial::ParseWriteUserInfoReturnData()
{	
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		if(*p_tmp == 0x80)
		{
			printf("operation is succesful!\n");	
		}
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		else if(*p_tmp == 0x81)
		{
			printf("operation is failed!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
	
}

bool CSerial::ParseReadUserInfoReturnData()
{
	char *p_tmp;
	bool status;
	char userinfo[120];
	memset(userinfo,0,120);
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		memcpy(userinfo,p_tmp,realDataFiled.datalen-1);
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseGetVersionNumReturnData()
{
	char *p_tmp;
	bool status;
	char versionNum[64];
	memset(versionNum,0,64);
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		memcpy(versionNum,p_tmp,realDataFiled.datalen-1);
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
	free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseControlLedReturnData()
{
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		if(*p_tmp == 0x80)
		{
			printf("operation is succesful!\n");	
		}
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		else if(*p_tmp == 0x81)
		{
			printf("operation is failed!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseControlBuzzerReturnData()
{
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		if(*p_tmp == 0x80)
		{
			printf("operation is succesful!\n");	
		}
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		else if(*p_tmp == 0x81)
		{
			printf("operation is failed!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}



bool CSerial::ParsePrepareSendReturnData()
{
	char *p_tmp;
	bool status;
    //char impedance[2] = {0};
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;

	if(realDataFiled.returnstatus == 0x00)
	{
        if(realDataFiled.datalen == 2)
        {
            impence =(*p_tmp);
            p_tmp++;
            impence = (impence<<8)|(*p_tmp);
        }
        /*
        else if(realDataFiled.datalen == 3)
        {
            impence =(*p_tmp);
            p_tmp++;
            impence = (impence<<8)|(*p_tmp);
        }*/
        status = true;
	}

    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
    realDataFiled.p_datafiled = NULL;
    p_tmp = NULL;
	return status;
}

bool CSerial::ParsePowerSendReturnData()
{
    char *p_tmp;
    bool status;
    //char impedance[2] = {0};
    GetRealDataField();
    p_tmp = realDataFiled.p_datafiled;

    if(realDataFiled.returnstatus == 0x00)
    {
        powerrate[0] = *p_tmp;
        p_tmp++;
        powerrate[1] = *p_tmp;
        p_tmp++;
        powerrate[2] = *p_tmp;
        //printf("vvv %d %d %d\n",powerrate[0],powerrate[1],powerrate[2]);
        status = true;
    }
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
    realDataFiled.p_datafiled = NULL;
    p_tmp = NULL;
    return status;
}


bool CSerial::ParsePwmdSendReturnData()
{
	char *p_tmp;
    bool status = true;
	char power[16] = {0};
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;

	memcpy(power,p_tmp,realDataFiled.datalen-1);	
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseIdleSendReturnData()
{

    if(!GetRealDataField())
        return false;
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
    if(realDataFiled.returnstatus == 0x00)
        return true;
    else
        return false;
}
bool CSerial::ParseCureSendReturnData()
{

    GetRealDataField();
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
    if(realDataFiled.returnstatus == 0x00)
        return true;
    else
        return false;
}

bool CSerial::ParseUpdateSendReturnData()
{
    GetRealDataField();
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
    if(realDataFiled.returnstatus == 0x00)
        return true;
    else
        return false;
}

/**/

int CSerial::Get_File_Size(const char *filename)
{
   struct stat buf;
   int fd;
   fd = open(filename, O_RDONLY);
   if(fd == -1)
   {
       printf("open fail error!\n");
       return -1;
   }
   fstat(fd, &buf);
   close(fd);
   return buf.st_size;
}


int CSerial::Read_File(const char *filename,uint8_t *buff,int32_t filelen)
{

   int fd;
   fd = open(filename, O_RDONLY);
   if(fd == -1)
   {
       printf("open fail error!\n");
       return -1;
   }
   if(read(fd,buff,filelen) < 0)
   {
      close(fd);
      return -2;
   }
   close(fd);
   return 0;
}

/**
  * @brief  Send a byte
  * @param  c: Character
  * @retval 0: Byte sent
  */
uint32_t CSerial::Send_Byte (uint8_t c)
{
    if(write(m_fd,&c,1) < 0)
    {
        printf("Send_Byte failed!\n");
    }

    usleep(100);
    return 0;
}


/**
  * @brief  Transmit a data packet using the ymodem protocol
  * @param  data
  * @param  length
  * @retval None
  */
void CSerial::Ymodem_SendPacket(uint8_t *data, uint16_t length)
{
#if 1
  uint16_t i;
  i = 0;
  while (i < length)
  {
    Send_Byte(data[i]);
    i++;
  }
#endif
//	if(write(fd,data,length)<0)
//	{
//		printf("Ymodem_SendPacket failed!\n");
//	}
}


/**
  * @brief  Transmit a file using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
uint8_t CSerial::Ymodem_Transmit (uint8_t *buf, const uint8_t* sendFileName, uint32_t sizeFile)

{
  uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];
  uint8_t FileName[FILE_NAME_LENGTH];
  uint8_t *buf_ptr, tempCheckSum ;
  uint16_t tempCRC, blkNumber;
  uint8_t receivedC[2], CRC16_F = 0, i;
  uint32_t errors = 0, ackReceived = 0, size = 0, pktSize;

  for (i = 0; i < (FILE_NAME_LENGTH - 1); i++)
  {
    FileName[i] = sendFileName[i];
  }
  CRC16_F = 1;

  /* Prepare first block */
  Ymodem_PrepareIntialPacket(&packet_data[0], FileName, &sizeFile);
  do
  {
    /* Send Packet */
    Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);

    /* Send CRC or Check Sum based on CRC16_F */
    if (CRC16_F)
    {
       tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
       Send_Byte(tempCRC >> 8);
       Send_Byte(tempCRC & 0xFF);
    }
    else
    {
       tempCheckSum = CalChecksum (&packet_data[3], PACKET_SIZE);
       Send_Byte(tempCheckSum);
    }

    /* Wait for Ack and 'C' */
    if (Receive_Byte(&receivedC[0], 1000000) == 0)
    {
        //printf("receve data %d!\n",receivedC[0]);
      if (receivedC[0] == ACK)
      {
        /* Packet transfered correctly */
        ackReceived = 1;
      }
    }
    else
    {
        printf("not receve data!\n");
        errors++;
    }
  }while (!ackReceived && (errors < 0x0A));

  if (errors >=  0x0A)
  {
    return errors;
  }
  buf_ptr = buf;
  size = sizeFile;
  blkNumber = 0x01;

  //printf("********************************first package finished*****************************************!\n");

  /* Here 1024 bytes package is used to send the packets */
  while (size)
  {
    /* Prepare next packet */
    Ymodem_PreparePacket(buf_ptr, &packet_data[0], blkNumber, size);
    ackReceived = 0;
    receivedC[0]= 0;
    errors = 0;
    do
    {
      /* Send next packet */
      if (size >= PACKET_1K_SIZE)
      {
        pktSize = PACKET_1K_SIZE;

      }
      else
      {
        pktSize = PACKET_SIZE;
      }
      usleep(50*1000);
      Ymodem_SendPacket(packet_data, pktSize + PACKET_HEADER);
      /* Send CRC or Check Sum based on CRC16_F */
      if (CRC16_F)
      {
         tempCRC = Cal_CRC16(&packet_data[3], pktSize);
         Send_Byte(tempCRC >> 8);
         Send_Byte(tempCRC & 0xFF);
      }
      else
      {
        tempCheckSum = CalChecksum (&packet_data[3], pktSize);
        Send_Byte(tempCheckSum);
      }

      //printf("22222222222222222222222222222222222222222222222222222222!\n");
      /* Wait for Ack */
      if (Receive_Byte(&receivedC[0], 1000000) == 0)
      {
      //printf("1111 receve data! %d\n",receivedC[0]);
      if (receivedC[0] == ACK)
      {
        ackReceived = 1;
        if (size > pktSize)
        {
           buf_ptr += pktSize;
           size -= pktSize;
           if (blkNumber == (USER_FLASH_SIZE/1024))
           {
             return 0xFF; /*  error */
           }
           else
           {
              blkNumber++;
              m_updateprocess++;
              printf("cccc %d\n",blkNumber);
           }
        }
        else
        {
           buf_ptr += pktSize;
           size = 0;
        }
      }
      }
      else
      {
        printf("1111 not receve data!\n");
        errors++;
      }
    }while(!ackReceived && (errors < 0x0A));

    /* Resend packet if NAK  for a count of 10 else end of commuincation */
    if (errors >=  0x0A)
    {
      return errors;
    }

  }

  ackReceived = 0;
  receivedC[0] = 0x00;
  receivedC[1] = 0x00;
  errors = 0;
  do
  {
    Send_Byte(EOT);
    /* Send (EOT); */
    /* Wait for Ack */
    receivedC[0] = USART_ReceiveData();
    if (receivedC[0] == ACK)
    {
      ackReceived = 1;
    }

    else
    {
      errors++;
    }
    /* Clear Overrun flag of the USART2 */
    //USART_ClearFlag(EVAL_COM1, USART_FLAG_ORE);
  }while (!ackReceived && (errors < 0x0A));

  if (errors >=  0x0A)
  {
    return errors;
  }

  /* Last packet preparation */
  ackReceived = 0;
  receivedC[0] = 0x00;
  receivedC[1] = 0x00;
  errors = 0;

  packet_data[0] = SOH;
  packet_data[1] = 0;
  packet_data [2] = 0xFF;

  for (i = PACKET_HEADER; i < (PACKET_SIZE + PACKET_HEADER); i++)
  {
     packet_data [i] = 0x00;
  }

  do
  {
    /* Send Packet */
    Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);

    /* Send CRC or Check Sum based on CRC16_F */
    tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
    Send_Byte(tempCRC >> 8);
    Send_Byte(tempCRC & 0xFF);

    /* Wait for Ack and 'C' */
    if (Receive_Byte(&receivedC[1], 1000000) == 0)
    {
      if (receivedC[1] == ACK)
      {
        /* Packet transfered correctly */
        ackReceived = 1;
      }
    }
    else
    {
      errors++;
    }
  }while (!ackReceived && (errors < 0x0A));

  /* Resend packet if NAK  for a count of 10  else end of commuincation */
  if (errors >=  0x0A)
  {
    return errors;
  }
  receivedC[0] = 0x00;
  do
  {
    usleep(10*1000);
    Send_Byte(EOT);
    /* Send (EOT); */
    /* Wait for Ack */
    if ((Receive_Byte(&receivedC[0], 1000000) == 0)  && receivedC[0] == ACK)
    {
      ackReceived = 1;
    }

    else
    {
      errors++;
    }
    /* Clear Overrun flag of the USART2 */
    //USART_ClearFlag(EVAL_COM1, USART_FLAG_ORE);
  }while (!ackReceived && (errors < 0x0A));

  if (errors >=  0x0A)
  {
    return errors;
  }
  return 0; /* file trasmitted successfully */
}

/**
  * @brief  Convert an Integer to a string
  * @param  str: The string
  * @param  intnum: The intger to be converted
  * @retval None
  */
#if 0
void Int2Str(uint8_t* str, int32_t intnum)
{
  uint32_t i, Div = 1000000000, j = 0, Status = 0;

  for (i = 0; i < 10; i++)
  {
    str[j++] = (intnum / Div) + 48;

    intnum = intnum % Div;
    Div /= 10;
    if ((str[j-1] == '0') & (Status == 0))
    {
      j = 0;
    }
    else
    {
      Status++;
    }
  }
}
#endif
unsigned char* CSerial::my_itoa(int32_t value,uint8_t *str,int32_t radix)
{
    int sign = 0;
    //char *s = str;
    char ps[32]={0};
    int i=0,j;
    if(value < 0)
    {
        sign = -1;
        value = -value;
    }
    do
    {
        if(value%radix>9)
            ps[i] = value%radix +'0'+7;    //A-F
        else
            ps[i] = value%radix +'0';
        i++;
    }while((value/=radix)>0);
    if(sign<0)
        ps[i] = '-';
    else
        i--;
    for( j=i;j>=0;j--)
    {
        str[i-j] = ps[j];
    }
    return str;
}

/**
  * @brief  Cal CRC16 for YModem Packet
  * @param  data
  * @param  length
  * @retval CRC value
  */
uint16_t CSerial::Cal_CRC16(const uint8_t* data, uint32_t size)
{
  uint32_t crc = 0;
  const uint8_t* dataEnd = data+size;

  while(data<dataEnd)
  {
    crc = UpdateCRC16(crc,*data++);
  }
  crc = UpdateCRC16(crc,0);
  crc = UpdateCRC16(crc,0);

  return (crc&0xffffu);
}

/**
  * @brief  Update CRC16 for input byte
  * @param  CRC input value
  * @param  input byte
  * @retval Updated CRC value
  */
uint16_t CSerial::UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
  uint32_t crc = crcIn;
  uint32_t in = byte|0x100;

  do
  {
    crc <<= 1;
    in <<= 1;

    if(in&0x100)
    {
      ++crc;
    }

    if(crc&0x10000)
    {
      crc ^= 0x1021;
    }
 } while(!(in&0x10000));

 return (crc&0xffffu);
}


/**
  * @brief  Prepare the first block
  * @param  timeout
  * @retval None
  */
void CSerial::Ymodem_PrepareIntialPacket(uint8_t *data, const uint8_t* fileName, uint32_t *length)
{
  uint16_t i, j;
  uint8_t file_ptr[10]={0};

  /* Make first three packet */
  data[0] = SOH;
  data[1] = 0x00;
  data[2] = 0xff;

  /* Filename packet has valid data */
  for (i = 0; (fileName[i] != '\0') && (i < FILE_NAME_LENGTH);i++)
  {
     data[i + PACKET_HEADER] = fileName[i];
  }

  data[i + PACKET_HEADER] = 0x00;

  my_itoa (*length, file_ptr,10);
  for (j =0, i = i + PACKET_HEADER + 1; file_ptr[j] != '\0' ; )
  {
     data[i++] = file_ptr[j++];
  }

  for (j = i; j < PACKET_SIZE + PACKET_HEADER; j++)
  {
    data[j] = 0;
  }

}





/**
  * @brief  Cal Check sum for YModem Packet
  * @param  data
  * @param  length
  * @retval None
  */
uint8_t CSerial::CalChecksum(const uint8_t* data, uint32_t size)
{
  uint32_t sum = 0;
  const uint8_t* dataEnd = data+size;

  while(data < dataEnd)
  {
    sum += *data++;
  }

 return (sum&0xffu);
}


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Receive byte from sender
  * @param  c: Character
  * @param  timeout: Timeout
  * @retval 0: Byte received
  *         -1: Timeout
  */
int32_t CSerial::Receive_Byte (uint8_t *c, uint32_t timeout)
{
    struct timeval tv;

    fd_set rdfds;
    int maxfd = -1;
    int ret;
    char buff;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    fflush(stdout);

    while(1)
    {
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        FD_ZERO(&rdfds);
        FD_SET(0,&rdfds);

        maxfd = m_fd>0?m_fd:0;
        FD_SET(maxfd, &rdfds);


        maxfd = maxfd > 0 ? maxfd:0;
        ret = select(maxfd + 1, &rdfds, NULL, NULL, &tv);
        if(ret < 0)
        {
            perror("Select fail\n");
            return -1;
        }
        else if(ret == 0)
        {
            printf("%s Receive_Byte receve data timeout!\n",serialname);
            return -2;
        }
        else
        {
            if(FD_ISSET(m_fd, &rdfds))
            {
                read(m_fd,&buff,1);
                *c = buff;
                break;
            }
        }
    }
    return 0;
}

uint8_t CSerial::USART_ReceiveData()
{
    struct timeval tv;
    fd_set rdfds;
    int maxfd = -1;
    int ret;
    char buff = -1;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    fflush(stdout);

    while(1)
    {
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        FD_ZERO(&rdfds);
        FD_SET(0,&rdfds);

        maxfd = m_fd>0?m_fd:0;
        FD_SET(maxfd, &rdfds);


        maxfd = maxfd > 0 ? maxfd:0;
        ret = select(maxfd + 1, &rdfds, NULL, NULL, &tv);
        if(ret < 0)
        {
            perror("Select fail\n");
            break;
        }
        else if(ret == 0)
        {
            printf("%s USART_ReceiveData	receve data timeout!\n",serialname);
            break;
        }
        else
        {
            if(FD_ISSET(m_fd, &rdfds))
            {
                read(m_fd,&buff,1);
                break;
            }
        }
    }
    return buff;
}

int CSerial::BreakForTransFile()
{

    struct timeval tv;
    int i=0,j=0;
    fd_set rdfds;
    int maxfd = -1;
    int ret;
    char buff[512];
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    fflush(stdout);

    while(1)
    {
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        FD_ZERO(&rdfds);
        FD_SET(0,&rdfds);

        maxfd = m_fd>0?m_fd:0;
        FD_SET(maxfd, &rdfds);


        maxfd = maxfd > 0 ? maxfd:0;
        ret = select(maxfd + 1, &rdfds, NULL, NULL, &tv);
        if(ret < 0)
        {
            perror("Select fail\n");
            return -1;
        }
        else if(ret == 0)
        {
            printf("%s RecvData receve data timeout!\n",serialname);
            break;
        }
        else
        {
            if(FD_ISSET(m_fd, &rdfds))
            {
                memset(buff, 0, 512);
                i = 0;
                while((read(m_fd,&buff[i++],1) > 0)&&(i<511));
                buff[i] = '\0';
#if 1
               for(j=0;j<i;j++)
                {
                    if(buff[j] == 0x43)
                    {
                        return 0;
                    }
                }
               printf("\n");
#endif
            }
        }
    }
    return 0;
}

int CSerial::GetVersionNumber(char *version)
{
    struct timeval tv;
    int i=0;
    fd_set rdfds;
    int maxfd = -1;
    int ret;
    char buff[512];
    char *tmo_pos = NULL;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    fflush(stdout);

    while(1)
    {
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        FD_ZERO(&rdfds);
        FD_SET(0,&rdfds);

        maxfd = m_fd>0?m_fd:0;
        FD_SET(maxfd, &rdfds);


        maxfd = maxfd > 0 ? maxfd:0;
        ret = select(maxfd + 1, &rdfds, NULL, NULL, &tv);
        if(ret < 0)
        {
            perror("Select fail\n");
            return -1;
        }
        else if(ret == 0)
        {
            printf("%s _RecvData receve data timeout!\n",serialname);
            break;
        }
        else
        {
            if(FD_ISSET(m_fd, &rdfds))
            {
                memset(buff, 0, 512);
                i = 0;

                while((read(m_fd,&buff[i++],1) > 0)&&(i<511));
                buff[i] = '\0';
                //if(NULL != strstr(buff,"version"))
                tmo_pos = strstr(buff,"version");
                if(tmo_pos != NULL)
                {
                    memcpy(version,tmo_pos,13);

                    return 0;
                }
            }
        }
    }
    return 0;
}


/**
  * @brief  Prepare the data packet
  * @param  timeout
  * @retval None
  */
void CSerial::Ymodem_PreparePacket(uint8_t *SourceBuf, uint8_t *data, uint8_t pktNo, uint32_t sizeBlk)
{
  uint16_t i, size, packetSize;
  uint8_t* file_ptr;

  /* Make first three packet */
  packetSize = sizeBlk >= PACKET_1K_SIZE ? PACKET_1K_SIZE : PACKET_SIZE;
  size = sizeBlk < packetSize ? sizeBlk :packetSize;
  if (packetSize == PACKET_1K_SIZE)
  {
     data[0] = STX;
  }
  else
  {
     data[0] = SOH;
  }
  data[1] = pktNo;
  data[2] = (~pktNo);
  file_ptr = SourceBuf;

  /* Filename packet has valid data */
  for (i = PACKET_HEADER; i < size + PACKET_HEADER;i++)
  {
     data[i] = *file_ptr++;
  }
  if ( size  <= packetSize)
  {
    for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
    {
      data[i] = 0x1A; /* EOF (0x1A) or 0x00 */
    }
  }


}




