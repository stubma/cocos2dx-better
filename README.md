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

Requirement
===========
cocos2dx-better project refers cocos2d-x project. You must clone cocos2d-x repository to a folder named "cocos2d-x", 
and then clone cocos2dx-better in the same parent folder.

How to use
===========
For iOS, just drag cocos2dx-better project into your project. For Android, just import cocos2dx-better module. 
If not clear, refer to demo code.
