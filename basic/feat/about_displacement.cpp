//
// Created by fufeng on 2023/12/10.
//
#include <iostream>
#include <bitset>

using namespace std;

int main() {
    char a1 = 0x63;  // 0110 0011
    a1 = (a1 << 4); // 0011 0000
    printf("0x%x\n", a1); // 0x30

    a1 = 0x63;  // 0110 0011
    a1 = (a1 >> 4); // 0000 0110
    printf("0x%x\n", a1); // 0x6

    char a2 = 0x95;  // 1001 0101
    a2 = (a2 << 4); // 0101 0000
    printf("0x%x\n", a2); // 0x50

    a2 = 0x95;  // 1001 0101
    a2 = (a2 >> 4); // 0000 1001 ? c语言实际不是这个值，具体看编译器，可能按算术右移(1111 1001)，可能按逻辑右移(0000 1001)
    // 考虑到通用型跨平台，c语言建议讲数据转换成无符号数进行右移运算
    printf("0x%x\n", a2); // 0xfffffff9

    cout << "--------unsigned char--------" << endl;
    unsigned char a3 = 0x63;  // 0110 0011
    a3 = (a3 << 4); // 0011 0000
    printf("0x%x\n", a3); // 0x30

    a3 = 0x63;  // 0110 0011
    a3 = (a3 >> 4); // 0000 0110
    printf("0x%x\n", a3); // 0x6

    unsigned char a4 = 0x95;  // 1001 0101
    a4 = (a4 << 4); // 0101 0000
    printf("0x%x\n", a4); // 0x50

    a4 = 0x95;  // 1001 0101
    a4 = (a4 >> 4); // 0000 1001 ? c语言实际不是这个值，具体看编译器，可能按算术右移(1111 1001)，可能按逻辑右移(0000 1001)
    // 考虑到通用型跨平台，c语言建议讲数据转换成无符号数进行右移运算
    printf("0x%x\n", a4); // 0xfffffff9

    // c语言移位问题
    // 逻辑右移：直接右移补0
    // 算术右移：符号位不变，正数右移补0，负数右移补1
    // c语言好的方案：右移只对无符号数
    // 移位操作位数的限制 ： 移位数不能超过数据类型的位数且大于0，比如int类型，移位数不能超过32位
    // c语言设计位移操作时需要考虑整数表示的上下文环境
    // c++中的解决方案：bitset

    cout << "--------权限判断位移--------" << endl;

    // 权限问题
    // 0000 0000
    const unsigned char priv = 0xFF;
    const unsigned char P_BACKUP = (1 << 6);
    const unsigned char P_ADMIN = (1 << 7);
    printf("0x%x\n", P_BACKUP);
    printf("0x%x\n", P_ADMIN);

    if (priv & P_BACKUP) {
        cout << "backup" << endl;
    }
    if (priv & P_ADMIN) {
        cout << "admin" << endl;
    }

    cout << "--------bitset--------" << endl;
    //bitset<10> priv_bitset = 0xFF;
    //bitset<10> priv_bitset = 0x7F;
    bitset<10> priv_bitset = 0x8F;
    bitset<10> P_BACKUP_bitset = 1 << 6;
    bitset<10> P_ADMIN_bitset = 1 << 7;
    cout << priv_bitset << endl;
    cout << P_BACKUP_bitset << endl;
    cout << P_ADMIN_bitset << endl;

    if ((priv_bitset & P_BACKUP_bitset) == P_BACKUP_bitset) {
        cout << "backup" << endl;
    }
    if ((priv_bitset & P_ADMIN_bitset) == P_ADMIN_bitset) {
        cout << "admin" << endl;
    }

    return 0;
}
