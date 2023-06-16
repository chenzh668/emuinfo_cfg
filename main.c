
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
// #include <stdlib.h>
#include <stdlib.h>
#include "share_type.h"
// #include <Linux/types.h>
// #include "cjson.h"

#include "cJSON.h"
#define FLIEPATH "/usr/local/sanway/emu_doc/emu.conf"

typedef struct {
	u8 Month;
	u8 Day;
	u16 Year;//外部全部统一用4位年份
	u8 Week;
	u8 Sec;
	u8 Min;
	u8 Hour;
	u16 Msec;//毫秒，0.001
} TDateTime; //10个字节

typedef struct {
	int firm_id;//公司编号
//LCD参数
	char  lcd_server_ip[6][64];
	u32 lcd_server_port[6];
    int lcd_num;
    int pcs_num;
    char plc_server_ip[64];
    u32 plc_server_port;
    int balance_rate;
    int sys_max_pw;
    int flag_init_lcd;

//BMS参数 用于lcd模块做判断对pcs停机
	float Maximum_individual_voltage; //电池分系统 n 单体最高电压  
	float Minimum_individual_voltage;//电池分系统 n 单体最低电压  
////
//    char bams1_pcsid[8];
//    char bams2_pcsid[8];

//系统参数
	char iflog;//是否将协议日志记录到flash中
	char hardware_ver;//硬件版本
	TDateTime StartLogDay;
	int logdays;
	char main_ver[6];


}pconf;

pconf emu_conf;



int read_config(int *fd, char *readBuf)
{
    int read_fd,readlen=1024;
    read_fd = open(FLIEPATH, O_RDWR | O_CREAT, 0600); // 从新打开文件，将光标位置置于最前面。
    // readBuf = (char *)malloc(sizeof(char) * readlen);
    int n_read = read(read_fd, readBuf, readlen); // 读取file1中的内容。
    // printf("read:%s \n",readBuf);
    *fd = read_fd;
    return n_read;
}

int write_config( char* conf_str){
    FILE *fp;
 
	fp = fopen(FLIEPATH,"w+");
	if(fp == NULL){
		fprintf(stderr,"open file failed\n");
		return -1;
	}
     size_t count = fwrite(conf_str, 1, strlen(conf_str), fp);
    //  int count1=count;
    //  printf("count:%d",count);

    fclose(fp);
    return count;
    
}

int get_rep(){
    int fd=0,ret;
    char buf[1024]={0};
    ret = read_config(&fd, buf);
    printf("%s", buf); // 打印读取的内容。
    close(fd);
}


int post_read_config( char* buf){
    int fd=0,len;
    int *pfd = &fd;
    len =read_config(pfd,buf); // 打印读取的内容。
    close(fd);
    return len;
}

int Analysis_data(char post_data[],char (*data)[256],int *len){
    char delim[] = "&";
    char *token;
    int count = 0, i = 1;
    
    char *pKey=NULL,*pValue=NULL;

    token = strtok(post_data,"&");
    strncpy(data[0], token, strlen(token) + 1); // 将token指针指向的字符串复制到data数组中
    while (token != NULL) {
        token = strtok(NULL,"&");
        if (token != NULL) {
            strncpy(data[i], token, strlen(token) + 1); // 将token指针指向的字符串复制到data数组中
            i++;
        }
        count++;
    }
    *len = count;
    return 0;
}

int config_assignment(char (*res_data)[256],int len){
    int i;
    char *pKey;
    char *pValue;

    for(i=0;i<len;i++){
        pKey= strtok(res_data[i], "=");
        pValue = strtok(NULL, "=");
        if(strcmp(pKey,"ip1")==0)
            memcpy(emu_conf.lcd_server_ip[0], pValue, sizeof(emu_conf.lcd_server_ip[0])); 
        else if(strcmp(pKey,"ip2")==0)
            memcpy(emu_conf.lcd_server_ip[1], pValue, sizeof(emu_conf.lcd_server_ip[1])); 
        else if(strcmp(pKey,"ip3")==0)
            memcpy(emu_conf.lcd_server_ip[2], pValue, sizeof(emu_conf.lcd_server_ip[2])); 
        else if(strcmp(pKey,"ip4")==0)
            memcpy(emu_conf.lcd_server_ip[3], pValue, sizeof(emu_conf.lcd_server_ip[3])); 
        else if(strcmp(pKey,"ip5")==0)
            memcpy(emu_conf.lcd_server_ip[4], pValue, sizeof(emu_conf.lcd_server_ip[4])); 
        else if(strcmp(pKey,"ip6")==0)
            memcpy(emu_conf.lcd_server_ip[5], pValue, sizeof(emu_conf.lcd_server_ip[5])); 
        else if(strcmp(pKey,"plcIP")==0)
            memcpy(emu_conf.plc_server_ip, pValue, sizeof(emu_conf.plc_server_ip)); 


        // printf("<script>");
        //         printf("console.log(\"%s:%s  \");",pKey,pValue);
        // printf("</script>");
     }

}


int Json_assignment(cJSON *pConfigJson){
      if(strcmp(cJSON_GetObjectItem(pConfigJson, "lcd_server_ip1")->valuestring,emu_conf.lcd_server_ip[0])!=0){
            cJSON_ReplaceItemInObject(pConfigJson, "lcd_server_ip1", cJSON_CreateString(emu_conf.lcd_server_ip[0]));
        }

       if(strcmp(cJSON_GetObjectItem(pConfigJson, "lcd_server_ip2")->valuestring,emu_conf.lcd_server_ip[1])!=0){
            cJSON_ReplaceItemInObject(pConfigJson, "lcd_server_ip2", cJSON_CreateString(emu_conf.lcd_server_ip[1]));
        }

        if(strcmp(cJSON_GetObjectItem(pConfigJson, "lcd_server_ip3")->valuestring,emu_conf.lcd_server_ip[2])!=0){
            cJSON_ReplaceItemInObject(pConfigJson, "lcd_server_ip3", cJSON_CreateString(emu_conf.lcd_server_ip[2]));
        }

        if(strcmp(cJSON_GetObjectItem(pConfigJson, "lcd_server_ip4")->valuestring,emu_conf.lcd_server_ip[3])!=0){
            cJSON_ReplaceItemInObject(pConfigJson, "lcd_server_ip4", cJSON_CreateString(emu_conf.lcd_server_ip[3]));
        }

        if(strcmp(cJSON_GetObjectItem(pConfigJson, "lcd_server_ip5")->valuestring,emu_conf.lcd_server_ip[4])!=0){
            cJSON_ReplaceItemInObject(pConfigJson, "lcd_server_ip5", cJSON_CreateString(emu_conf.lcd_server_ip[4]));
        }

        if(strcmp(cJSON_GetObjectItem(pConfigJson, "lcd_server_ip6")->valuestring,emu_conf.lcd_server_ip[5])!=0){
            cJSON_ReplaceItemInObject(pConfigJson, "lcd_server_ip6", cJSON_CreateString(emu_conf.lcd_server_ip[5]));
        }

         if(strcmp(cJSON_GetObjectItem(pConfigJson, "plc_server_ip")->valuestring,emu_conf.plc_server_ip)!=0){
            cJSON_ReplaceItemInObject(pConfigJson, "plc_server_ip", cJSON_CreateString(emu_conf.plc_server_ip));
        }
    return 0;
}

int form_get_post()
{
    char *ret = NULL;
   
    int len, i;
            // 获取请求类型 get或者post
            ret = getenv("REQUEST_METHOD");
            if (ret == NULL)
            {
                printf("获取method失败！");
                return -1;
    }

    if (strncmp(ret, "GET", 3) == 0)
    {
        get_rep();
    }
    else if (strncmp(ret, "POST", 4) == 0)
    {
        // 查看post的请求
        // CONTENT_LENGTH 客户端向标准输入设备发送的数据长度，单位为字节
      
        int ret;

        
        char res_data[50][256];
        int len=0,conf_len=0;

       
        char conf[1024];
        // cJSON* configJson;
        len = atoi(getenv("CONTENT_LENGTH"));
        char info[len];
        for (i = 0; i < len; i++)
        {
            info[i] = (char)fgetc(stdin);
        }

        printf("<script>");
            printf("console.log(\"html:post: %s\");",info);
        printf("</script>");

        Analysis_data(info,res_data,&len);

        config_assignment(res_data,len);

        ret=post_read_config(conf);
        cJSON *configJson = cJSON_Parse(conf);
      
        Json_assignment(configJson);

        char *cjson_str = cJSON_Print(configJson);

        ret = write_config(cjson_str);
        printf("<iframe id=\"ifr_con\" border=\"0\" marginwidth=\"0\" framespacing=\"2\" marginheight=\"0\" src=\"\" frameborder=\"0\" noresize=\"noresize\" width=\"800\" scrolling=\"no\" height=\"1000\" allowtransparency=\"allowtransparency\" vspale=\"0\"></iframe>");
        printf("<script>");
           char apath[]="";
                    printf("function myFunction()");
                    printf("{");
                     if(ret>0){
                        printf("alert(\"修改成功\");");
                     }else{
                        printf("{alert(\"修改失败\");");
                     }
                    printf("document.getElementById(\"ifr_con\").src = \"../emuinfo_cfg.html\";");
                     printf("}");
                    printf("myFunction()");
        printf("</script>");
    

       cJSON_Delete(configJson);//清除结构体 
    }
    else
    {
        printf("<h3>form请求类型未知</h3>");
        return -1;
    }
    return 0;
}


int main(int argv, char *argc[])
{
    // http协议
    printf("Content-Type:text/html;charset=utf-8\r\n");
    printf("\r\n");

    form_get_post();
    return 0;
}