!!NOTE!!
===
I already merged cocos2dx-better code to cocos2d-x v2, here is my new repo: [cocos2dx-classical](https://github.com/stubma/cocos2dx-classical). So, cocos2dx-better is end.

我已开始将cocos2dx-better的代码合并到了cocos2d-x v2中, 新的项目叫[cocos2dx-classical](https://github.com/stubma/cocos2dx-classical), cocos2dx-better将不再维护.

Better Cocos2d-x
===============
As its name suggests, this
extension will make cocos2d-x better in many aspect, such as better UI controls, better utilities, better compatibility, and more. 

Cocos2d-x is a good engine, but:
* it lacked some UI controls, or just not good enough
* it lacked some features and third-party tool support
* it has bugs and fixing comes too slowly, the worse thing is they are super slow responsive to your pull requests. WTF. 

Furthermore, I don't want to modify its code because it may introduce extra work to sync code. Here is my solution: a well-designed extension, without touching cocos2d-x code.

Features
===========
I don't want to list all features because it is too many. The best way to understand what it can do is running its test project. The test project supports ONLY iOS and Android. I don't use Windows a lot so there are some basic features missing for Windows. If you are a Windows guy, do it yourself and GOOD LUCK.

However, even the test project doesn't show all its capabilities. I will add more tests if necessary.

How to use
===========
cocos2dx-better project refers cocos2d-x project. You must clone cocos2d-x repository to a folder named "cocos2d-x", 
and then clone cocos2dx-better in the same parent folder. Currently it only supports cocos2d-x v2 branch and I don't have a clear plan to migrate to v3.

NOTE: recently they renamed master branch to v2 branch, and third-party libraries are not inclued in cocos2d-x git. You must execute download-deps.py to finish the setup.

<b>For iOS:</b>
* drag cocos2dx-better project into your project
* you need set up proper include path, here is mine:

```
$(SDKROOT)/usr/include/libxml2/
$(SRCROOT)/../../cocos2d-x/cocos2dx/include
$(SRCROOT)/../../cocos2d-x/cocos2dx
$(SRCROOT)/../../cocos2d-x/cocos2dx/platform/ios
$(SRCROOT)/../../cocos2d-x/cocos2dx/kazmath/include
$(SRCROOT)/../../cocos2d-x/extensions (set it recursive)
$(SRCROOT)/../../cocos2dx-better/cocos2dx-better/include 
$(SRCROOT)/../../cocos2d-x/CocosDenshion/include
```

* drag libcocos2dx-better.a (in Products folder of cocos2dx-better.xcodeproj) to your target framework list
* add other necessary frameworks to your project, view test project for the detail list
* if you encounter objc runtime error, change "Compile Source As" to "Objective-C++". If you have pure c library, create a new target for it

<b>For Android:</b>
* import cocos2dx-better module in your Android.mk
* you must set a proper NDK_MODULE_PATH environment, below is my setting:

```
export C2DX_ROOT=$HOME/Projects/cocos2d-x
export NDK_MODULE_PATH=$C2DX_ROOT:${C2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt:$HOME/Projects/cocos2dx-better
```

* cocos2dx-better has some java code, you must link it in your project. If you use ant to build, you can set source.dir property in local.properties, below is my setting.

```
source.dir=src;${user.home}/../../${c2dx.root}/cocos2dx/platform/android/java/src;${user.home}/../../${c2dx.root}/../cocos2dx-better/cocos2dx-better/java
```

* in Android.mk, you must link cocos2dx-better library with whole archive option, something like below:

```
... more
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx-better
include $(BUILD_SHARED_LIBRARY)
$(call import-module,cocos2dx-better)
```

You need only to import cocos2dx-better module because cocos2dx-better module already imports cocos2d-x module.

If not clear, refer to demo code.

How to use CCImagePicker
==========================
There is a CCImagePicker in cocos2dx-better, it can take image from camera or album and return it in any size. It is very handy, but you need more setup before using it. In iOS, just use it. In Android, don't forget to config something:
* register activities in AndroidManifest.xml, below is an example, you can change some attribute if you like, such as screenOrientation.

```
<activity
    android:name="org.cocos2dx.lib.cropimage.CropImage"
    android:configChanges="keyboardHidden|orientation"
    android:screenOrientation="portrait"
    android:theme="@android:style/Theme.NoTitleBar.Fullscreen" >
</activity>
<activity
    android:name="org.cocos2dx.lib.ImagePickerActivity"
    android:configChanges="keyboardHidden|orientation"
    android:screenOrientation="portrait"
    android:theme="@android:style/Theme.NoTitleBar.Fullscreen" >
</activity>
```

* add necessary permission

```
<uses-permission android:name="android.permission.CAMERA" />
```

* add feature declaration

```
<uses-feature
    android:name="android.hardware.camera"
    android:required="false" />
<uses-feature
    android:name="android.hardware.camera.front"
    android:required="false" />
```

And that is it! No more settings needed. Easy to use, powerful, view its demo for fun!

Optional
==================
Some optional code or tools is in cocos2dx-better/optional folder. Optional code is not compiled into cocos2dx-better library because they may depend on other components. As regards optional tools, they are my little trickes to handle some weird requirements. However, maybe you have such requirements, who knows.

* B2DebugDraw: it is box2d debug drawing layer, it requires box2d
* File templates: it is file templates for Xcode. If you like that, you can copy Templates folder to ~/Library/Developer/Xcode
* AtlasExtractor: a tool can parse texture atlas and output every single image. Use "-h" to show how to use. The project can be directly run and result can be found in test/output folder
* exportExcel: it is a tool which can export Excel file into JSON format and C++ class. You can use Ant to build the exportExcel.jar file and directly run it.
* jsoncpp: it is a modified version of jsoncpp, better support in data format conversion. It is also referenced by classes generated by exportExcel.
* StoryDesigner: it is a designer tool for screenplay, the script language is Lua. However, the story is mainly described by a set of functions defined in CCStoryCommandSet.h. Here is a test.lua in test folder for your reference.
* pngc: it is a png compress tool which depends on pngquant, written by python. Just execute ```python compress.py``` to see usage 
