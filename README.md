# BuildIncrement
Build Increment Tool For dev ( and use with cocos2dx too but can be without)

This tool is executed before the compilation of your project and increment a header file whish contain a "#define Build x"

you can use the Build variable in your porject after that to show into your soft the current build number

# Normale Use
for that you must call : yourpath1/BuildIncrement.exe yourpath2/Build.h

In visual studio, you can call this in Settings/Build Event/Pre-Build/command

# Use with android ( like in cocos2dx by example)

I have added a system to modify the android manifest file passed in second argument to the command line tool

In the sample Build.h, i define 3 constants :

Build, Major, Minor

only the Build var is incremented.
The major and minor number are user defined.

And i format the two var of the android manifest file :

- android:versionCode = /Build/
- android:versionName = /Major.Minor.Build/

For that you can call at the same place : yourpath1/BuildIncrement.exe yourpath2/Build.h yourpath3/AndroidManifest.xml
