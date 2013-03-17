#ifndef __CCDrawingPrimitivesEx_h__
#define __CCDrawingPrimitivesEx_h__

#include "cocos2d.h"

NS_CC_BEGIN

void ccDrawColor4BEx( GLubyte r, GLubyte g, GLubyte b, GLubyte a );
void ccDrawSolidCircle( const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY);

NS_CC_END

#endif // __CCDrawingPrimitivesEx_h__
