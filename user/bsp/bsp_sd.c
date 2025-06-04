#include "bsp_sd.h"
#include <stdio.h>


sd_card_info_struct sd_cardinfo;                            /* information of SD card */
uint32_t buf_write[512];                                    /* store the data written to the card */
uint32_t buf_read[512];                                     /* store the data read from the card */



/*!
    \brief      configure the NVIC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void sd_nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    nvic_irq_enable(SDIO_IRQn, 5, 0);
}

/*!
    \brief      initialize the card, get the card information, set the bus mode and transfer mode
    \param[in]  none
    \param[out] none
    \retval     sd_error_enum
*/
sd_error_enum sd_io_init(void)
{
    sd_error_enum status = SD_OK;
    uint32_t cardstate = 0;

    sd_nvic_config();

    status = sd_init();
    if(SD_OK == status) {
        status = sd_card_information_get(&sd_cardinfo);
    }
    if(SD_OK == status) {
        status = sd_card_select_deselect(sd_cardinfo.card_rca);
    }
    status = sd_cardstatus_get(&cardstate);
    if(cardstate & 0x02000000) {
        printf("\r\n the card is locked!");
        while(1) {
        }
    }
    if((SD_OK == status) && (!(cardstate & 0x02000000))) {
        /* set bus mode */
        status = sd_bus_mode_config(SDIO_BUSMODE_4BIT);
//        status = sd_bus_mode_config(SDIO_BUSMODE_1BIT);
    }
    if(SD_OK == status) {
        /* set data transfer mode */
        status = sd_transfer_mode_config(SD_DMA_MODE);
//        status = sd_transfer_mode_config(SD_POLLING_MODE);
    }
    return status;
}

/*!
    \brief      get the card information and print it out by USRAT
    \param[in]  none
    \param[out] none
    \retval     none
*/
void card_info_get(void)
{
    uint8_t sd_spec, sd_spec3, sd_spec4, sd_security;
    uint32_t block_count, block_size;
    uint16_t temp_ccc;

    printf("\r\n Card information:");
    sd_spec = (sd_scr[1] & 0x0F000000) >> 24;
    sd_spec3 = (sd_scr[1] & 0x00008000) >> 15;
    sd_spec4 = (sd_scr[1] & 0x00000400) >> 10;
    if(2 == sd_spec) {
        if(1 == sd_spec3) {
            if(1 == sd_spec4) {
                printf("\r\n## Card version 4.xx ##");
            } else {
                printf("\r\n## Card version 3.0x ##");
            }
        } else {
            printf("\r\n## Card version 2.00 ##");
        }
    } else if(1 == sd_spec) {
        printf("\r\n## Card version 1.10 ##");
    } else if(0 == sd_spec) {
        printf("\r\n## Card version 1.0x ##");
    }

    sd_security = (sd_scr[1] & 0x00700000) >> 20;
    if(2 == sd_security) {
        printf("\r\n## SDSC card ##");
    } else if(3 == sd_security) {
        printf("\r\n## SDHC card ##");
    } else if(4 == sd_security) {
        printf("\r\n## SDXC card ##");
    }

    block_count = (sd_cardinfo.card_csd.c_size + 1) * 1024;
    block_size = 512;
    printf("\r\n## Device size is %dKB ##", sd_card_capacity_get());
    printf("\r\n## Block size is %dB ##", block_size);
    printf("\r\n## Block count is %d ##", block_count);

    if(sd_cardinfo.card_csd.read_bl_partial) {
        printf("\r\n## Partial blocks for read allowed ##");
    }
    if(sd_cardinfo.card_csd.write_bl_partial) {
        printf("\r\n## Partial blocks for write allowed ##");
    }
    temp_ccc = sd_cardinfo.card_csd.ccc;
    printf("\r\n## CardCommandClasses is: %x ##", temp_ccc);
    if((SD_CCC_BLOCK_READ & temp_ccc) && (SD_CCC_BLOCK_WRITE & temp_ccc)) {
        printf("\r\n## Block operation supported ##");
    }
    if(SD_CCC_ERASE & temp_ccc) {
        printf("\r\n## Erase supported ##");
    }
    if(SD_CCC_WRITE_PROTECTION & temp_ccc) {
        printf("\r\n## Write protection supported ##");
    }
    if(SD_CCC_LOCK_CARD & temp_ccc) {
        printf("\r\n## Lock unlock supported ##");
    }
    if(SD_CCC_APPLICATION_SPECIFIC & temp_ccc) {
        printf("\r\n## Application specific supported ##");
    }
    if(SD_CCC_IO_MODE & temp_ccc) {
        printf("\r\n## I/O mode supported ##");
    }
    if(SD_CCC_SWITCH & temp_ccc) {
        printf("\r\n## Switch function supported ##");
    }
}

/*
//	������FatFs_FileTest
//	���ܣ������ļ�д��Ͷ�ȡ����
//
uint8_t  FatFs_FileTest(void)	//�ļ�������д�����
{
    uint8_t i = 0;
    uint16_t BufferSize = 0;
    FIL	MyFile;			// �ļ�����
    UINT 	MyFile_Num;		//	���ݳ���
    BYTE 	MyFile_WriteBuffer[] = "STM32H7B0 SD�� �ļ�ϵͳ����";	//Ҫд�������
    BYTE 	MyFile_ReadBuffer[1024];	//Ҫ����������
    uint8_t MyFile_Res;   // Return value for SD

    printf("-------------FatFs �ļ�������д�����---------------\r\n");

    //�����ļ�ϵͳ
    FATFS fs;
    MyFile_Res = f_mount(&fs, "", 1);
    if (MyFile_Res != FR_OK) {
        return MyFile_Res;
    }

    MyFile_Res = f_open(&MyFile,"0:Test.txt",FA_CREATE_ALWAYS | FA_WRITE);	//���ļ������������򴴽����ļ�
    if(MyFile_Res == FR_OK)
    {
        printf("�ļ���/�����ɹ���׼��д������...\r\n");

        MyFile_Res = f_write(&MyFile,MyFile_WriteBuffer,sizeof(MyFile_WriteBuffer),&MyFile_Num);	//���ļ�д������
        if (MyFile_Res == FR_OK)
        {
            printf("д��ɹ���д������Ϊ��\r\n");
            printf("%s\r\n",MyFile_WriteBuffer);
        }
        else
        {
            printf("�ļ�д��ʧ�ܣ�����SD�������¸�ʽ��!\r\n");
            f_close(&MyFile);	  //�ر��ļ�
            return ERROR;
        }
        f_close(&MyFile);	  //�ر��ļ�
    }
    else
    {
        printf("�޷���/�����ļ�������SD�������¸�ʽ��!\r\n");
        f_close(&MyFile);	  //�ر��ļ�
        return ERROR;
    }

    printf("-------------FatFs �ļ���ȡ����---------------\r\n");

    BufferSize = sizeof(MyFile_WriteBuffer)/sizeof(BYTE);									// ����д������ݳ���
    MyFile_Res = f_open(&MyFile,"0:Test.txt",FA_OPEN_EXISTING | FA_READ);	//���ļ������������򴴽����ļ�
    MyFile_Res = f_read(&MyFile,MyFile_ReadBuffer,BufferSize,&MyFile_Num);			// ��ȡ�ļ�
    if(MyFile_Res == FR_OK)
    {
        printf("�ļ���ȡ�ɹ�������У������...\r\n");

        for(i=0; i<BufferSize; i++)
        {
            if(MyFile_WriteBuffer[i] != MyFile_ReadBuffer[i])		// У������
            {
                printf("У��ʧ�ܣ�����SD�������¸�ʽ��!\r\n");
                f_close(&MyFile);	  //�ر��ļ�
                return ERROR;
            }
        }
        printf("У��ɹ�������������Ϊ��\r\n");
        printf("%s\r\n",MyFile_ReadBuffer);
    }
    else
    {
        printf("�޷���ȡ�ļ�������SD�������¸�ʽ��!\r\n");
        f_close(&MyFile);	  //�ر��ļ�
        return ERROR;
    }

    f_close(&MyFile);	  //�ر��ļ�
    f_mount(NULL, "", 0);
    return SUCCESS;
}
*/

