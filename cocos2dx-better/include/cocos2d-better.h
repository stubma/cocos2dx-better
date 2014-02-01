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
#ifndef __cocos2d_better_h__
#define __cocos2d_better_h__

// common
#include "CCMoreMacros.h"
#include "ccMoreTypes.h"
#include "CCPointList.h"
#include "CCUtils.h"
#include "CCMD5.h"
#include "CCScroller.h"
#include "CCScrollView.h"
#include "CCAutoRenderMenuItemSprite.h"
#include "CCMenuItemColor.h"
#include "CCMissile.h"
#include "CCShake.h"
#include "CCDrawingPrimitivesEx.h"
#include "CCAssetInputStream.h"
#include "CCMemoryInputStream.h"
#include "CCResourceLoader.h"
#include "CCResourceLoaderListener.h"
#include "CCGradientSprite.h"
#include "CCTextureAtlasEx.h"
#include "CCTiledSprite.h"
#include "CCTreeFadeIn.h"
#include "CCTreeFadeOut.h"
#include "CCLocalization.h"
#include "CCRichLabelTTF.h"
#include "CCLocale.h"
#include "CCCalendar.h"
#include "CCVelocityTracker.h"
#include "CCToast.h"
#include "CCLayerMultiplexEx.h"
#include "CCGestureRecognizer.h"
#include "CCLongPressGestureRecognizer.h"
#include "CCPinchGestureRecognizer.h"
#include "CCPanGestureRecognizer.h"
#include "CCSwipeGestureRecognizer.h"
#include "CCTapGestureRecognizer.h"
#include "CCRookieGuide.h"
#include "CCSimpleRookieGuide.h"
#include "CCSpriteEx.h"
#include "CCClipIn.h"
#include "CCClipOut.h"
#include "CCProgressHUD.h"
#include "CCCatmullRomSprite.h"
#include "CCSlider.h"
#include "CCJumpByEx.h"
#include "CCJumpToEx.h"
#include "CCLayerClip.h"
#include "CCImagePicker.h"
#include "CCImagePickerCallback.h"

// db
#include "CCDatabase.h"
#include "CCResultSet.h"
#include "CCStatement.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    #include "platform/android/sqlite3.h"
#endif

// json
#include "CCJSONArray.h"
#include "CCJSONObject.h"
#include "CCJSONValue.h"
#include "CCJSONParser.h"

// anim
#include "CCAFCAnimation.h"
#include "CCAFCClip.h"
#include "CCAFCClipMapping.h"
#include "CCAFCConstants.h"
#include "CCAFCFileData.h"
#include "CCAFCFrame.h"
#include "CCAFCSprite.h"
#include "CCArcticConstants.h"
#include "CCArcticFileData.h"
#include "CCArcticLoader.h"
#include "CCArcticManager.h"
#include "CCArcticSprite.h"
#include "CCAuroraConstants.h"
#include "CCAuroraFileData.h"
#include "CCAuroraLoader.h"
#include "CCAuroraManager.h"
#include "CCAuroraSprite.h"
#include "CCMWSprite.h"
#include "CCMWLoader.h"
#include "CCMWFileData.h"
#include "CCMWManager.h"
#include "CCSPX3Constants.h"
#include "CCSPX3Sprite.h"
#include "CCSPX3Manager.h"
#include "CCSPX3FileData.h"
#include "CCSPX3Frame.h"
#include "CCSPX3Action.h"
#include "CCSPX3Loader.h"
#include "CCSPX3TileSet.h"
#include "CCSPXConstants.h"
#include "CCSPXSprite.h"
#include "CCSPXManager.h"
#include "CCSPXFileData.h"
#include "CCSPXFrame.h"
#include "CCSPXAction.h"
#include "CCSPXLoader.h"

// network
#include "CCNetworkCommon.h"
#include "CCByteBuffer.h"
#include "CCTCPSocketListener.h"
#include "CCTCPSocket.h"
#include "CCTCPSocketHub.h"
#include "CCUDPSocket.h"
#include "CCUDPSocketListener.h"
#include "CCUDPSocketHub.h"

#endif // __cocos2d_better_h__
