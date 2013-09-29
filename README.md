cocos2dx-common
===============

Because I wrote more than one cocos2d-x extension so I found some code can be moved out as a common codebase.

Here is my extensions which refer to this common base:
* https://github.com/stubma/cocos2dx-anim
* https://github.com/stubma/cocos2dx-json
* https://github.com/stubma/cocos2dx-wisound
* https://github.com/stubma/cocos2dx-db

Requirement
===========
cocos2dx-common project refers cocos2d-x project. You must clone cocos2d-x repository to a folder named "cocos2d-x", 
and then clone cocos2dx-common in the same parent folder.

Demo
===========
There is a test project in test folder, it supports iOS and android. 

NOTE
===========
I don't use Windows a lot so there are some basic features not implemented for Windows. If you are a Windows guy, do
it yourself and good luck.

How to use
===========
For iOS, just drag cocos2dx-common project into your project. For Android, just import cocos2dx-common module. 
If not clear, refer to demo code.
