#ifndef __SOCKET_TUTORIAL_CLIENT_H__
#define __SOCKET_TUTORIAL_CLIENT_H__

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class EchoTester {
public:
    void test_server() {
        std::cout << "\n🧪 开始测试Echo服务器..." << std::endl;
        
        // 1. 买一部电话
        int my_phone = socket(AF_INET, SOCK_STREAM, 0);
        if (my_phone < 0) {
            std::cerr << "❌ 买电话失败！" << std::endl;
            return;
        }
        
        // 2. 准备服务器的名片
        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(8888);
        inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
        
        std::cout << "📞 正在呼叫 127.0.0.1:8888 ..." << std::endl;
        
        // 3. 拨打电话（connect）
        if (connect(my_phone, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
            std::cerr << "❌ 打电话失败！" << std::endl;
            close(my_phone);
            return;
        }
        
        std::cout << "✅ 电话接通了！" << std::endl;
        
        // 4. 接收服务器的欢迎消息
        // char welcome[1024];
        // memset(welcome, 0, sizeof(welcome));
        // int bytes_received = recv(my_phone, welcome, sizeof(welcome) - 1, 0);
        
        // if (bytes_received > 0) {
        //     std::cout << "📨 服务器说: " << welcome;
        // }
        
        // 5. 开始对话测试
        std::cout << "\n💬 开始回声测试 (输入'bye'退出)" << std::endl;
        
        std::string user_input;
        char response[1024];
        
        while (true) {
            // 获取用户输入
            std::cout << "你说: ";
            std::getline(std::cin, user_input);
            
            if (user_input.empty()) continue;
            
            // 发送给服务器
            send(my_phone, user_input.c_str(), user_input.length(), 0);
            
            // 检查退出
            if (user_input == "bye" || user_input == "quit") {
                std::cout << "👋 退出测试" << std::endl;
                break;
            }
            
            // 接收回声
            memset(response, 0, sizeof(response));
            int bytes_received = recv(my_phone, response, sizeof(response) - 1, 0);
            
            if (bytes_received <= 0) break;
            
            std::cout << "回声: " << response << std::endl;
        }
        
        // 6. 挂断电话
        close(my_phone);
        std::cout << "📞 测试完成，挂断电话" << std::endl;
    }
};

#endif