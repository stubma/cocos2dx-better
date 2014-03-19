/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CBTextureAtlas.h"

//According to some tests GL_TRIANGLE_STRIP is slower, MUCH slower. Probably I'm doing something very wrong

// implementation CBTextureAtlas

NS_CC_BEGIN

CBTextureAtlas::CBTextureAtlas() {
}

CBTextureAtlas::~CBTextureAtlas() {
}

// TextureAtlas - alloc & init

CBTextureAtlas * CBTextureAtlas::create(const char* file, unsigned int capacity)
{
    CBTextureAtlas * pTextureAtlas = new CBTextureAtlas();
    if(pTextureAtlas && pTextureAtlas->initWithFile(file, capacity))
    {
        pTextureAtlas->autorelease();
        return pTextureAtlas;
    }
    CC_SAFE_DELETE(pTextureAtlas);
    return NULL;
}

CBTextureAtlas * CBTextureAtlas::createWithTexture(CCTexture2D *texture, unsigned int capacity)
{
    CBTextureAtlas * pTextureAtlas = new CBTextureAtlas();
    if (pTextureAtlas && pTextureAtlas->initWithTexture(texture, capacity))
    {
        pTextureAtlas->autorelease();
        return pTextureAtlas;
    }
    CC_SAFE_DELETE(pTextureAtlas);
    return NULL;
}

void CBTextureAtlas::drawQuadsEx()
{
    this->drawNumberOfQuadsEx(m_uTotalQuads, 0);
}

void CBTextureAtlas::drawNumberOfQuadsEx(unsigned int n)
{
    this->drawNumberOfQuadsEx(n, 0);
}

void CBTextureAtlas::drawNumberOfQuadsEx(unsigned int n, unsigned int start)
{    
    if (0 == n) 
    {
        return;
    }
    ccGLBindTexture2D(m_pTexture->getName());
	
	#define kQuadSize sizeof(m_pQuads[0].bl)
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);

    // vertices
	long offset = (long)m_pQuads;
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)(offset + diff));

    // colors
	diff = offsetof(ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*)(offset + diff));

    // tex coords
	diff = offsetof(ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*)(offset + diff));

#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6 + m_pIndices));
#else
    glDrawElements(GL_TRIANGLES, (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6 + m_pIndices));
#endif // CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP

    CC_INCREMENT_GL_DRAWS(1);
    CHECK_GL_ERROR_DEBUG();
}


NS_CC_END

