/*
 * 线程安全的单例模式
 */

#include <iostream>
#include <mutex>
#include <pthread.h>

/************************** 线程不安全的懒汉式单例模式 **************************/
class NoLockSingleInstance
{
public:
    static NoLockSingleInstance *GetInstance();
    static void deleteInstance();
    void Address();

private:
    // 将其构造和析构成为私有的, 禁止外部构造和析构
    NoLockSingleInstance();
    ~NoLockSingleInstance();

    // 将其拷贝构造和赋值构造成为私有函数, 禁止外部拷贝和赋值
    NoLockSingleInstance(const NoLockSingleInstance &signal);
    const NoLockSingleInstance &operator=(const NoLockSingleInstance &signal);

private:
    static NoLockSingleInstance *m_SingleInstance;
};

// 初始化静态成员变量
NoLockSingleInstance *NoLockSingleInstance::m_SingleInstance = nullptr;

NoLockSingleInstance *NoLockSingleInstance::GetInstance()
{
    if (m_SingleInstance == nullptr) {
        m_SingleInstance = new (std::nothrow) NoLockSingleInstance; // 没有加锁是线程不安全的，当线程并发时会创建多个实例
    }

    return m_SingleInstance;
}

void NoLockSingleInstance::deleteInstance()
{
    if (m_SingleInstance) {
        delete m_SingleInstance;
        m_SingleInstance = nullptr;
    }
}

void NoLockSingleInstance::Address()
{
    std::cout << "实例内存地址:" << this << std::endl;
}

NoLockSingleInstance::NoLockSingleInstance()
{
    std::cout << "执行构造函数" << std::endl;
}

NoLockSingleInstance::~NoLockSingleInstance()
{
    std::cout << "执行析构函数" << std::endl;
}

// 线程函数
void *ThreadFunc(void *threadId)
{
    // 主线程与子线程分离，两者相互不干涉，子线程结束同时子线程的资源自动回收
    pthread_detach(pthread_self());

    // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
    int tid = *((int *)threadId);
    std::cout << "线程ID:" << tid << std::endl;

    // 打印实例地址
    NoLockSingleInstance::GetInstance()->Address();
    pthread_exit(nullptr);
}

constexpr int NUM_THREADS = 5;

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS] = {0};
    int indexes[NUM_THREADS] = {0}; // 用数组来保存i的值

    std::cout << "-----------main-----------" << std::endl;
    for (int threadId = 0; threadId < NUM_THREADS; ++threadId) {
        std::cout << "[main]创建线程:" << threadId << std::endl;
        
        indexes[threadId] = threadId;
        int ret = pthread_create(&threads[threadId], nullptr, ThreadFunc, (void *)&(indexes[threadId]));
        if (ret) {
            std::cout << "[main]创建线程失败，ret=" << ret << std::endl;
            exit(-1);
        }
    }

    // 手动释放单实例的资源
    NoLockSingleInstance::deleteInstance();
    std::cout << "-----------main-----------" << std::endl;
    return 0;
}
/************************** 线程不安全的懒汉单例模式 **************************/

/************************** 加锁线程安全的懒汉式单例模式 **************************/
class AddLockSingleInstance
{
public:
    static AddLockSingleInstance *&GetInstance();
    static void deleteInstance();
    void Address();

private:
    // 将其构造和析构成为私有的, 禁止外部构造和析构
    AddLockSingleInstance();
    ~AddLockSingleInstance();

    // 将其拷贝构造和赋值构造成为私有函数, 禁止外部拷贝和赋值
    AddLockSingleInstance(const AddLockSingleInstance &signal);
    const AddLockSingleInstance &operator=(const AddLockSingleInstance &signal);

private:
    static AddLockSingleInstance *m_SingleInstance;
    static std::mutex m_Mutex;
};

AddLockSingleInstance *AddLockSingleInstance::m_SingleInstance = nullptr;
std::mutex AddLockSingleInstance::m_Mutex;

AddLockSingleInstance *&AddLockSingleInstance::GetInstance()
{
    // 使用了两个if语句的加锁技术称为双检锁
    // 只有判断指针为空的时候才加锁，避免每次调用GetInstance的方法都加锁，减少锁的开销
    if (m_SingleInstance == nullptr) {
        std::unique_lock<std::mutex> lock(m_Mutex); // 加锁
        if (m_SingleInstance == nullptr) {
            m_SingleInstance = new (std::nothrow) AddLockSingleInstance;
        }
    }

    return m_SingleInstance;
}

void AddLockSingleInstance::deleteInstance()
{
    std::unique_lock<std::mutex> lock(m_Mutex); // 加锁
    if (m_SingleInstance) {
        delete m_SingleInstance;
        m_SingleInstance = nullptr;
    }
}

void AddLockSingleInstance::Address()
{
    std::cout << "实例内存地址:" << this << std::endl;
}

AddLockSingleInstance::AddLockSingleInstance()
{
    std::cout << "执行构造函数" << std::endl;
}

AddLockSingleInstance::~AddLockSingleInstance()
{
    std::cout << "执行析构函数" << std::endl;
}
/************************** 加锁线程安全的懒汉式单例模式 **************************/

/************************** 内部静态变量线程安全的懒汉式单例模式 **************************/
class StaticSingleInstance
{
public:
    static StaticSingleInstance *GetInstance();
    void Address();

private:
    StaticSingleInstance();
    ~StaticSingleInstance();
    StaticSingleInstance(const StaticSingleInstance &signal);
    const StaticSingleInstance &operator=(const StaticSingleInstance &signal);
};

StaticSingleInstance *StaticSingleInstance::GetInstance()
{
    // 局部静态特性的方式实现单实例
    static StaticSingleInstance signal;
    return &signal;
}

void StaticSingleInstance::Address()
{
	std::cout << "实例内存地址:" << this << std::endl;
}

StaticSingleInstance::StaticSingleInstance()
{
    std::cout << "执行构造函数" << std::endl;
}

StaticSingleInstance::~StaticSingleInstance()
{
    std::cout << "执行析构函数" << std::endl;
}
/************************** 内部静态变量线程安全的懒汉式单例模式 **************************/

/************************** 线程安全的饿汉式单例模式 **************************/
class InitSingleInstance
{
public:
    static InitSingleInstance *GetInstance();
    static void deleteInstance();
    void Address();

private:
    // 将其构造和析构成为私有的, 禁止外部构造和析构
    InitSingleInstance();
    ~InitSingleInstance();

    // 将其拷贝构造和赋值构造成为私有函数, 禁止外部拷贝和赋值
    InitSingleInstance(const InitSingleInstance &signal);
    const InitSingleInstance &operator=(const InitSingleInstance &signal);

private:
    static InitSingleInstance *g_pSingleton;
};

// 代码一运行就初始化创建实例，本身就线程安全
InitSingleInstance* InitSingleInstance::g_pSingleton = new (std::nothrow) InitSingleInstance;

InitSingleInstance* InitSingleInstance::GetInstance()
{
    return g_pSingleton;
}

void InitSingleInstance::deleteInstance()
{
    if (g_pSingleton) {
        delete g_pSingleton;
        g_pSingleton = NULL;
    }
}

void InitSingleInstance::Address()
{
    std::cout << "实例内存地址:" << this << std::endl;
}

InitSingleInstance::InitSingleInstance()
{
    std::cout << "执行构造函数" << std::endl;
}

InitSingleInstance::~InitSingleInstance()
{
    std::cout << "执行析构函数" << std::endl;
}
/************************** 线程安全的饿汉式单例模式 **************************/
