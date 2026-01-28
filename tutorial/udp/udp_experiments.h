#ifndef __UDP_SOCKET_TUTORIAL_HELLO_H__
#define __UDP_SOCKET_TUTORIAL_HELLO_H__

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

class UDPExperiments {
public:
    void experiment_connectionless() {
        std::cout << "\n🔬 实验1：UDP的无连接特性" << std::endl;
        
        // 创建两个UDP Socket
        int sock1 = socket(AF_INET, SOCK_DGRAM, 0);
        int sock2 = socket(AF_INET, SOCK_DGRAM, 0);
        
        // 绑定不同端口
        struct sockaddr_in addr1, addr2;
        memset(&addr1, 0, sizeof(addr1));
        memset(&addr2, 0, sizeof(addr2));
        
        addr1.sin_family = AF_INET;
        addr1.sin_addr.s_addr = INADDR_ANY;
        addr1.sin_port = htons(9991);
        
        addr2.sin_family = AF_INET;
        addr2.sin_addr.s_addr = INADDR_ANY;
        addr2.sin_port = htons(9992);
        
        bind(sock1, (struct sockaddr*)&addr1, sizeof(addr1));
        bind(sock2, (struct sockaddr*)&addr2, sizeof(addr2));
        
        std::cout << "创建了两个UDP Socket：" << std::endl;
        std::cout << "  Socket1: 端口 9991" << std::endl;
        std::cout << "  Socket2: 端口 9992" << std::endl;
        std::cout << "\n🎯 实验：Socket1向Socket2发送消息" << std::endl;
        std::cout << "   注意：没有调用connect()！" << std::endl;
        
        // 准备目标地址
        struct sockaddr_in target_addr = addr2;
        
        const char* message = "Hello from socket1!";
        sendto(sock1, message, strlen(message), 0,
              (struct sockaddr*)&target_addr, sizeof(target_addr));
        
        std::cout << "📤 消息已发送，无需建立连接！" << std::endl;
        
        // 接收消息
        char buffer[1024];
        struct sockaddr_in sender_addr;
        socklen_t sender_len = sizeof(sender_addr);
        
        ssize_t recv_len = recvfrom(sock2, buffer, sizeof(buffer)-1, 0,
                                   (struct sockaddr*)&sender_addr, &sender_len);
        
        if (recv_len > 0) {
            buffer[recv_len] = '\0';
            char sender_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &sender_addr.sin_addr, sender_ip, sizeof(sender_ip));
            
            std::cout << "📨 Socket2收到消息: \"" << buffer << "\"" << std::endl;
            std::cout << "   来自: " << sender_ip << ":" << ntohs(sender_addr.sin_port) << std::endl;
        }
        
        close(sock1);
        close(sock2);
    }
    
    void experiment_message_boundary() {
        std::cout << "\n🔬 实验2：UDP的消息边界" << std::endl;
        std::cout << "   TCP是字节流，没有边界" << std::endl;
        std::cout << "   UDP是数据报，每个包都是完整消息" << std::endl;
        
        int sock1 = socket(AF_INET, SOCK_DGRAM, 0);
        int sock2 = socket(AF_INET, SOCK_DGRAM, 0);
        
        struct sockaddr_in addr1, addr2;
        memset(&addr1, 0, sizeof(addr1));
        memset(&addr2, 0, sizeof(addr2));
        
        addr1.sin_family = AF_INET;
        addr1.sin_addr.s_addr = INADDR_ANY;
        addr1.sin_port = htons(9993);
        
        addr2.sin_family = AF_INET;
        addr2.sin_addr.s_addr = INADDR_ANY;
        addr2.sin_port = htons(9994);
        
        bind(sock1, (struct sockaddr*)&addr1, sizeof(addr1));
        bind(sock2, (struct sockaddr*)&addr2, sizeof(addr2));
        
        // 发送多个消息
        std::cout << "\n发送三个消息：" << std::endl;
        const char* messages[] = {"Hello", "World", "UDP"};
        
        struct sockaddr_in target_addr = addr2;
        
        for (int i = 0; i < 3; i++) {
            sendto(sock1, messages[i], strlen(messages[i]), 0,
                  (struct sockaddr*)&target_addr, sizeof(target_addr));
            std::cout << "  发送: \"" << messages[i] << "\"" << std::endl;
        }
        
        // 接收消息
        std::cout << "\n接收消息：" << std::endl;
        char buffer[1024];
        struct sockaddr_in sender_addr;
        socklen_t sender_len = sizeof(sender_addr);
        
        for (int i = 0; i < 3; i++) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t recv_len = recvfrom(sock2, buffer, sizeof(buffer)-1, 0,
                                       (struct sockaddr*)&sender_addr, &sender_len);
            if (recv_len > 0) {
                buffer[recv_len] = '\0';
                std::cout << "  收到完整消息: \"" << buffer << "\"" << std::endl;
                std::cout << "  长度: " << recv_len << " 字节" << std::endl;
            }
        }
        
        close(sock1);
        close(sock2);
    }
};

#endif