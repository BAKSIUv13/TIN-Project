// Copyright 2019 TIN

#ifndef SERVER_IMAGE_IMAGE_H_
#define SERVER_IMAGE_IMAGE_H_

#include <utility>
#include <list>
#include <map>
#include <memory>
#include <iostream>

#include "core/logger.h"
#include "image/basic_object.h"

namespace tin {
class Image {
 public:
  using Id = int32_t;
  using Dist = BasicObject::Dist;
  using Vec = BasicObject::Vec;
  using Transform = BasicObject::Transform;

  Image()
    : next_new_id_(100) {}

  template <typename T>
  T &NewObject() {
    Id id = GetNewId_();
    std::unique_ptr<BasicObject> x = new T();
    auto emplace_ret = objects_.emplace(id, std::move(x));
    if (!emplace_ret.second) {
      LogH << "Ojojoj, nie dodano obiektu :<\n";
      throw std::runtime_error("error at adding new object to image");
    }
    obj_order_.push_back(id);
    addr_to_id_.emplace(&*x, id);
    return *x;
  }

  Id GetId(BasicObject *o) {
    try {
      return addr_to_id_.at(o);
    }
    catch (std::out_of_range &e) {
      return -1;
    }
  }

 private:
  Id GetNewId_() {
    return next_new_id_++;
  }

  Id next_new_id_;

  std::map<Id, std::unique_ptr<BasicObject> > objects_;
  std::list<Id> obj_order_;
  std::map<BasicObject *, Id> addr_to_id_;
};
}  // namespace tin

#endif  // SERVER_IMAGE_IMAGE_H_
