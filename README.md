Better Cocos2d-x
===============
The cocos2dx-common is renamed to cocos2dx-better!

Purpose
===============
Now it is time to integrate all my extensions into a single one, for easy management. As its name suggests, this
extension will make cocos2d-x better in many aspect, such as better UI controls, better utilities, better compatibility, and
more. 

Cocos2d-x is a good engine, but:
* it lacks some UI controls, or just not good enough
* it lacks some features and third-party tool support
* it has bugs and fixing comes too slowly, probably because they are focus on 3.0 branch.

You want a better cocos2d-x, so am I. Furthermore, I don't want to modify its code because it may introduce extra work to
sync code. So I give you my solution, a well-designed extensions, without touching cocos2d-x code.

Features
===========
I don't want to list all features because it is too many! The best way to understand what it can do is running its test project. The test project supports ONLY iOS and android. I don't use Windows a lot so there are some basic features missing  for Windows. If you are a Windows guy, do it yourself and GOOD LUCK.

However, even the test project doesn't show all capability of it. Sure I will add more test if necessary.

Requirement
===========
cocos2dx-better project refers cocos2d-x project. You must clone cocos2d-x repository to a folder named "cocos2d-x", 
and then clone cocos2dx-better in the same parent folder.

Also, set a proper NDK_MODULE_PATH environment, below is my setting:
```
export C2DX_ROOT=$HOME/Projects/cocos2d-x
export NDK_MODULE_PATH=$C2DX_ROOT:${C2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt:$HOME/Projects/cocos2dx-better
```

How to use
===========
For iOS:
* drag cocos2dx-better project into your project
* you need set up proper include path, here is mine:

```
$(SDKROOT)/usr/include/libxml2/
$(SRCROOT)/../../cocos2d-x/cocos2dx/include
$(SRCROOT)/../../cocos2d-x/cocos2dx
$(SRCROOT)/../../cocos2d-x/cocos2dx/platform/ios
$(SRCROOT)/../../cocos2d-x/cocos2dx/kazmath/include
$(SRCROOT)/../../cocos2d-x/extensions/CocoStudio/Armature 
$(SRCROOT)/../../cocos2d-x/extensions 
$(SRCROOT)/../../cocos2dx-better/cocos2dx-better/include 
$(SRCROOT)/../../cocos2d-x/CocosDenshion/include
```

* drag libcocos2dx-better.a (in Products folder of cocos2dx-better.xcodeproj) to your target framework list
* if you encounter objc runtime error, change "Compile Source As" to "Objective-C++". If you have pure c library, create a new target for it

For Android:
* import cocos2dx-better module in your Android.mk
* cocos2dx-better has some java code, you must link it in your project. If you use ant to build, you can set source.dir properties in local.properties, below is my setting(c2dx_root is a environment variable which point to cocos2d-x root)

```
source.dir=src;${user.home}/../../${c2dx.root}/cocos2dx/platform/android/java/src;${user.home}/../../${c2dx.root}/../cocos2dx-better/cocos2dx-better/java
```

If not clear, refer to demo code.
