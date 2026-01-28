#ifndef __GAME_WITH_UDP_H__
#define __GAME_WITH_UDP_H__

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

class GameWithUDP {
public:
    void demonstrate_game_protocol() {
        std::cout << "\n🎮 UDP在在线游戏中的应用" << std::endl;
        
        std::cout << "\n📊 游戏数据特点：" << std::endl;
        std::cout << "   • 高频更新（位置、状态）" << std::endl;
        std::cout << "   • 实时性要求高" << std::endl;
        std::cout << "   • 可以容忍少量丢包" << std::endl;
        std::cout << "   ➡️ 完美匹配UDP的特性！" << std::endl;
        
        // 模拟游戏客户端
        std::cout << "\n👾 模拟游戏客户端发送位置信息：" << std::endl;
        
        int game_sock = socket(AF_INET, SOCK_DGRAM, 0);
        
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(7777);  // 游戏服务器端口
        inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
        
        // 模拟发送位置信息
        for (int i = 0; i < 5; i++) {
            // 生成位置数据
            float x = 100.0f + i * 10.0f;
            float y = 200.0f + i * 5.0f;
            
            // 创建游戏数据包
            struct GamePacket {
                uint32_t player_id = 1;
                uint32_t sequence;
                float pos_x;
                float pos_y;
                uint32_t timestamp;
            } packet;
            
            packet.sequence = i;
            packet.pos_x = x;
            packet.pos_y = y;
            packet.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count();
            
            std::cout << "\n帧 " << i << ":" << std::endl;
            std::cout << "  位置: (" << x << ", " << y << ")" << std::endl;
            std::cout << "  时间戳: " << packet.timestamp << std::endl;
            
            // 发送UDP数据包
            sendto(game_sock, &packet, sizeof(packet), 0,
                  (struct sockaddr*)&server_addr, sizeof(server_addr));
            
            std::cout << "  📤 已发送UDP数据包" << std::endl;
            
            // 等待下一帧
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        close(game_sock);
    }
};

#endif