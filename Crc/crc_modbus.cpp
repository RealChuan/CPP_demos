#include <stdio.h>

unsigned int crc_modbus(unsigned int *ptr, unsigned int len)
{
	unsigned char i;
	unsigned int crc = 0xFFFF;
	if (len == 0)
		len = 1;
	while (len--)
	{
		crc ^= *ptr;
		for (i = 0; i < 8; i++)
		{
			if (crc & 1)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
				crc >>= 1;
		}
		ptr++;
	}
	return crc;
}

int main()
{
	unsigned int a[] = { 0x7E,0x10,0x00,0x08,0x00,0x7B };
	unsigned int crc=crc_modbus(a,6);
	printf("CRC=%x\n",crc);
	unsigned int out[2]={0};
	out[1]=crc&0xff;
	out[0]=crc>>8;
	printf("CRC=%02x %02x\n",out[0],out[1]);
}
