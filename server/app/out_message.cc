// Copyright 2019 Piotrek

#include "app/out_message.h"


namespace tin {

OutMessage::OutMessage(const Username &un, const std::string &content)
    : type_(USER_MESSAGE), username_(un), content_(content) {}

OutMessage::OutMessage(const std::string &content)
    : type_(ANNOUNCEMENT), content_(content) {}

}  // namespace tin
