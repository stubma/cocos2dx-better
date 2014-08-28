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
#ifndef __ccShaders__
#define __ccShaders__

#include "cocos2d.h"
#include "ccMoreTypes.h"
#include "kazmath/kazmath.h"

using namespace std;

NS_CC_BEGIN

// shader keys
#define kCCShader_flash "kCCShader_flash"
#define kCCShader_blur "kCCShader_blur"
#define kCCShader_laser "kCCShader_laser"
#define kCCShader_lighting "kCCShader_lighting"
#define kCCShader_matrix "kCCShader_matrix"
#define kCCShader_shine "kCCShader_shine"

/// a custom shader management helper
class CC_DLL CCShaders {
private:
	/// load one custom shader by key
	static void loadCustomShader(const string& key);
	
public:
	/// program for key
	static CCGLProgram* programForKey(const string& key);
	
	/// flash
	static void setFlash(float r, float g, float b, float t);
	
	/// blur
	static void setBlur(CCSize nodeSize, CCSize blurSize, ccColor4F blurSubtract = cc4fTRANSPARENT);
    
    /// lighting, view ccShader_lighting_frag.h for detail algorithm
    static void setLighting(ccColor4B mul, ccColor3B add);
    
    /// color matrix
    static void setColorMatrix(const kmMat4& mat4);
    static void setGray();
    
    /**
     * Set uniform for shine shader.
     *
     * @param width node width
     * @param lb shining pattern left bottom position
     * @param rt shining pattern right top position
     * @param color1 gradient color 1
     * @param color2 gradient color 2
     * @param color3 gradient color 3
     * @param gradientPositions the gradient point, it is relative and can be between [0, 1]. First value
     *      indicating the start of color 1, second value is indicating the start of color 2, and so on. Color between
     *      two values will be interpolated.
     * @param time time, from 0 to 1
     */
    static void setShine(float width, CCPoint lb, CCPoint rt, ccColor4B color1, ccColor4B color2, ccColor4B color3, ccVertex3F gradientPositions, float time);
};

NS_CC_END

#endif // __ccShaders__