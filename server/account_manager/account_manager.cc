// Copyright 2019 TIN

#include "account_manager/account_manager.h"

#include <unistd.h>

#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>
#include <ios>
#include <utility>
#include <algorithm>


namespace tin {

namespace {

constexpr const char BASE64_CHARS[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789+/=";

constexpr char char_b64_to_number(char c) {
  return c > 'z' ? '\xff' :
    c >= 'a' ? c - 'a' + 26 :
    c > 'Z' ? '\xff' :
    c >= 'A' ? c - 'A' :
    c > '9' ? '\xff' :
    c >= '0' ? c - '0' + 52 :
    c == '+' ? 62 :
    c == '/' ? 63 : '\xff';
}

constexpr size_t base64_len(size_t x) {
  return (x * 4 / 3 + 3) & (3 ^ -1);
}

constexpr size_t max2(size_t a, size_t b) {
  return a > b ? a : b;
}

constexpr size_t max3(size_t a, size_t b, size_t c) {
  return max2(max2(a, b), c);
}

inline bool is_b64(char c) {
  return std::isalnum(c) || c == '/' || c == '+';
}

inline Username un_to_delim(const char *s, char delim) {
  char x[Username::MAX_NAME_LEN + 1];
  size_t i = 0;
  for (; i < Username::MAX_NAME_LEN && s[i] != '\0' && s[i] != delim; ++i)
    x[i] = s[i];
  x[i] = '\0';
  return Username(x);
}

constexpr size_t LINE_BUF_LEN =
  Username::MAX_NAME_LEN +
  1 +  // :
  1 +  // x
  1 +  // :
  base64_len(AccountManager::MAX_SALT_LEN) +
  1 +  // :
  base64_len(SHA256_DIGEST_LENGTH) +
  10 +  // na wszelki wypadek
  1;  // '\0'

constexpr size_t ELEM_BUF_LEN = max3(
  Username::MAX_NAME_LEN,
  base64_len(AccountManager::MAX_SALT_LEN),
  base64_len(SHA256_DIGEST_LENGTH)
) + 4;

std::string xor_strs(std::string one, const std::string &two) {
  size_t i = 0;
  size_t end = std::min(one.size(), two.size());
  for (; i < end; ++i) {
    one[i] ^= two[i];
  }
  for (; i < two.size(); ++i) {
    one.push_back(two[i]);
  }
  return one;
}

std::string make_hash(const std::string &s) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha;
  SHA256_Init(&sha);
  SHA256_Update(&sha, s.c_str(), s.size());
  SHA256_Final(hash, &sha);
  return std::string(reinterpret_cast<char *>(hash), SHA256_DIGEST_LENGTH);
}

std::string make_base64(const std::string &s) {
  // Piszę 'własne', bo nie ogarniam tego z OpenSSL
  std::string res;
  uint8_t normal[3];
  uint8_t b64[4];
  size_t i = 0;
  for (char x : s) {
    normal[i] = x;
    ++i;
    if (i >= 3) {
      b64[0] =  (normal[0] & 0b11111100) >> 2;

      b64[1] = ((normal[0] & 0b00000011) << 4) |
               ((normal[1] & 0b11110000) >> 4);

      b64[2] = ((normal[1] & 0b00001111) << 2) |
               ((normal[2] & 0b11000000) >> 6);

      b64[3] =   normal[2] & 0b00111111;

      for (size_t j = 0; j < 4; ++j)
        res.push_back(BASE64_CHARS[b64[j]]);
      i = 0;
    }
  }
  if (i > 0) {
    // Nie dokończyliśmy trójki
    for (size_t j = i; j < 3; ++j)
      normal[j] = '\0';

    b64[0] =  (normal[0] & 0b11111100) >> 2;

    b64[1] = ((normal[0] & 0b00000011) << 4) |
             ((normal[1] & 0b11110000) >> 4);

    b64[2] = i >= 2 ?
             ((normal[1] & 0b00001111) << 2) |
             ((normal[2] & 0b11000000) >> 6) :
             '\x40';

    b64[3] = '\x40';

    for (size_t j = 0; j < 4; ++j)
      res.push_back(BASE64_CHARS[b64[j]]);
  }
  return res;
}

std::string unmake_base64(const std::string &s) noexcept {
  std::string res;
  uint8_t normal[3];
  uint8_t b64[4];
  size_t i = 0;
  for (char x : s) {
    if (x == '=')
      break;
    b64[i] = x;
    ++i;
    if (i >= 4) {
      for (size_t j = 0; j < 4; ++j)
        b64[j] = char_b64_to_number(b64[j]);

      normal[0] = ((b64[0] & 0b00111111) << 2) |
                  ((b64[1] & 0b00110000) >> 4);

      normal[1] = ((b64[1] & 0b00001111) << 4) |
                  ((b64[2] & 0b00111100) >> 2);

      normal[2] = ((b64[2] & 0b00000011) << 6) |
                   (b64[3] & 0b00111111);

      for (size_t j = 0; j < 3; ++j)
        res.push_back(normal[j]);
      i = 0;
    }
  }
  if (i > 0) {
    for (size_t j = 0; j < i; ++j)
      b64[j] = char_b64_to_number(b64[j]);
    for (size_t j = i; j < 4; ++j)
      b64[j] = '\0';

    normal[0] =   ((b64[0] & 0b00111111) << 2) |
                  ((b64[1] & 0b00110000) >> 4);
    res.push_back(normal[0]);

    if (i >= 3) {
      normal[1] = ((b64[1] & 0b00001111) << 4) |
                  ((b64[2] & 0b00111100) >> 2);
      res.push_back(normal[1]);
    }
  }
  return res;
}

std::string GetFileLine(std::fstream &file, const Username &un) {
  char buf[LINE_BUF_LEN];
  std::stringstream ss;
  file.seekg(0);
  while (file.getline(buf, LINE_BUF_LEN, '\n')) {
    Username user = un_to_delim(buf, ':');
    if (user == un) {
      return buf;
    }
  }
  file.clear();
  return "";
}


struct ShadowLine {
  std::string hash;
  std::string salt;
  Username name;
  bool is_admin;
};

int make_shadow_line(ShadowLine *sl, const std::string &s) {
  char buf[ELEM_BUF_LEN];
  std::istringstream ss(s);
  ss.getline(buf, ELEM_BUF_LEN, ':');
  if (ss.eof()) return -1;
  Username un{buf};
  if (!un) return -1;
  sl->name = un;
  ss.getline(buf, ELEM_BUF_LEN, ':');
  if (ss.eof()) return -1;
  if (buf[0] == '\0') {
    sl->is_admin = false;
  } else if (buf[0] == 'x' && buf[1] == '\0') {
    sl->is_admin = true;
  } else {
    return -1;
  }
  ss.getline(buf, ELEM_BUF_LEN, ':');
  if (ss.eof()) return -1;
  std::string salt{std::move(unmake_base64(buf))};
  sl->salt = std::move(salt);
  ss.getline(buf, ELEM_BUF_LEN, ':');
  std::string passwd{std::move(unmake_base64(buf))};
  sl->hash = std::move(passwd);
  return 0;
}

bool check_pass(std::string passwd, std::string salt, std::string hash) {
  passwd = xor_strs(passwd, salt);
  passwd = make_hash(passwd);
  return passwd == hash;
}

}  // namespace

int AccountManager::AttachFile(const char *path, bool writable) {
  if (state_ != BLANK) return -1;
  the_file_.open(path, writable ?
    std::ios::in | std::ios::out :
    std::ios::in);
  if (!the_file_.is_open()) {
    return -1;
  }
  state_ = writable ? ATTACHED_RDWR : ATTACHED_RD;
  return 0;
}

int AccountManager::DetachFile() {
  if (state_ == BLANK) return -1;
  the_file_.close();
  state_ = BLANK;
  return 0;
}

int AccountManager::Authenticate(Username *un, std::string passwd) {
  if (!un->Good()) {
    *un = Username();
    return UserPass::NOT_ALLOWED;
  }
  if (state_ == BLANK)
    return UserPass::GUEST;
  std::string line = GetFileLine(the_file_, *un);
  if (line == "") {
    switch (ga_) {
      case GuestAccess::ANY_PASSWD:
        return UserPass::GUEST;
      case GuestAccess::EMPTY_PASSWD:
        if (passwd == "") return UserPass::GUEST;
      case GuestAccess::NONE: default:
        *un = Username();
        return UserPass::NOT_ALLOWED;
    }
  }
  ShadowLine sl;
  int res = make_shadow_line(&sl, line);
  if (res < 0) return -1;
  if (check_pass(passwd, sl.salt, sl.hash)) {
    *un = sl.name;
    return sl.is_admin ? UserPass::ADMIN : UserPass::NORMAL;
  }
  *un = Username();
  return UserPass::NOT_ALLOWED;
}

int AccountManager::test(int argc, char **argv, char **env) {
  //std::cerr << "Samurai Jack:x:" << make_base64("Aku") << ":" << make_base64(make_hash(xor_strs("Aku", "xdxd"))) << '\n';
  std::cerr << "admin:x:" << make_base64("11111") << ":" << make_base64(make_hash(xor_strs("1111", "admin"))) << '\n';
  //std::cerr << "Shrek::" << make_base64("All Star") << ":" << make_base64(make_hash(xor_strs("All Star", "osle!"))) << '\n';

  return 0;
  int res;
  AccountManager am;
  res = am.AttachFile("shadow.log", false);
  std::cerr << "1: " << res << '\n';
  Username un("hehehe");
  //std::cerr << un << '\n';
  //std::cerr << am.Authenticate(&un, "xdxd") << '\n';
  //std::cerr << un << '\n';
  const char *x[][2] = {
    {"xd", "xd"},
    {"samuraijack", "xdxd"},
    {"Teemo", "xd"},
    {"nkp123", "xd"},
    {"nkp123", "michau3"},
    {"SHREK", "osle!"},
  };
  for (size_t i = 0; i < sizeof x / (&x[1] - &x[0]); ++i) {
    Username unn(x[i][0]);
    std::cerr << unn <<'\n';
    std::cerr << am.Authenticate(&unn, x[i][1]) << '\n';
    std::cerr << unn <<'\n';
  }

  std::string line = GetFileLine(am.the_file_, un);
  std::cerr << "== " << line << '\n';

  return 0;
}

}  // namespace tin
