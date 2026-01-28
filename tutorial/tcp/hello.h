#ifndef __SOCKET_TUTORIAL_HELLO_H__
#define __SOCKET_TUTORIAL_HELLO_H__

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class GreetingServer {
public:
    void say_hello() {
        std::cout << "\n🎁 开始建造'Hello'服务器..." << std::endl;
        
        // 1. 买一部电话（创建socket）
        int server_phone = socket(AF_INET, SOCK_STREAM, 0);
        if (server_phone < 0) {
            std::cerr << "❌ 买电话失败！" << std::endl;
            return;
        }
        std::cout << "✅ 买到电话了！编号: " << server_phone << std::endl;
        
        // 2. 允许重复使用地址（避免"地址被占用"错误）
        int yes = 1;
        setsockopt(server_phone, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        
        // 3. 制作名片（地址结构）
        struct sockaddr_in my_address;
        memset(&my_address, 0, sizeof(my_address));
        my_address.sin_family = AF_INET;
        my_address.sin_addr.s_addr = INADDR_ANY;  // 监听所有网卡
        my_address.sin_port = htons(8888);  // 用8888端口
        
        // 4. 申请电话号码（bind）
        if (bind(server_phone, (struct sockaddr*)&my_address, sizeof(my_address)) < 0) {
            std::cerr << "❌ 申请号码失败！" << std::endl;
            close(server_phone);
            return;
        }
        std::cout << "✅ 申请到号码: 0.0.0.0:8888" << std::endl;
        
        // 5. 打开电话铃声（listen）
        if (listen(server_phone, 5) < 0) {  // 最多5个等待来电
            std::cerr << "❌ 打开铃声失败！" << std::endl;
            close(server_phone);
            return;
        }
        std::cout << "✅ 打开铃声，等待来电..." << std::endl;
        
        // 6. 等待电话（accept - 会一直等直到有电话）
        std::cout << "⏳ 等待有人打电话来..." << std::endl;
        
        struct sockaddr_in caller_address;
        socklen_t caller_len = sizeof(caller_address);
        int caller_phone = accept(server_phone, (struct sockaddr*)&caller_address, &caller_len);
        
        if (caller_phone < 0) {
            std::cerr << "❌ 接电话失败！" << std::endl;
            close(server_phone);
            return;
        }
        
        // 看看谁打来的
        char caller_ip[20];
        inet_ntop(AF_INET, &caller_address.sin_addr, caller_ip, sizeof(caller_ip));
        std::cout << "🎉 有人打电话来了！来自: " << caller_ip << std::endl;
        
        // 7. 热情地说"Hello"（send）
        const char* welcome_message = "Hello! 欢迎来到我的第一个Socket服务器！\n";
        int bytes_sent = send(caller_phone, welcome_message, strlen(welcome_message), 0);
        
        if (bytes_sent < 0) {
            std::cerr << "❌ 说话失败！" << std::endl;
        } else {
            std::cout << "✅ 说了" << bytes_sent << "个字节的欢迎语！" << std::endl;
            std::cout << "   内容: " << welcome_message;
        }
        
        // 8. 挂断电话（close）
        close(caller_phone);
        close(server_phone);
        std::cout << "📞 挂断电话，任务完成！" << std::endl;
    }
};

#endif