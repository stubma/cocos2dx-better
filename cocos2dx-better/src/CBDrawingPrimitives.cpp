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
#include "CBDrawingPrimitives.h"
#include "ccMoreTypes.h"

NS_CC_BEGIN

static bool s_bInitialized = false;
static CCGLProgram* s_pShader = NULL;
static int s_nColorLocation = -1;
static ccColor4F s_tColor = {1.0f,1.0f,1.0f,1.0f};

static void lazy_init( void )
{
    if( ! s_bInitialized ) {
		
        //
        // Position and 1 color passed as a uniform (to simulate glColor4ub )
        //
        s_pShader = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_Position_uColor);
		
        s_nColorLocation = glGetUniformLocation( s_pShader->getProgram(), "u_color");
		CHECK_GL_ERROR_DEBUG();
		
        s_bInitialized = true;
    }
}

void ccDrawColor4BEx( GLubyte r, GLubyte g, GLubyte b, GLubyte a )
{
    s_tColor.r = r/255.0f;
    s_tColor.g = g/255.0f;
    s_tColor.b = b/255.0f;
    s_tColor.a = a/255.0f;
}

void ccDrawSolidCircle( const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY)
{
    lazy_init();
	
    int additionalSegment = 1;
    if (drawLineToCenter)
        additionalSegment++;
	
    const float coef = 2.0f * (float)M_PI/segments;
	
    GLfloat *vertices = (GLfloat*)calloc( sizeof(GLfloat)*2*(segments+2), 1);
    if( ! vertices )
        return;
	
    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads + angle) * scaleX + center.x;
        GLfloat k = radius * sinf(rads + angle) * scaleY + center.y;
		
        vertices[i*2] = j;
        vertices[i*2+1] = k;
    }
    vertices[(segments+1)*2] = center.x;
    vertices[(segments+1)*2+1] = center.y;
	
    s_pShader->use();
    s_pShader->setUniformsForBuiltins();
    s_pShader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);
	
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
	
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) segments+additionalSegment);
	
    free( vertices );
	
    CC_INCREMENT_GL_DRAWS(1);
}

void ccDrawSolidRoundRect(CCPoint origin, CCPoint destination, float cornerRadius, ccColor4B color) {
    // ensure origin is left bottom
    CCPoint bl = origin;
    CCPoint tr = destination;
    if(bl.x > tr.x) {
        bl.x = MIN(origin.x, destination.x);
        tr.x = MAX(origin.x, destination.x);
    }
    if(bl.y > tr.y) {
        bl.y = MIN(origin.y, destination.y);
        tr.y = MAX(origin.y, destination.y);
    }
    
    // four center of corners
    CCPoint tlCenter = ccp(bl.x + cornerRadius, tr.y - cornerRadius);
    CCPoint trCenter = ccp(tr.x - cornerRadius, tr.y - cornerRadius);
    CCPoint blCenter = ccp(bl.x + cornerRadius, bl.y + cornerRadius);
    CCPoint brCenter = ccp(tr.x - cornerRadius, bl.y + cornerRadius);
    
    // populate vertices
    CCPoint vertices[] = {
        // left edge
        ccp(bl.x, bl.y + cornerRadius),
        ccp(bl.x, tr.y - cornerRadius),
        
        // top left corner
        ccpAdd(tlCenter, ccpDegree(170) * cornerRadius),
        ccpAdd(tlCenter, ccpDegree(160) * cornerRadius),
        ccpAdd(tlCenter, ccpDegree(150) * cornerRadius),
        ccpAdd(tlCenter, ccpDegree(140) * cornerRadius),
        ccpAdd(tlCenter, ccpDegree(130) * cornerRadius),
        ccpAdd(tlCenter, ccpDegree(120) * cornerRadius),
        ccpAdd(tlCenter, ccpDegree(110) * cornerRadius),
        ccpAdd(tlCenter, ccpDegree(100) * cornerRadius),
        
        // top edge
        ccp(bl.x + cornerRadius, tr.y),
        ccp(tr.x - cornerRadius, tr.y),
        
        // right top corner
        ccpAdd(trCenter, ccpDegree(80) * cornerRadius),
        ccpAdd(trCenter, ccpDegree(70) * cornerRadius),
        ccpAdd(trCenter, ccpDegree(60) * cornerRadius),
        ccpAdd(trCenter, ccpDegree(50) * cornerRadius),
        ccpAdd(trCenter, ccpDegree(40) * cornerRadius),
        ccpAdd(trCenter, ccpDegree(30) * cornerRadius),
        ccpAdd(trCenter, ccpDegree(20) * cornerRadius),
        ccpAdd(trCenter, ccpDegree(10) * cornerRadius),
        
        // right edge
        ccp(tr.x, tr.y - cornerRadius),
        ccp(tr.x, bl.y + cornerRadius),
        
        // bottom right corner
        ccpAdd(brCenter, ccpDegree(-10) * cornerRadius),
        ccpAdd(brCenter, ccpDegree(-20) * cornerRadius),
        ccpAdd(brCenter, ccpDegree(-30) * cornerRadius),
        ccpAdd(brCenter, ccpDegree(-40) * cornerRadius),
        ccpAdd(brCenter, ccpDegree(-50) * cornerRadius),
        ccpAdd(brCenter, ccpDegree(-60) * cornerRadius),
        ccpAdd(brCenter, ccpDegree(-70) * cornerRadius),
        ccpAdd(brCenter, ccpDegree(-80) * cornerRadius),
        
        // bottom edge
        ccp(tr.x - cornerRadius, bl.y),
        ccp(bl.x + cornerRadius, bl.y),
        
        // bottom left corner
        ccpAdd(blCenter, ccpDegree(-100) * cornerRadius),
        ccpAdd(blCenter, ccpDegree(-110) * cornerRadius),
        ccpAdd(blCenter, ccpDegree(-120) * cornerRadius),
        ccpAdd(blCenter, ccpDegree(-130) * cornerRadius),
        ccpAdd(blCenter, ccpDegree(-140) * cornerRadius),
        ccpAdd(blCenter, ccpDegree(-150) * cornerRadius),
        ccpAdd(blCenter, ccpDegree(-160) * cornerRadius),
        ccpAdd(blCenter, ccpDegree(-170) * cornerRadius)
    };
    
    ccDrawSolidPoly(vertices, 40, ccc4FFromccc4B(color));
}

NS_CC_END