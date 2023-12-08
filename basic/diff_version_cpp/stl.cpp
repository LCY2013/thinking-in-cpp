//
// Created by fufeng on 2023/12/8.
//
#include <iostream>
#include <list>
#include <map>
#include <memory>

//stl容器新增的实用方法介绍

//原位构造与容器的emplace系列函数
class Test {
public:
    Test(int a, int b, int c) {
        ma = a;
        mb = b;
        mc = c;
        std::cout << "Test constructed." << std::endl;
    }

    ~Test() {
        std::cout << "Test destructed." << std::endl;
    }

    Test(const Test &rhs) {
        if (&rhs == this) {
            return;
        }
        this->ma = rhs.ma;
        this->mb = rhs.mb;
        this->mc = rhs.mc;
        std::cout << "Test copy-constructed" << std::endl;
    }

    // 移动构造函数
    Test(Test &&rhs) {
        if (&rhs == this) {
            return;
        }
        this->ma = rhs.ma;
        this->mb = rhs.mb;
        this->mc = rhs.mc;
        std::cout << "Test move-constructed" << std::endl;
    }

private:
    int ma;
    int mb;
    int mc;
};


void func1() {
    std::list<Test> collections;
    for (int i = 0; i < 10; ++i) {
        Test t(1 * i, 2 * i, 3 * i);
        collections.push_back(t);
    }
}

//上述代码在一个循环里面产生一个对象，然后将这个对象放入集合当中，这样的代码在实际开发中太常见了。
// 但是这样的代码存在严重的效率问题。
// 循环中的t对象在每次循环时，都分别调用一次构造函数、拷贝构造函数和析构函数。
// 这个过程示意如下：
// 构造函数：-> 拷贝构造函数：-> 析构函数：
// 循环10次，总共调用三十次。
// 但实际上我们的初衷是创建一个对象t，将其直接放入集合中，而不是将t作为一个中间临时产生的对象，
// 这样的话，总共只需要调用t的构造函数10次就可以了。
// C++11提供了一个在这种情形下替代push_back的方法——emplace_back，使用emplace_back，将函数中的代码改写一下：
void func2() {
    std::list<Test> collections;
    for (int i = 0; i < 10; i++) {
        collections.emplace_back(1 * i, 2 * i, 3 * i);
    }
}

//实际执行的时候，我们发现现在，只需要调用Test类的构造函数10次，大大地提高了执行效率。
//
//同理，对于提供insert/push/push_back/push_front等方法的STL容器，在C++11中也有对应的改进方法，
// 即emplace/emplace_back/emplace_front等方法。
// C++ Reference上将这里的emplace操作称之为“原位构造元素”（EmplaceConstructible）是非常贴切的。
//原方法	            C++ 11 改进方法	                方法含义
//push/insert	    emplace	                   在容器指定位置原位构造元素
//push_front	    emplace_front	           在容器首部原位构造元素
//push_back	        emplace_back	           在容器尾部原位构造元素
//除了使用emplace系列函数原位构造元素，也可以为Test类添加移动构造函数（Move Constructor）来复用产生的临时对象t以提高效率。

//std::map的try_emplace与insert_or_assign方法

//由于std::map中的元素的key是唯一的，所以在实际开发中我们经常会遇到这样一类需求：
// 即往某个map中插入元素时需要先检测map中指定的key是否存在，如果不存在才做插入操作，
// 如果存在，则直接取来使用；或者在指定key不存在时做插入操作，存在时做更新操作。
//
//以PC版QQ为例，好友列表中每个好友都对应一个userid，当我们双击某个QQ好友头像时，
// 如果与该好友的聊天对话框（这里使用ChatDialog 表示）已经创建，则直接激活显示，
// 如果不存在，则创建并显示之。
// 假设我们使用std::map来管理这些聊天对话框，在C++17之前，我们必须编写额外的逻辑去判断元素是否存在，上述逻辑可以编写成如下代码：
class ChatDialog {
//其他实现省略...
public:
    ChatDialog() {
        std::cout << "ChatDialog constructor" << std::endl;
    }

    ~ChatDialog() {
        std::cout << "ChatDialog destructor" << std::endl;
    }

    void activate() {
        //实现省略
    }
};

//用于管理所有聊天对话框的map，key是好友id，ChatDialog是聊天对话框指针
std::map<int64_t, ChatDialog *> m_ChatDialogs;

//双击好友头像后
void onDoubleClickFriendItem(int64_t userid) {
    auto targetChatDialog = m_ChatDialogs.find(userid);
    //好友对话框不存在，则创建之，并激活
    if (targetChatDialog == m_ChatDialogs.end()) {
        ChatDialog *pChatDialog = new ChatDialog();
        m_ChatDialogs.insert(std::pair<int64_t, ChatDialog *>(userid, pChatDialog));
        pChatDialog->activate();
    }
        //好友对话框存在，直接激活
    else {
        targetChatDialog->second->activate();
    }
}

//在C++17中map提供了一个try_emplace这样的方法，该方法会检测指定的key是否存在，如果存在，则什么也不做。函数签名如下：
//template <class... Args>
//pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);
//
//template <class... Args>
//pair<iterator, bool> try_emplace(key_type&& k, Args&&... args);
//
//template <class... Args>
//iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args);
//
//template <class... Args>
//iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args);

//上述函数签名中， 参数k表示需要插入的key，args参数是一个不定参数，表示构造value对象需要传给构造函数的参数，hint参数可以指定插入位置。
//
//在前两种签名形式中， try_emplace的返回值是一个std::pair<T1, T2>类型，
// 其中T2是一个bool类型表示元素是否成功插入map中，T1是一个map的迭代器，如果插入成功，则返回指向插入位置的元素的迭代器，
// 如果插入失败，则返回map中已存在的相同key元素的迭代器。
// 我们用try_emplace改写上面的代码（这里我们不关心插入位置，因此使用前两个签名）：
//C++ 17 版本1
void onDoubleClickFriendItemV2(int64_t userid) {
    //结构化绑定和try_emplace都是 C++17语法
    auto [iter, isInserted] = m_ChatDialogs.try_emplace(userid);
    if (isInserted)
        iter->second = new ChatDialog();

    iter->second->activate();
}

//g++ -g -Wall stl.cpp -std=c++17
void func3() {
    //906106643 是userid
    onDoubleClickFriendItemV2(906106643L);
    //906106644 是userid
    onDoubleClickFriendItemV2(906106644L);
    //906106643 是userid
    onDoubleClickFriendItemV2(906106643L);
}

//使用了try_emplace改写后的代码简洁了许多。
// 但是上述代码存在一个注意事项，由于std::map<int64_t, ChatDialog*> m_ChatDialogs的value是指针类型（ChatDialog*），
// 而try_emplace第二个参数支持的是构造一个ChatDialog对象，而不是指针类型，
// 因此，当某个userid不存在时，成功插入map后会导致相应的value为空指针。
// 因此，我们利用inserted的值按需new出一个ChatDialog。
// 当然，新的C++语言规范（C++11及后续版本）提供了灵活而强大的智能指针以后，
// 我们就不应该再有任何理由去使用裸指针了，因此上述代码可以使用std::unique_ptr智能指针类型来重构：

//用于管理所有聊天对话框的map，key是好友id，value是ChatDialog是聊天对话框智能指针
std::map<int64_t, std::unique_ptr<ChatDialog>> m_up_ChatDialogs;

//C++ 17 版本2
void onDoubleClickFriendItemV3(int64_t userid) {
    //结构化绑定和try_emplace都是 C++17语法
    auto spChatDialog = std::make_unique<ChatDialog>();
    auto [iter, isInserted] = m_up_ChatDialogs.try_emplace(userid, std::move(spChatDialog));
    iter->second->activate();
}

//g++ -g -Wall stl.cpp -std=c++17
void func4() {
    //906106643 是userid
    onDoubleClickFriendItemV3(906106643L);
    //906106644 是userid
    onDoubleClickFriendItemV3(906106644L);
    //906106643 是userid
    onDoubleClickFriendItemV3(906106643L);
}

//上述代码将map的类型从std::map<int64_t, ChatDialog*> 改为std::map<int64_t, std::unique_ptr<ChatDialog>> ，让程序自动管理聊天对话框对象。
//输出了三次构造和析构，说明每次插入时都new出一个ChatDialog对象，并在插入成功后激活。
//ChatDialog constructor
//ChatDialog constructor
//ChatDialog constructor
//ChatDialog destructor
//ChatDialog destructor
//ChatDialog destructor

//上述代码中构造函数和析构函数均被调用了3次，
// 实际上，按最原始的逻辑（上文中普通版本）ChatDialog应该只被构造和析构2次，多出来的一次是因为在try_emplace时，
// 无论某个userid是否存在于map中均创建一个ChatDialog对象（这个是额外的、用不上的对象），
// 由于这个对象并没有被用上，当出了函数 onDoubleClickFriendItemV3 作用域后，
// 智能指针对象spChatDialog被析构，进而导致这个额外的、用不上的ChatDialog对象被析构。
// 这相当于做了一次无用功。为此，可以继续优化我们的代码如下：
//用于管理所有聊天对话框的map，key是好友id，value是ChatDialog是聊天对话框智能指针
std::map<int64_t, std::unique_ptr<ChatDialog>> m_up2_ChatDialogs;

//C++ 17版本3
void onDoubleClickFriendItemV4(int64_t userid) {
    //结构化绑定和try_emplace都是 C++17语法
    auto [iter, isInserted] = m_up2_ChatDialogs.try_emplace(userid, nullptr);
    if (isInserted) {
        // 按需创建chatdialog
        auto spChatDialog = std::make_unique<ChatDialog>();
        iter->second = std::move(spChatDialog);
    }
    iter->second->activate();
}

//g++ -g -Wall stl.cpp -std=c++17
void func5() {
    //906106643 是userid
    onDoubleClickFriendItemV4(906106643L);
    //906106644 是userid
    onDoubleClickFriendItemV4(906106644L);
    //906106643 是userid
    onDoubleClickFriendItemV4(906106643L);
}

//输出如下：
//ChatDialog constructor
//ChatDialog constructor
//ChatDialog destructor
//ChatDialog destructor

//auto [iter, inserted] = m_up2_ChatDialogs.try_emplace(userid, nullptr);
// 这种函数返回值有一个布尔变量表示操作是否成功，如果成功，另外一个返回值含有函数调用成功后的数据的模式，
// 我们称为ok-idiom模式，golang语言的开发者应该非常熟悉ok-idiom模式。

//为了演示try_emplace函数支持原位构造，我们将map的value类型改成ChatDialog类型，
// 当然，这里只是为了演示方便，实际开发中对于非POD类型的复杂数据类型，在stl容器中应该存储其指针或者智能指针类型，而不是对象本身。
// 修改后的代码如下：
class ChatDialogOne {
//其他实现省略...
public:
    ChatDialogOne(int64_t userid) : user_id(userid) {
        std::cout << "ChatDialog constructor" << std::endl;
    }

    ~ChatDialogOne() {
        std::cout << "ChatDialog destructor" << std::endl;
    }

    void activate() {
        //实现省略
    }

private:
    int64_t user_id;
};

//用于管理所有聊天对话框的map，key是好友id，value是ChatDialog是聊天对话框对象
std::map<int64_t, ChatDialogOne> m_ChatDialogs2;

//C++ 17版本4
void onDoubleClickFriendItemV5(int64_t userid) {
    //结构化绑定和try_emplace都是 C++17语法
    auto [iter, isInserted] = m_ChatDialogs2.try_emplace(userid, userid);
    iter->second.activate();
}

void func6() {
    //906106643 是userid
    onDoubleClickFriendItemV5(906106643L);
    //906106644 是userid
    onDoubleClickFriendItemV5(906106644L);
    //906106643 是userid
    onDoubleClickFriendItemV5(906106643L);
}

//上述代码中，我们为ChatDialog类的构造函数增加了一个userid参数，
// 因此当我们调用try_emplace方法时，需要传递一个参数，
// 这样try_emplace就会根据map中是否已存在同样的userid按需构造ChatDialog对象。
// 程序执行结果和上一个代码示例应该是一样的：
//ChatDialog constructor
//ChatDialog constructor
//ChatDialog destructor
//ChatDialog destructor

//上面我们介绍了如果map中指定的key不存在则插入，存在则使用的情形。
// 我们再来介绍一下如果map中指定的key不存在则插入，存在则更新其value值的情形。
// C++17为此也为map容器新增了一个这样的方法insert_or_assign，让我们不再像C++17标准之前，
// 需要额外编写先判断是否存在，不存在则插入，存在则更新的代码了，这次我们可以直接一步到位。
// insert_or_assign的函数签名如下：
//template <class M>
//pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
//
//template <class M>
//pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);
//
//template <class M>
//iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
//
//template <class M>
//iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);

//g++ -Wall -g stl.cpp -std=c++17
void func7() {
    std::map<std::string, int> mapUsersAge{{"fufeng", 18},
                                           {"magic",  17}};
    mapUsersAge.insert_or_assign("fufeng", 19);
    mapUsersAge.insert_or_assign("luo", 18);

    for (auto &[username, age]: mapUsersAge) {
        std::cout << username << " is " << age << " years old" << std::endl;
    }
}

//C++11/17为stl容器新增的几个实用方法，合理利用它们会让我们的程序变得更简洁、更高效。
// 其实新的标准一致在不断改进和优化已有stl各个容器，如果读者的工作需要经常与这些容器打交道，
// 平常留意C++新标准涉及到它们的新动态。

int main() {
    //func1();
    //func2();
    //func3();
    //func4();
    //func5();
    //func6();
    func7();
    return 0;
}

