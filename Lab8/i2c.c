#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"                   
#include "i2c.h"

int i2c_isBusy(void);

void i2c_init(void){
    SYSCTL_RCGCGPIO_R |= 0x10;//Turn on GPIOE (for I2C2)
    SYSCTL_RCGCI2C_R |=0x4;//Turn on I2C2

    //configure E4(SCL) and E5(SDA) for I2C signals
    GPIO_PORTE_DEN_R |=(0x1<<4)|(0x1<<5); //digital type
    GPIO_PORTE_PUR_R |=(0x1<<4)|(0x1<<5); //pull ups
    GPIO_PORTE_ODR_R |=(0x1<<5); //open-drain output (only on SDA pin)
    GPIO_PORTE_AFSEL_R |=(0x1<<4)|(0x1<<5);//alternate functions
		GPIO_PORTE_PCTL_R |= (0x3<<16) | (0x3<<20);//pins controlled by I2C2

    I2C2_MCR_R =0x10;//I2C module in master mode
    I2C2_MTPR_R =0x18;//clock configuration 50MHz to 100KHz(standard speed)
}

void i2c_sendData(unsigned char address_7b, unsigned char regAddress, unsigned char data){
    I2C2_MSA_R=(address_7b<<1)|(0x0);//write to address uC->slave
    I2C2_MDR_R=regAddress;
    I2C2_MCS_R=0x7;//single master transmit (STOP START RUN)
		I2C2_MDR_R = data;
    while(i2c_isBusy());
}

unsigned char i2c_readData(unsigned char address_7b,unsigned char regAddress){
    I2C2_MSA_R=(address_7b<<1)|(0x0);//write uC->slave
    I2C2_MDR_R=regAddress;
    I2C2_MCS_R=0x3;//single master transmit (START RUN)
    while(i2c_isBusy());
    I2C2_MSA_R=(address_7b<<1)|(0x1);//read uC<-slave
    I2C2_MCS_R=0x7;//single master receive (STOP START RUN)
    while(i2c_isBusy());
    return I2C2_MDR_R;
}

int i2c_sendPacket(unsigned char  address_7b, unsigned char regAddress, int nData, unsigned char  *pData){
    int i;
    if(nData>0){
        I2C2_MSA_R=(address_7b<<1)|(0x0);//write to address uC_slave
        I2C2_MDR_R=regAddress;
        I2C2_MCS_R=(nData>0)?0x3:0x7;//single master transmit (START RUN)/(STOP START RUN)
        while(i2c_isBusy());
        for(i=0; i<(nData-1); i++){
            I2C2_MDR_R=pData[i];
            I2C2_MCS_R=0x1;//single master transmit (RUN)
            while(i2c_isBusy());
        }
        if(nData>0){
            I2C2_MDR_R=pData[nData-1];
            I2C2_MCS_R=0x5;//single master transmit (STOP RUN)
            while(i2c_isBusy());
        }
    }
		return 1;
}


int i2c_readPacket(unsigned char  address_7b,unsigned char regAddress,
        int nData, unsigned char *pData){
    if(nData>0){
        //Write register Address
        I2C2_MSA_R=(address_7b<<1)|(0x0);//write uC_slave
        I2C2_MDR_R=regAddress;
        I2C2_MCS_R=0x3;//single master transmit (START RUN)
        while(i2c_isBusy());
        //read first byte
        I2C2_MSA_R=(address_7b<<1)|(0x1);//read uC<-slave
        I2C2_MCS_R=(nData>1)?0xB:0x7;//single master transmit (ACK START RUN)/(STOP START RUN)
        while(i2c_isBusy());
        pData[0]= I2C2_MDR_R;
        //This only executes if nData >_R= 3
        for(int i=1; i<(nData-1); i++){
                I2C2_MCS_R=0x9;//single master transmit (RUN)
                while(i2c_isBusy());
                pData[i] = I2C2_MDR_R;
        }
        // only  executes if nData >_R= 2
        if(nData>1){
            I2C2_MCS_R=0x5;//single master transmit (STOP RUN)
            while(i2c_isBusy());
            pData[nData-1] = I2C2_MDR_R;
        }
    }
		return 1;
}

int i2c_isBusy(void){
    return (((I2C2_MCS_R)&(0x1<<0))>>0);//return busy bit
}
