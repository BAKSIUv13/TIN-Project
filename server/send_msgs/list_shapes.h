// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_LIST_SHAPES_H_
#define SERVER_SEND_MSGS_LIST_SHAPES_H_

#include <string>
#include <list>

#include "core/out_message.h"
#include "core/write_buf.h"
#include "image/basic_object.h"
#include "image/image.h"

namespace tin {

class ListShapes : public OutMessage {
 public:
  using ShapeIt = Image::ShapeIterator;
  ListShapes() {}
  explicit ListShapes(const Username &un, std::array<ShapeIt, 2> iterators)
      : username_(un) {
    for (auto it = iterators[0]; it != iterators[1]; ++it) {
      ++shapes_count_;
      it->WriteToCppString(&shapes_string_);
    }
  }

  virtual ~ListShapes() {}

  virtual std::string GetTypeName() {return "ListShapes";}

  virtual int Audience() {return ONE_U;}

  Username User() override {
    return username_;
  }

  virtual int AddToBuf(WriteBuf *buf);

 private:
  Username username_;
  uint32_t shapes_count_;
  std::string shapes_string_;
};  // class ListShapes
}  // namespace tin

#endif  // SERVER_SEND_MSGS_LIST_SHAPES_H_
