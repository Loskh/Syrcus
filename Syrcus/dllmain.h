#pragma once
#include <winnt.h>


class HijackUtils {
public:
  virtual std::wstring GetWorkPath() = 0;
  virtual DWORD_PTR Scan(const char *string) = 0;
  virtual VOID Log(std::wstring string) = 0;
};
typedef HijackUtils *(*HijackUtilsInstance)();

class Plugin {
public:
  virtual void Init(HijackUtils *Util);
  virtual std::wstring GetName();
};
typedef Plugin *(*PluginInstance)();

class PluginEntity {
public:
  std::wstring Name;
  std::wstring Path;
  HINSTANCE Handle;
  Plugin *Instance;
};




