#ifndef __UDP_SOCKET_TUTORIAL_H__
#define __UDP_SOCKET_TUTORIAL_H__

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

class UDPSocketDemo {
public:
    void create_udp_socket() {
        std::cout << "\n🔧 创建UDP Socket（电报机）" << std::endl;
        
        // 🎯 关键参数：SOCK_DGRAM 表示使用UDP
        int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
        
        if (udp_socket < 0) {
            std::cerr << "❌ 创建UDP Socket失败！" << std::endl;
            return;
        }
        
        std::cout << "✅ 创建成功！Socket描述符: " << udp_socket << std::endl;
        std::cout << "   💡 注意：SOCK_DGRAM = 数据报模式（UDP）" << std::endl;
        std::cout << "   对比：SOCK_STREAM = 流模式（TCP）" << std::endl;
        
        // 演示UDP和TCP Socket的区别
        std::cout << "\n📊 UDP vs TCP Socket创建对比：" << std::endl;
        std::cout << "   相同点：都用socket()函数创建" << std::endl;
        std::cout << "   不同点：第二个参数不同：" << std::endl;
        std::cout << "     UDP: SOCK_DGRAM (数据报)" << std::endl;
        std::cout << "     TCP: SOCK_STREAM (流)" << std::endl;
        
        close(udp_socket);
    }
};

class UDPFunctions {
public:
    void demonstrate_sendto() {
        std::cout << "\n📤 sendto() - 发送UDP数据报" << std::endl;
        std::cout << "   比喻：写一封短信，写上收件人地址，然后寄出" << std::endl;
        
        // 创建UDP Socket
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        
        // 准备目标地址（收信人信息）
        struct sockaddr_in dest_addr;
        memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(8888);  // 目标端口
        inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr);  // 目标IP
        
        // 要发送的消息
        const char* message = "Hello UDP!";
        
        std::cout << "\n📝 准备发送：" << std::endl;
        std::cout << "   消息: \"" << message << "\"" << std::endl;
        std::cout << "   目标: 127.0.0.1:8888" << std::endl;
        
        // 🎯 关键：使用sendto发送
        ssize_t sent_bytes = sendto(
            sock,                    // UDP Socket
            message,                 // 要发送的数据
            strlen(message),         // 数据长度
            0,                      // 标志位（通常为0）
            (struct sockaddr*)&dest_addr,  // 目标地址
            sizeof(dest_addr)       // 地址长度
        );
        
        if (sent_bytes < 0) {
            std::cerr << "❌ 发送失败！" << std::endl;
        } else {
            std::cout << "✅ 发送成功！发送了 " << sent_bytes << " 字节" << std::endl;
        }
        
        close(sock);
    }
    
    void demonstrate_recvfrom() {
        std::cout << "\n📥 recvfrom() - 接收UDP数据报" << std::endl;
        std::cout << "   比喻：收到一封短信，同时知道是谁寄的" << std::endl;
        
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        
        // 绑定本地地址（设置信箱地址）
        struct sockaddr_in my_addr;
        memset(&my_addr, 0, sizeof(my_addr));
        my_addr.sin_family = AF_INET;
        my_addr.sin_addr.s_addr = INADDR_ANY;  // 监听所有网卡
        my_addr.sin_port = htons(8888);       // 端口8888
        
        bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr));
        
        std::cout << "\n📮 信箱已设置：" << std::endl;
        std::cout << "   地址: 0.0.0.0:8888" << std::endl;
        std::cout << "   状态: 等待短信..." << std::endl;
        
        char buffer[1024];
        struct sockaddr_in sender_addr;  // 用于保存发送者地址
        socklen_t sender_len = sizeof(sender_addr);
        
        // 🎯 关键：使用recvfrom接收
        ssize_t recv_bytes = recvfrom(
            sock,                    // UDP Socket
            buffer,                  // 接收缓冲区
            sizeof(buffer) - 1,      // 缓冲区大小
            0,                      // 标志位
            (struct sockaddr*)&sender_addr,  // 发送者地址（出参）
            &sender_len              // 地址长度（出参）
        );
        
        if (recv_bytes < 0) {
            std::cerr << "❌ 接收失败！" << std::endl;
        } else {
            buffer[recv_bytes] = '\0';  // 添加字符串结束符
            
            // 获取发送者信息
            char sender_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &sender_addr.sin_addr, sender_ip, sizeof(sender_ip));
            int sender_port = ntohs(sender_addr.sin_port);
            
            std::cout << "\n📨 收到短信！" << std::endl;
            std::cout << "   发件人: " << sender_ip << ":" << sender_port << std::endl;
            std::cout << "   内容: \"" << buffer << "\"" << std::endl;
            std::cout << "   长度: " << recv_bytes << " 字节" << std::endl;
        }
        
        close(sock);
    }
};

#endif