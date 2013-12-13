#ifndef WGSERVER_CCWorldPacket_H
#define WGSERVER_CCWorldPacket_H

#include "cocos2d.h"
#include "CCByteBuffer.h"

NS_CC_BEGIN

class CCWorldPacket : public CCByteBuffer
{
public:
    __inline CCWorldPacket() : CCByteBuffer(), m_opcode(0) { }
    __inline CCWorldPacket(uint16 opcode, size_t res) : CCByteBuffer(res), m_opcode(opcode) {}
    __inline CCWorldPacket(size_t res) : CCByteBuffer(res), m_opcode(0) { }
    __inline CCWorldPacket(const CCWorldPacket &packet) : CCByteBuffer(packet), m_opcode(packet.m_opcode) {}
	
    //! Clear packet and set opcode all in one mighty blow
    __inline void Initialize(uint16 opcode )
    {
        clear();
        m_opcode = opcode;
    }
	
    __inline uint16 GetOpcode() const { return m_opcode; }
    __inline void SetOpcode(const uint16 & opcode) { m_opcode = opcode; }
    __inline void SetLength(const uint16 & len)    {
        uint16 * plen = (uint16 * ) &(contents()[2]);
        *plen = len;
    }
	
    __inline std::string getString()
    {
        //std::string buff =
        return (const char*)contents();
    }
    
	template<typename T>
	void SetOffset(const uint16 & offset, const T value ) {
        T * pval = (T *) &(contents()[offset]);
        *pval = value;
    }
	
public:
    /**
     * @创建时间 2011-08-31
     * @创建人 李志勇
     * @函数作用 向数据包追加字符串
     * @参数
     *     @packet 数据封包指针
     *  @str 追加的字符串
     */
    template<typename T> void AppendPacketString(std::string str)
    {
        T ilen = (T)str.size();
        *this << ilen;
        if (ilen > 0)
            Write((const uint8 *) str.c_str(), ilen);
    }
    /**
     * @创建时间 2011-08-31
     * @创建人 李志勇
     * @函数作用 获取字符串
     * @参数
     *     @packet 数据封包
     * @返回 是否成功
     */
    template<typename T> bool GetPacketString(std::string &str)
    {
        T ilen;
        *this >> ilen;
        if (ilen == 0)
            return true;
        uint8 buf[ilen+1];
        uint16 plen = size();
        if (plen < ilen)
            return false;
        read(buf, ilen);
        buf[ilen] = 0;
        str = (char*)buf;
        return true;
    }
	
	
protected:
    uint16 m_opcode;
};

NS_CC_END

#endif