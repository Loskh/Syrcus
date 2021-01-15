#pragma once
#include "pch.h"

#ifdef MYDLL_EXPORTS
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif

class Plugin {
public:
  Plugin();
  virtual void Init(HijackUtils *Util);
  virtual std::wstring GetName();

public:
  std::wstring Name;

private:
  HijackUtils *Interface;
  void Log(std::wstring msg);
  void StarlightBreaker();
};


MYDLL_API Plugin * CreateInstance();