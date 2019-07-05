// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_NEW_OBJ_H_
#define SERVER_SEND_MSGS_NEW_OBJ_H_

#include <string>
#include <list>
#include <memory>
#include <utility>

#include "core/out_message.h"
#include "core/write_buf.h"

#include "image/image_types.h"
#include "image/basic_object.h"

namespace tin {

class NewObj : public OutMessage {
 public:
  using ObjectId = ImageTypes::ObjectId;

  NewObj() {}
  NewObj(const Username &un, ObjectId id, const std::string &str)
      : OutMessage(), username_(un), id_(id), shape_code_(str) {}

  virtual ~NewObj();

  virtual std::string GetTypeName() const {return "NewObj";}

  virtual int Audience() const {return BROADCAST_U;}

  Username GetUsername() const {return username_;}
  constexpr ObjectId Id() const {return id_;}
  const std::string &Code() const {return shape_code_;}

  virtual std::string GetStr() const;
 private:
  Username username_;
  ObjectId id_;
  std::string shape_code_;
  // std::unique_ptr<BasicObject> shape_copy_;
};  // class MouseMoved
}  // namespace tin

#endif  // SERVER_SEND_MSGS_MOUSE_MOVED_H_
