#ifndef _I2C_
#define _I2C_


void i2c_init(void);
void i2c_sendData(unsigned char  address_7b, unsigned char regAddress, unsigned char data);
unsigned char i2c_readData(unsigned char address_7b,unsigned char regAddress);
int i2c_sendPacket(unsigned char  address_7b, unsigned char regAddress, int nData, unsigned char  *pData);
int i2c_sendPacket2(unsigned char  address_7b, unsigned char  *pData, int nData);
int i2c_readPacket(unsigned char  address_7b,unsigned char regAddress, int nData, unsigned char  *pData);


#endif /* _I2C_*/
