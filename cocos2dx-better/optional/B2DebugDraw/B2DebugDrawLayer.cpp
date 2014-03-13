/****************************************************************************
 * B2DebugDrawLayer.cpp
 *
 * Created by Stefan Nguyen on Oct 8, 2012.
 *
 * Copyright Vinova Pte. Ltd. All rights reserved.
 ****************************************************************************/

#include "B2DebugDrawLayer.h"

using namespace cocos2d;

B2DebugDrawLayer* B2DebugDrawLayer::create(b2World* pB2World, float pPtmRatio)
{
  B2DebugDrawLayer *pRet = new B2DebugDrawLayer(pB2World, pPtmRatio);
  if (pRet && pRet->init())
  {
    pRet->autorelease();
    return pRet;
  }
  else
  {
    delete pRet;
    pRet = NULL;
    return NULL;
  }
}
B2DebugDrawLayer::B2DebugDrawLayer(b2World* pB2World, float pPtmRatio)
: mB2World(pB2World), mPtmRatio(pPtmRatio)
{
}

bool B2DebugDrawLayer::init()
{
  //////////////////////////////
  // 1. super init first

  if(!CCLayer::init())
  {
    return false;
  }
  
  mB2DebugDraw = new GLESDebugDraw( mPtmRatio );
  mB2World->SetDebugDraw(mB2DebugDraw);
  
  uint32 flags = 0;
  flags += b2Draw::e_shapeBit;
  flags += b2Draw::e_jointBit;
//  flags += b2Draw::e_aabbBit;
  flags += b2Draw::e_pairBit;
  flags += b2Draw::e_centerOfMassBit;
  mB2DebugDraw->SetFlags(flags);
  
  return true;
}


void B2DebugDrawLayer::draw()
{
  ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
  
  kmGLPushMatrix();
  
  mB2World->DrawDebugData();
  
  kmGLPopMatrix();
}
