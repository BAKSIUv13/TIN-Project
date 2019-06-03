// Copyright 2019 TIN

#ifndef SERVER_IMAGE_IMAGE_H_
#define SERVER_IMAGE_IMAGE_H_

#include <utility>
#include <list>
#include <map>
#include <memory>
#include <iostream>

#include "core/logger.h"
#include "image/image_types.h"
#include "image/basic_object.h"

namespace tin {
class Image {
 public:
  Image()
    : next_new_id_(100) {}

  template <typename T>
  std::pair<ObjectId, T &> NewObject() {
    ObjectId id = GetNewId_();
    T *obj = new T();
    std::unique_ptr<BasicObject> x{obj};
    auto emplace_ret = objects_.emplace(id, std::move(x));
    if (!emplace_ret.second) {
      LogH << "Ojojoj, nie dodano obiektu :<\n";
      throw std::runtime_error("error at adding new object to image");
    }
    obj_order_.push_back(id);
    addr_to_id_.emplace(&*x, id);
    return std::pair<ObjectId, T &>(id, *obj);
  }

  ObjectId GetId(BasicObject *o) {
    try {
      return addr_to_id_.at(o);
    }
    catch (std::out_of_range &e) {
      return -1;
    }
  }

  BasicObject *GetObject(ObjectId id) {
    if (objects_.count(id) < 1)
      return nullptr;
    return &*objects_.at(id);
  }

 private:
  ObjectId GetNewId_() {
    return next_new_id_++;
  }

  ObjectId next_new_id_;

  std::map<ObjectId, std::unique_ptr<BasicObject> > objects_;
  std::list<ObjectId> obj_order_;
  std::map<BasicObject *, ObjectId> addr_to_id_;
};
}  // namespace tin

#endif  // SERVER_IMAGE_IMAGE_H_
