#include <iostream>
#include <string>
#include "WEB_SERVER/socket_lib/socket_common_define.h"
#include "WEB_SERVER/socket_lib/socket_exception.h"

int main(){
    try{
        throw SONNIE::socket_exception("test error messages: ");
    }catch(SONNIE::socket_exception &s_excep){
        code_position_info();
        std::cout<< s_excep.what()<<std::endl;
    }
    std::string str="hello";
    socklen_t dest_len=10;
    ssize_t mesg_len = sendto(3,str.c_str(),str.size(),0,NULL,dest_len);//发送响应
    if(mesg_len==dest_len){
        std::cout<<"the send buffer is overflowed"<<std::endl;
    }else if(mesg_len<0){
        systemcall_error_info();
    }else if(mesg_len!=0){
        std::cout<<"send successfully !"<<std::endl;
    }
    return 0;
}