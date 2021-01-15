#pragma once
#include "pch.h"

#ifdef MYDLL_EXPORTS
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif

class HijackUtils {
public:
  virtual std::wstring GetWorkPath() = 0;
  virtual DWORD_PTR Scan(const char *string) = 0;
  virtual VOID Log(std::wstring string) = 0;
};

class HijackUtilsDLL : public HijackUtils {
public:
  HijackUtilsDLL(void);

private:
  VOID SetLogger();
  VOID GetPath();
  signature_scanner *sig;
  std::wofstream ofs;

public:
  std::wstring WorkPath;
  std::wstring GetWorkPath();
  DWORD_PTR Scan(const char *string);
  VOID Log(std::wstring string);
};

HijackUtils *CreateInstance();
