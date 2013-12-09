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
#ifndef __VolatileTexture_richlabel__
#define __VolatileTexture_richlabel__

#include "cocos2d.h"
#include "ccMoreTypes.h"

using namespace std;

NS_CC_BEGIN

#ifdef CC_ENABLE_CACHE_TEXTURE_DATA

class CCTexture2D_richlabel;

/**
 * a info cache for CCRichLabelTTF
 */
class VolatileTexture_richlabel {
private:
    typedef enum {
        kInvalid = 0,
        kRichString,
    } ccCachedImageType;
    
private:
    // find VolatileTexture by CCTexture2D*
    // if not found, create a new one
    static VolatileTexture_richlabel* findVolotileTexture(CCTexture2D_richlabel* tt);

public:
    VolatileTexture_richlabel(CCTexture2D_richlabel *t);
    virtual ~VolatileTexture_richlabel();
    
    static list<VolatileTexture_richlabel*> s_textures;
    static bool s_isReloading;
    
protected:
    /// font definition for rich label
    ccRichFontDefinition m_fontDef;
    
    /// texture
    CCTexture2D_richlabel* m_texture;
    
    /// type
    ccCachedImageType m_cachedImageType;
    
    /// text
    string m_strText;
    
public:
    static void addRichStringTexture(CCTexture2D_richlabel* tex, const char* text, const ccRichFontDefinition& fontDef);
    static void reloadAllTextures();
};

#endif // CC_ENABLE_CACHE_TEXTURE_DATA

NS_CC_END

#endif /* defined(__VolatileTexture_richlabel__) */
