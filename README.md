# Syrcus

**A DLL hijack Framework for FFXIV**

Syrcus是一个DLL劫持框架，主要用于FFXIV。使用这个框架，可以实现在游戏启动时主动注入DLL，运行CMD等。

### 目录结构

+ ffxiv_dx11.exe
+ SoundCoreBridge.dll:编译生成的DLL
+ SoundCoreBridge.Real.dll:原本的SoundCoreBridge.dll
+ Plugins\
    * commands.txt:程序启动时执行的CMD，有的命令需要延时加载，如Dalamud.Inject
    * DLLs
    * .....