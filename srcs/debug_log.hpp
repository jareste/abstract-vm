#pragma once

#ifdef DEBUG
  #define LOG(msg) do { std::cout << msg << std::endl; } while(0)
#else
  #define LOG(msg) do {} while(0)
#endif

#ifdef DEBUG
  #define LOG_OP(op, line) \
    std::cout << "[VM] " << op << " (line " << line << ")" << std::endl
#else
  #define LOG_OP(op, line) do {} while(0)
#endif


