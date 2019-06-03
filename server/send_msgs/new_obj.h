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
  NewObj(const Username &un, ObjectId id, std::unique_ptr<BasicObject> &&obj)
      : OutMessage(), username_(un), id_(id), shape_copy_(std::move(obj)) {}

  virtual ~NewObj();

  virtual std::string GetTypeName() {return "NewObj";}

  virtual int Audience() {return BROADCAST_U;}

  Username GetUsername() const {return username_;}
  constexpr ObjectId Id() const {return id_;}
  const BasicObject *Shape() const {return &*shape_copy_;}

  virtual int AddToBuf(WriteBuf *buf);
 private:
  Username username_;
  ObjectId id_;
  std::unique_ptr<BasicObject> shape_copy_;
};  // class MouseMoved
}  // namespace tin

#endif  // SERVER_SEND_MSGS_MOUSE_MOVED_H_
