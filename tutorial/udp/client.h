#ifndef __UDP_SOCKET_TUTORIAL_CLIENT_H__
#define __UDP_SOCKET_TUTORIAL_CLIENT_H__

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class UDPClientDemo {
public:
    void demonstrate_tcp_vs_udp() {
        std::cout << "\n🆚 TCP客户端 vs UDP客户端对比" << std::endl;
        
        std::cout << "\n👔 TCP客户端流程（打电话）：" << std::endl;
        std::cout << "1. socket() - 买电话" << std::endl;
        std::cout << "2. connect() - 拨号（必须！）" << std::endl;
        std::cout << "3. send()/recv() - 通话" << std::endl;
        std::cout << "4. close() - 挂电话" << std::endl;
        
        std::cout << "\n⚡ UDP客户端流程（发电报）：" << std::endl;
        std::cout << "1. socket() - 买电报机" << std::endl;
        std::cout << "2. 🎯 不需要connect()！" << std::endl;
        std::cout << "3. sendto()/recvfrom() - 发/收电报" << std::endl;
        std::cout << "4. close() - 关机" << std::endl;
        
        std::cout << "\n💡 关键区别：" << std::endl;
        std::cout << "   TCP必须connect()建立连接" << std::endl;
        std::cout << "   UDP不需要连接，直接sendto()" << std::endl;
    }
    
    void create_udp_client() {
        std::cout << "\n📱 创建UDP客户端（发报员）" << std::endl;
        
        // 1. 创建UDP Socket
        int client_sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (client_sock < 0) {
            std::cerr << "❌ 创建电报机失败！" << std::endl;
            return;
        }
        std::cout << "✅ 电报机准备就绪！" << std::endl;
        
        // 2. 准备服务器地址
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8888);
        inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
        
        std::string user_input;
        char buffer[1024];
        
        std::cout << "\n💬 开始UDP Echo测试 (输入'bye'退出)" << std::endl;
        
        while (true) {
            // 获取用户输入
            std::cout << "发报: ";
            std::getline(std::cin, user_input);
            
            if (user_input.empty()) continue;
            
            // 3. 发送电报（不需要connect！）
            ssize_t sent_len = sendto(
                client_sock,
                user_input.c_str(),
                user_input.length(),
                0,
                (struct sockaddr*)&server_addr,
                sizeof(server_addr)
            );
            
            if (sent_len < 0) {
                std::cerr << "❌ 发送失败！" << std::endl;
                break;
            }
            
            std::cout << "📤 已发送 " << sent_len << " 字节" << std::endl;
            
            // 检查退出命令
            if (user_input == "bye" || user_input == "quit") {
                std::cout << "👋 结束发报" << std::endl;
                break;
            }
            
            // 4. 等待回电
            std::cout << "⏳ 等待回电..." << std::endl;
            
            struct sockaddr_in reply_addr;
            socklen_t reply_len = sizeof(reply_addr);
            memset(buffer, 0, sizeof(buffer));
            
            ssize_t recv_len = recvfrom(
                client_sock,
                buffer,
                sizeof(buffer) - 1,
                0,
                (struct sockaddr*)&reply_addr,
                &reply_len
            );
            
            if (recv_len < 0) {
                std::cerr << "❌ 接收失败！" << std::endl;
                break;
            }
            
            buffer[recv_len] = '\0';
            std::cout << "📨 收到回电: \"" << buffer << "\"" << std::endl;
            
            // 可选：验证回复来源
            char reply_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &reply_addr.sin_addr, reply_ip, sizeof(reply_ip));
            int reply_port = ntohs(reply_addr.sin_port);
            std::cout << "   来自: " << reply_ip << ":" << reply_port << std::endl;
        }
        
        close(client_sock);
        std::cout << "📞 电报机关机" << std::endl;
    }
};

#endif