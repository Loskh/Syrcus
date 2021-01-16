// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include "pch.h"
#include "dllmain.h"

Plugin::Plugin() { return; };

void Plugin::StarlightBreaker() {
  DWORD_PTR address = Interface->Scan("74??488BD3E8????????488BC3");
  // out << std ::hex << address << std ::endl;
  char *p;
  p = (char *)address;
  *p = 0xEB;
  Log(L"屏蔽词反和谐已加载");
}

void Plugin::Init(HijackUtils *Util) {
  Name = L"TestPlugin";
  Interface = Util;
  Log(L"Hello World!");
  //StarlightBreaker();
}
std::wstring Plugin::GetName() { return Name; }
void Plugin::Log(std::wstring msg) {
  Interface->Log(L" [" + Name + L"] " + msg);
};

Plugin *CreateInstance() {
  Plugin *p = new Plugin();
  return p;
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
};
