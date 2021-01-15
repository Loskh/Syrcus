// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Shlwapi.h>
#include <clocale>
#include <fstream>
#include <io.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include "SignatureScanner.h"
#include "dllmain.h"

#pragma comment(lib, "shlwapi")

HMODULE hMod;
std::wstring HijackUtilsDLL::GetWorkPath() { return HijackUtilsDLL::WorkPath; };

HijackUtilsDLL::HijackUtilsDLL() {
  GetPath();
  SetLogger();
  sig = new signature_scanner();
  return;
};

DWORD_PTR HijackUtilsDLL::Scan(const char *string) {
  return sig->search(string);
};

VOID HijackUtilsDLL::SetLogger() {
  ofs.imbue(std::locale(std::locale(), "", LC_CTYPE));
  auto logPath = HijackUtilsDLL::WorkPath + L"\\Syrcus.log";
  ofs.open(logPath, std::wofstream::app);
};

VOID HijackUtilsDLL::Log(std::wstring log) {
  if (!ofs.is_open())
    return;
  time_t t = time(0);
  char tmp[64];
  strftime(tmp, sizeof(tmp), "[%Y-%m-%d %X]", localtime(&t));
  ofs << tmp << " " << log << " " << std::endl;
  ofs.flush();
};

VOID HijackUtilsDLL::GetPath() {
  TCHAR tszDllPath[MAX_PATH] = {0};
  GetModuleFileName(hMod, tszDllPath, MAX_PATH);
  PathRemoveFileSpec(tszDllPath);
  WorkPath = std::wstring(tszDllPath);
};

HijackUtils *CreateInstance() { return new HijackUtilsDLL(); };

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    hMod = hModule;
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
};
