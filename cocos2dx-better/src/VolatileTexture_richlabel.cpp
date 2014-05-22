/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
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
#include "VolatileTexture_richlabel.h"
#include "CCTexture2D_richlabel.h"

NS_CC_BEGIN

#ifdef CC_ENABLE_CACHE_TEXTURE_DATA

list<VolatileTexture_richlabel*> VolatileTexture_richlabel::s_textures;
bool VolatileTexture_richlabel::s_isReloading = false;

VolatileTexture_richlabel::VolatileTexture_richlabel(CCTexture2D_richlabel* t) :
m_texture(t),
m_cachedImageType(kInvalid) {
    s_textures.push_back(this);
}

VolatileTexture_richlabel::~VolatileTexture_richlabel() {
    s_textures.remove(this);
}

void VolatileTexture_richlabel::addRichStringTexture(CCTexture2D_richlabel* tex, const char* text, const ccRichFontDefinition& fontDef) {
    if(s_isReloading) {
        return;
    }
    
    VolatileTexture_richlabel* vt = findVolotileTexture(tex);
    vt->m_cachedImageType = kRichString;
    vt->m_strText = text;
    vt->m_fontDef = fontDef;
}

VolatileTexture_richlabel* VolatileTexture_richlabel::findVolotileTexture(CCTexture2D_richlabel *tt) {
    VolatileTexture_richlabel *vt = 0;
    std::list<VolatileTexture_richlabel *>::iterator i = s_textures.begin();
    while (i != s_textures.end()) {
        VolatileTexture_richlabel *v = *i++;
        if (v->m_texture == tt) {
            vt = v;
            break;
        }
    }
    
    if(!vt) {
        vt = new VolatileTexture_richlabel(tt);
    }
    
    return vt;
}

void VolatileTexture_richlabel::reloadAllTextures() {
    s_isReloading = true;
    
    list<VolatileTexture_richlabel*>::iterator iter = s_textures.begin();
    while(iter != s_textures.end()) {
        VolatileTexture_richlabel* vt = *iter++;
        
        switch(vt->m_cachedImageType) {
            case kRichString:
                vt->m_texture->initWithRichString(vt->m_strText.c_str(), &vt->m_fontDef);
                break;
            default:
                break;
        }
    }
    
    s_isReloading = false;
}

#endif // CC_ENABLE_CACHE_TEXTURE_DATA

NS_CC_END
