#ifndef __SOCKET_TUTORIAL_H__
#define __SOCKET_TUTORIAL_H__

#include <iostream>
#include <string>
#include <iostream>
#include <cstring>  // memset函数
#include <arpa/inet.h>  // 网络函数

class TelephoneWizard {
public:
    void cast_spells() {
        std::cout << "\n🔮 学习七个Socket咒语：" << std::endl;
        
        // 咒语1: socket() - 召唤一部魔法电话
        std::cout << "\n1. socket() - 召唤魔法电话" << std::endl;
        std::cout << "   int phone = socket(AF_INET, SOCK_STREAM, 0);" << std::endl;
        std::cout << "   💡 AF_INET = 用IPv4协议打电话" << std::endl;
        std::cout << "   💡 SOCK_STREAM = 用TCP方式（可靠通话）" << std::endl;
        
        // 咒语2: bind() - 给你的电话一个号码
        std::cout << "\n2. bind() - 申请专属电话号码" << std::endl;
        std::cout << "   bind(phone, 地址结构, 地址长度);" << std::endl;
        std::cout << "   📍 告诉系统：'这个号码127.0.0.1:8888是我的！'" << std::endl;
        
        // 咒语3: listen() - 打开电话铃声
        std::cout << "\n3. listen() - 打开铃声，准备接听" << std::endl;
        std::cout << "   listen(phone, 5);" << std::endl;
        std::cout << "   🔔 最多允许5个人同时给你打电话" << std::endl;
        
        // 咒语4: accept() - 接听一个来电
        std::cout << "\n4. accept() - 拿起电话筒" << std::endl;
        std::cout << "   int new_phone = accept(phone, 对方地址, 地址长度);" << std::endl;
        std::cout << "   ⏳ 注意：这个函数会一直等，直到有电话打来！" << std::endl;
        std::cout << "   📞 返回一个新的电话，专门和这个人聊天" << std::endl;
        
        // 咒语5: connect() - 主动拨打电话
        std::cout << "\n5. connect() - 主动打电话" << std::endl;
        std::cout << "   connect(phone, 对方地址, 地址长度);" << std::endl;
        std::cout << "   📲 只有打电话的人（客户端）用这个" << std::endl;
        
        // 咒语6: send()/recv() - 通话的核心
        std::cout << "\n6. send()/recv() - 说话和倾听" << std::endl;
        std::cout << "   send(phone, '你好', 6, 0);" << std::endl;
        std::cout << "   recv(phone, 缓冲区, 大小, 0);" << std::endl;
        std::cout << "   💬 这是实际交流的部分！" << std::endl;
        
        // 咒语7: close() - 挂断电话
        std::cout << "\n7. close() - 礼貌地挂断" << std::endl;
        std::cout << "   close(phone);" << std::endl;
        std::cout << "   🚪 一定要记得挂电话，不然会浪费资源！" << std::endl;
    }
};

class NetworkBusinessCard {
public:
    void demonstrate() {
        std::cout << "\n📇 制作网络名片（sockaddr_in）：" << std::endl;
        
        // 1. 准备一张空白名片
        struct sockaddr_in address_card;
        
        // 2. 清空名片（避免有残留信息）
        memset(&address_card, 0, sizeof(address_card));
        std::cout << "   ✅ memset: 清空名片" << std::endl;
        
        // 3. 选择地址族（用哪种语言写地址）
        address_card.sin_family = AF_INET;  // 用IPv4格式
        std::cout << "   ✅ sin_family = AF_INET: 使用IPv4格式" << std::endl;
        
        // 4. 填写端口号（特别注意：要转换字节序！）
        int my_port = 8888;
        address_card.sin_port = htons(my_port);  // htons = host to network short
        std::cout << "   ✅ sin_port = htons(" << my_port << "): 设置端口" << std::endl;
        std::cout << "      🎯 为什么要转换？因为不同CPU理解数字的方式不同！" << std::endl;
        std::cout << "        你的电脑: 8888  →  网络世界: 0x22B8" << std::endl;
        
        // 5. 填写IP地址
        const char* my_ip = "127.0.0.1";  // 本地回环地址（就是自己）
        inet_pton(AF_INET, my_ip, &address_card.sin_addr);
        std::cout << "   ✅ inet_pton: 将\"" << my_ip << "\"转换为二进制" << std::endl;
        
        // 6. 显示最终名片
        std::cout << "\n🏷️ 最终网络名片：" << std::endl;
        std::cout << "   📍 地址格式: IPv4" << std::endl;
        std::cout << "   🏠 IP地址: " << my_ip << std::endl;
        std::cout << "   🔢 端口号: " << my_port << std::endl;
        
        // 演示字节序转换
        std::cout << "\n🔄 字节序转换演示：" << std::endl;
        uint16_t host_number = 8888;  // 你看到的数字
        uint16_t network_number = htons(host_number);  // 网络看到的数字
        
        std::cout << "   你看到的: " << host_number << " (0x" << std::hex << host_number << ")" << std::endl;
        std::cout << "   网络看到的: 0x" << network_number << std::dec << std::endl;
        std::cout << "   转换回来: " << ntohs(network_number) << std::endl;
    }
};

#endif