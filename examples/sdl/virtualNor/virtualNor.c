#include "virtualNor.h"

#define NOR_FILE_PATH  "./image.bin"

#define NOR_PAGE_SIZE      4096

char norImgPath[1024]={0};

void norSetBin(char *path)
{
    strcpy(norImgPath,path);
}

void norInit(uint32_t maxSize)
{
    FILE *fp;
    uint8_t buf[1024]={0};
    uint32_t length;
    uint32_t i;
    length=maxSize/sizeof(buf);
    if(maxSize%sizeof(buf))
    {
        length++;
    }
    VIRTUAL_NOR_LOG("virtual nor\nsize:%fMB page:1KB\n",length*sizeof(buf)/1024.0/1024.0);
    memset(buf,0xFF,sizeof(buf));
    if(norImgPath[0]==0)
    {
        fp=fopen(NOR_FILE_PATH,"wb+");
    }
    else
    {
        fp=fopen(norImgPath,"wb+");
    }
    if(fp!=NULL)
    {
        for(i=0;i<length;i++)
        {
            fwrite(buf,sizeof(buf),1,fp);
        }
        fflush(fp);
        fclose(fp);
    }
}

void norRead(uint8_t *buf, uint32_t addr, uint16_t length)
{
    FILE *fp;

    if(norImgPath[0]==0)
    {
        fp=fopen(NOR_FILE_PATH,"rb");
    }
    else
    {
        fp=fopen(norImgPath,"rb");
    }
    if(fp!=NULL)
    {
        fseek(fp,addr,SEEK_SET);
        fread(buf,length,1,fp);
        fclose(fp);
    }
}

void norWrite(uint8_t *buf, uint32_t addr, uint16_t length)
{
    FILE *fp;
    if(norImgPath[0]==0)
    {
        fp=fopen(NOR_FILE_PATH,"rb+");
    }
    else
    {
        fp=fopen(norImgPath,"rb+");
    }
    if(fp!=NULL)
    {
        fseek(fp,addr,SEEK_SET);
        fwrite(buf,length,1,fp);
        fflush(fp);
        fclose(fp);
    }
}

void norWritePage(uint8_t *buf, uint32_t addr, uint16_t length)
{
    uint16_t pageNum;
    uint32_t targetPageLength;
    uint8_t readBuf[NOR_PAGE_SIZE]={0},writeBuf[NOR_PAGE_SIZE]={0};
    uint32_t i;
    //计算本页结束长度
    pageNum=addr/NOR_PAGE_SIZE;
    if(addr%NOR_PAGE_SIZE)
    {
        pageNum++;
    }
    pageNum++;//第0页为1页
    targetPageLength=pageNum*NOR_PAGE_SIZE-addr;
    //计算实际能写入的长度
    if(targetPageLength<length)
    {
        length=targetPageLength;
    }
    norRead(readBuf,addr,length);
    for(i=0;i<length;i++)
    {
        writeBuf[i]=readBuf[i]&buf[i];
    }
    norWrite(writeBuf,addr,length);
}

void norErasePage(uint16_t pageNum)
{
    uint32_t addr;
    uint32_t buf[NOR_PAGE_SIZE];
    addr=pageNum*NOR_PAGE_SIZE;
    memset(buf,0xff,NOR_PAGE_SIZE);
    VIRTUAL_NOR_LOG("erase addr %d\n",addr);
    norWrite(buf,addr,NOR_PAGE_SIZE);
}

void norReadWrite_fast(FILE *fp_fast,uint8_t *path,bool isWrite,uint8_t *buf, uint32_t addr, uint16_t length)
{
    if(fp_fast==NULL)
    {
        fp_fast = fopen((char*)path, "r+");
        if(fp_fast==NULL)
        {
            fp_fast = fopen((char*)path, "w");
        }
        fclose(fp_fast);

        fp_fast=fopen((char*)path,"rb+");
    }

    if(fp_fast!=NULL)
    {
        fseek(fp_fast,addr,SEEK_SET);
        if(isWrite)
        {
            fwrite(buf,length,1,fp_fast);
        }
        else
        {
            fread(buf,length,1,fp_fast);
        }
        fflush(fp_fast);
    }
}

