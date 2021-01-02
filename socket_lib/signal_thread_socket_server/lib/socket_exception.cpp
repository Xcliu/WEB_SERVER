#include "socket_exception.h"

using namespace SONNIE;

socket_exception::socket_exception(const char * _error_mesg){
    error_mesg=_error_mesg;
}

const char *socket_exception::what(){
    return error_mesg.c_str();
}

//test code for exception 
// #include <iostream>
// #include <string>
// #include "socket_related_head_file.h"
// int main(){
//     try{
//         throw socket_exception("test error messages");
//     }catch(socket_exception &s_excep){
//         code_position_info();
//         std::cout<< s_excep.what()<<std::endl;
//     }
//     std::string str="hello";
//     socklen_t dest_len=10;
//     ssize_t mesg_len = sendto(3,str.c_str(),str.size(),0,NULL,dest_len);//发送响应
//     if(mesg_len==dest_len){
//         std::cout<<"the send buffer is overflowed"<<std::endl;
//     }else if(mesg_len<0){
//         systemcall_error_info();
//     }else if(mesg_len!=0){
//         std::cout<<"send successfully !"<<std::endl;
//     }
//     return 0;
// }