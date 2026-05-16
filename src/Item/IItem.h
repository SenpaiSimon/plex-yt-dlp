#pragma once

#include <string>

class IItem {
public:
  virtual ~IItem() = default;

  virtual void PreProcess() = 0;
  virtual void Process() = 0;
  virtual void PostProcess() = 0;
};