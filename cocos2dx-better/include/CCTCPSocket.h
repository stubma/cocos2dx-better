#ifndef __CCTCPSocket_h__
#define __CCTCPSocket_h__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "CCByteBuffer.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

NS_CC_BEGIN

#ifndef CHECKF
#define CHECKF(x) \
do \
{ \
if (!(x)) { \
log_msg("CHECKF", #x, __FILE__, __LINE__); \
return 0; \
} \
} while (0)
#endif

#define _MAX_MSGSIZE 16 * 1024        // 暂定一个消息最大为16k
#define BLOCKSECONDS    30            // INIT函数阻塞时间
#define INBUFSIZE    (64*1024)        //?    具体尺寸根据剖面报告调整  接收数据的缓存
#define OUTBUFSIZE    (8*1024)        //? 具体尺寸根据剖面报告调整。 发送数据的缓存，当不超过8K时，FLUSH只需要SEND一次

class CC_DLL CCTCPSocket
{
public:
    CCTCPSocket(void);
    bool    Create(const char* pszServerIP, int nServerPort, int tagid, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);
    bool    SendMsg(void* pBuf, int nSize);
    bool    ReceiveMsg(void* pBuf, int& nSize);
    bool    Flush(void);
    bool    Check(void);
    void    Destroy(void);
    SOCKET    GetSocket(void) const { return m_sockClient; }
    
    int        getTagID(){ return m_tag; }
private:
    bool    recvFromSock(void);        // 从网络中读取尽可能多的数据
    bool    hasError();            // 是否发生错误，注意，异步模式未完成非错误
    void    closeSocket();
	
    SOCKET    m_sockClient;
	
    // 发送数据缓冲
    char    m_bufOutput[OUTBUFSIZE];    //? 可优化为指针数组
    int        m_nOutbufLen;
	
    // 环形缓冲区
    char    m_bufInput[INBUFSIZE];
    int        m_nInbufLen;
    int        m_nInbufStart;                // INBUF使用循环式队列，该变量为队列起点，0 - (SIZE-1)
    int        m_tag;
};

typedef bool (*ProAllFunc)(int, int, CCByteBuffer&);    // 接收所有协议，自行处理，@socket标识,@协议头，@数据包,返回是否分发
typedef void (*ProFunc)(int, CCByteBuffer&);    // 接收单个协议，@socket标识,@数据包
typedef void (*sckFunc)(int);    // 连接成功/断开事件

#define SCT_CALLBACK_1(func, _Object) std::bind(&func,_Object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define SCT_CALLBACK_2(func, _Object) std::bind(&func,_Object, std::placeholders::_1, std::placeholders::_2)
#define SCT_CALLBACK_3(func, _Object) std::bind(&func,_Object, std::placeholders::_1)
// 创建SOCKET管理器
#define CREATE_CCTCPSocketMGR(pNode)    pNode->addChild(new CCTCPSocketManager(), 0)

class CC_DLL CCTCPSocketManager : public CCNode
{
public:
    CCTCPSocketManager()
    {
        assert(!mSingleton);
        this->mSingleton = this;
        // 开启update
        scheduleUpdate();
    };
    ~CCTCPSocketManager(){};
    // 创建socket并添加到管理器
    CCTCPSocket *createSocket(const char* pszServerIP,    // IP地址
                            int nServerPort,            // 端口
                            int _tag,                    // 标识ID
                            int nBlockSec = BLOCKSECONDS, // 阻塞时间ms
                            bool bKeepAlive = false);
    // 注册协议包
    void    register_process(const uint16 &entry, ProFunc callback);
    // 注册接收所有协议
    void    register_all_process(ProAllFunc callback){ _pProcess = callback; }
    // 注册socket连接成功事件
    void    register_connect(sckFunc callback){ _pOnConnect = callback; }
    // 注册socket断线事件
    void    register_disconnect(sckFunc callback){ _OnDisconnect = callback; }
	
    // 单独添加socket到管理器
    bool    addSocket(CCTCPSocket *pSocket);
    // 删除socket
    bool    removeSocket(int _tag);
    // 断开socket
    void    disconnect(int _tag);
    // 获取socket
    CCTCPSocket    *GetSocket(int _tag);
    // 发送消息
    bool    SendPacket(int _tag, CCByteBuffer *packet);
	
    virtual void    update(float delta);
	
    static CCTCPSocketManager &getSingleton(){ assert(mSingleton); return *mSingleton;}
	
private:
    ProAllFunc _pProcess;
    sckFunc _pOnConnect;
    sckFunc _OnDisconnect;
    std::list<CCTCPSocket*> m_lstSocket;
    std::map<uint16, ProFunc> _mapProcess;
    static CCTCPSocketManager * mSingleton;
};

#define sSocketMgr    CCTCPSocketManager::getSingleton()

NS_CC_END

#endif //__CCTCPSocket_h__