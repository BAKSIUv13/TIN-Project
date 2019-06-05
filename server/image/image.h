// Copyright 2019 TIN

#ifndef SERVER_IMAGE_IMAGE_H_
#define SERVER_IMAGE_IMAGE_H_

#include <utility>
#include <list>
#include <map>
#include <memory>
#include <iostream>
#include <limits>

#include "core/logger.h"
#include "image/image_types.h"
#include "image/basic_object.h"

namespace tin {
class Image {
 public:
  static constexpr ObjectId INIT_NEXT_ID = 100;
  static constexpr ObjectId ID_LIMIT = std::numeric_limits<ObjectId>::max() - 7;
  static constexpr size_t MAX_OBJECTS_NUMBER = 16000;

  class ShapeIterator;

  Image()
    : next_new_id_(INIT_NEXT_ID) {}

  template <typename T>
  std::pair<ObjectId, T *> NewObject() {
    ObjectId id;
    if (objects_.size() >= MAX_OBJECTS_NUMBER) {
      const size_t x = MAX_OBJECTS_NUMBER;  // undefined reference
      LogH << "Nie można dodać kolejnego obiektu do obrazu, osiągnięto "
        << "maksymalną ilość:\n" << x << '\n';
      return std::pair<ObjectId, T *>(IMG_ERR_MAX_COUNT, nullptr);
    }
    id = GetNewId_();
    if (id < 0) {
      LogH << "Przeroczono maksymalne id kształtu, gratulacje xd\n";
      return std::pair<ObjectId, T *>(IMG_ERR_MAX_ID, nullptr);
    }
    T *obj = new T();
    std::unique_ptr<BasicObject> x{obj};
    auto emplace_ret = objects_.emplace(id, std::move(x));
    if (!emplace_ret.second) {
      LogH << "Ojojoj, nie dodano obiektu :<\nw środku jakiś błąd\n";
      // throw std::runtime_error("error at adding new object to image");
      return std::pair<ObjectId, T *>(IMG_ERR_OTHER, nullptr);
    }
    obj_order_.push_back(id);
    addr_to_id_.emplace(&*x, id);
    return std::pair<ObjectId, T *>(id, obj);
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

  void Clear() {
    objects_.clear();
    obj_order_.clear();
    addr_to_id_.clear();
    next_new_id_ = INIT_NEXT_ID;
  }

  class ShapeIterator {
   public:
    ShapeIterator() : image_(nullptr) {}
    ShapeIterator &operator++() {
      ++it_;
      return *this;
    }
    /*
    const BasicObject *operator->() {
      return image_ ?
        &*image_->objects_.at(*it_) : nullptr;
    }
    const BasicObject &operator*() {
      return *operator->();
    }
    */

    ObjectId GetId() const {
      return *it_;
    }

    const BasicObject &GetShape() {
      return *image_->objects_.at(*it_);
    }

    bool operator!=(const ShapeIterator &other) const {
      return image_ != other.image_ || it_ != other.it_;
    }

   private:
    Image *image_;
    std::list<ObjectId>::const_iterator it_;

    friend class Image;
  };

  ShapeIterator cbegin() {
    ShapeIterator it;
    it.image_ = this;
    it.it_ = obj_order_.cbegin();
    return it;
  }

  ShapeIterator cend() {
    ShapeIterator it;
    it.image_ = this;
    it.it_ = obj_order_.cend();
    return it;
  }

 private:
  ObjectId GetNewId_() {
    return next_new_id_ < ID_LIMIT ? next_new_id_++ : -1;
  }

  ObjectId next_new_id_;

  std::map<ObjectId, std::unique_ptr<BasicObject> > objects_;
  std::list<ObjectId> obj_order_;
  std::map<BasicObject *, ObjectId> addr_to_id_;
};
}  // namespace tin

#endif  // SERVER_IMAGE_IMAGE_H_
