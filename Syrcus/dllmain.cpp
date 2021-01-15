// dllmain.cpp : 定义 DLL 应用程序的入口点。


#include <atlstr.h>
#include <cassert>
#include <fstream>
#include <io.h>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "dllmain.h"
#include "dllhijack.h"
#include "shlwapi.h"

#pragma comment(lib, "shlwapi")

std::wofstream ofs;

std::wstring WorkPath;
std::wstring GetWorkPath() { return WorkPath; };
HijackUtils *Utils;
std::list<Plugin *> PluginList;

VOID Log(std::wstring msg) { Utils->Log(msg); };

VOID GetPath(HMODULE hMod) {
  TCHAR tszDllPath[MAX_PATH] = {0};
  GetModuleFileName(hMod, tszDllPath, MAX_PATH);
  PathRemoveFileSpec(tszDllPath);
  WorkPath = std::wstring(tszDllPath);
}

VOID LoadUtils() {
  HINSTANCE hUtil;
  HijackUtilsInstance instance;
  hUtil = LoadLibrary(L"Hijack.Utils.dll");
  if (hUtil) {
    instance = (HijackUtilsInstance)GetProcAddress(hUtil, "CreateInstance");
    Utils = instance();
  }
}

VOID DllHijack() {
  auto tszDllPath = WorkPath + L"\\SoundCoreBridge.Real.dll";
  SuperDllHijack(L"SoundCoreBridge.dll", (LPWSTR)tszDllPath.c_str());
  Log(L"=========SoundCoreBridge.dll Hijacked=========");
};

VOID getAllFiles(std::wstring path, std::vector<std::wstring> &files,
                 std::wstring fileType) {
  DWORD_PTR hFile = 0;
  struct _wfinddata_t fileinfo;
  std::wstring p;
  hFile =
      _wfindfirst(p.assign(path).append(L"\\*" + fileType).c_str(), &fileinfo);
  if (hFile != -1) {
    do {
      files.push_back(p.assign(path).append(L"\\").append(fileinfo.name));

    } while (_wfindnext(hFile, &fileinfo) == 0);

    _findclose(hFile);
  }
}

VOID LoadPlugin(std::wstring path) {
  HINSTANCE hDLL = LoadLibrary(path.c_str());
  PluginInstance pInst;
  if (hDLL) {
    pInst = (PluginInstance)GetProcAddress(hDLL, "CreateInstance");
    Plugin *logTest = pInst();
    if (logTest) {
      PluginList.push_back(logTest);
      logTest->Init(Utils);
      Log(logTest->GetName() + L"已加载");
    }
  } else {
    Log(path + L"加载失败");
  }
};

VOID LoadPlugins() {
  std::vector<std::wstring> files;
  std::wstring pluginsPath = WorkPath + L"\\Plugins";
  getAllFiles(pluginsPath, files, L".dll");
  // getAllFiles(L"D:\\Games\\爱抚爱抚十四\\game", files, L".dll");
  for (auto file : files) {
    LoadPlugin(file);
  }
};

VOID RunCmdLine(std::wstring cmdLine) {
  STARTUPINFO si = {sizeof(si)};
  PROCESS_INFORMATION pi = {0};
  BOOL bRet = FALSE;
  bRet = CreateProcess(NULL, (LPWSTR)cmdLine.c_str(), NULL, NULL, FALSE,
                       CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
};

VOID LoadRunCommands() {
  std::vector<std::wstring> commands;
  std::wifstream infile;
  auto commandsPath = WorkPath + L"\\Plugins\\commands.txt";
  infile.open(commandsPath);
  assert(infile.is_open());
  std::wstring s;
  while (getline(infile, s)) {
    commands.push_back(s);
    // Log(s);
  }
  infile.close();
  for (auto command : commands) {
    Log(L"Run Command:" + command);
    RunCmdLine(command);
  }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    try {
      std::locale loc = std::locale::global(std::locale(""));
      GetPath(hModule);
      LoadUtils();
      DllHijack();
      LoadRunCommands();
      LoadPlugins();
      Log(L"加载完成");
    } catch (std::wstring ex) {
      if (!ofs.is_open())
        Log(ex);
    }
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    //Log(L"进程已退出");
    break;
  }
  return TRUE;
}
