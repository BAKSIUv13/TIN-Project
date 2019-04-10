// Copyright 0 0

#ifndef SERVER_CORE_SOCKET_H_
#define SERVER_CORE_SOCKET_H_

#include <cstdint>

namespace tin {

class SocketTCP4 {
 public:
  enum Status {
    BLANK,
    CREATED,
    BOND,
    LISTENING,
    CONNECTED
  };

  SocketTCP4();
  //  explicit SocketTCP4(uint32_t address, uint16_t port);
  SocketTCP4(const SocketTCP4 &) = delete;
  SocketTCP4(SocketTCP4 &&);
  SocketTCP4 &operator=(SocketTCP4 &&);
  ~SocketTCP4();

  int Create();
  int Bind(uint32_t, uint16_t);
  int BindAny(uint16_t);
  int Listen();
  int Connect();
  int Accept();

  Status GetStatus() {return status_;}
  int GetFD();  // This shall be used for 'select'.

 private:
  static const struct Accept_Flag_t {} Accept_Flag;
  //  explicit SocketTCP4(Accept);

  void Destroy_();
  void Move_(SocketTCP4 *other);

  int fd_;
  Status status_;
  uint32_t addr_local_;
  uint16_t port_local_;
  uint32_t addr_other_;
  uint16_t port_other_;
};

}  // namespace tin

#endif  // SERVER_CORE_SOCKET_H_
