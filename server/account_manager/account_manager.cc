// Copyright 2019 TIN

#include "account_manager/account_manager.h"

#include <unistd.h>
#include <fcntl.h>

#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <set>
#include <cassert>
#include <ctime>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
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

struct ShadowLine {
  std::string hash;
  std::string salt;
  Username name;
  bool is_admin;
};

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


void hash_passwd(const std::string &passwd, AccountManager::Rand *rand,
    ShadowLine *sl) {
  AccountManager::Rand::result_type salt_int = rand->operator()();
  sl->salt = std::string(reinterpret_cast<char *>(&salt_int), sizeof salt_int);
  sl->hash = make_hash(xor_strs(passwd, sl->salt));
}


// Checks if file is empty or has '\n' on end.
bool chk_file_end(FILE *file) {
  fseek(file, 0, SEEK_END);
  if (ftell(file) == 0) {
    return true;
  }
  fseek(file, -1, SEEK_END);
  if (fgetc(file) == '\n')
    return true;
  return false;
}

int ch_file_bytes(FILE *file, intptr_t off, intptr_t len,
    const std::string &s) {
  static constexpr intptr_t BUF_SIZE = 256;
  char buf[BUF_SIZE];
  if (len == (intptr_t)s.size()) {
    fseek(file, off, SEEK_SET);
    fwrite(s.c_str(), s.size(), 1, file);
  } else if ((intptr_t)s.size() < len) {
    intptr_t end_of_file;
    fseek(file, 0, SEEK_END);
    end_of_file = ftell(file);
    fseek(file, off, SEEK_SET);
    fwrite(s.c_str(), s.size(), 1, file);
    bool end;
    fseek(file, off + len, SEEK_SET);
    intptr_t i = 0, j = 0;
    while (!end) {
      j = 0;
      while (j < BUF_SIZE) {
        int c = fgetc(file);
        if (c == EOF) {
          end = true;
          break;
        }
        buf[i] = c;
        ++i;
        ++j;
      }
      fseek(file, off + s.size() + i - j, SEEK_SET);
      int jj = 0;
      while (jj < j) {
        fputc(buf[jj], file);
        ++jj;
      }
    }
    // truncate
    fflush(file);
    fseek(file, 0, SEEK_SET);
    int fd = fileno(file);
    ftruncate(fd, end_of_file - (len - s.size()));
    fflush(file);
    fseek(file, 0, SEEK_SET);
    //
  } else {  // s.size() > len
    intptr_t increase = s.size() - len;
    fseek(file, 0, SEEK_END);
    intptr_t file_end = ftell(file);
    bool end = false;
    intptr_t bytes_to_shift = BUF_SIZE;
    intptr_t where = file_end;
    while (!end) {
      where -= BUF_SIZE;
      if (where < off + len) {
        bytes_to_shift -= (off + len) - where;
        where = off + len;
        end = true;
      }
      fseek(file, where, SEEK_SET);
      fread(buf, bytes_to_shift, 1, file);
      fseek(file, where + increase, SEEK_SET);
      fwrite(buf, bytes_to_shift, 1, file);
    }
    fseek(file, off, SEEK_SET);
    fwrite(s.c_str(), s.size(), 1, file);
  }
  fflush(file);
  return 0;
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

int read_line(FILE *file, char *buf, size_t len, char delim) {
  int c;
  size_t i = 0;
  while ((c = fgetc(file)) != EOF && c != delim && i < len - 1) {
    buf[i] = c;
    ++i;
  }
  buf[i] = '\0';
  return i;
}

int get_file_line(FILE *file, const Username &un, std::string *s) {
  char buf[LINE_BUF_LEN];
  std::stringstream ss;
  fseek(file, 0, SEEK_SET);
  intptr_t off = 0;
  while (read_line(file, buf, LINE_BUF_LEN, '\n')) {
    Username user = un_to_delim(buf, ':');
    if (user == un) {
      if (s != nullptr)
        *s = std::string(buf);
      return off;
    }
    off = ftell(file);
  }
  return -1;
}




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


std::string gen_shadow_line(const ShadowLine *sl) {
  std::string res;
  res.append(sl->name);
  res.push_back(':');
  if (sl->is_admin) res.push_back('x');
  res.push_back(':');
  res.append(make_base64(sl->salt));
  res.push_back(':');
  res.append(make_base64(sl->hash));
  return res;
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int AccountManager::AttachFile(const char *path, bool writable) {
  if (state_ != BLANK) return -1;
  the_file_ = fopen(path, writable ? "r+b" : "rb");
  if (the_file_ == nullptr) return -1;
  // the_file_.open(path, writable ?
  //   std::ios::in | std::ios::out :
  //   std::ios::in);
  // We need this to have fd to use ftruncate/
  state_ = writable ? ATTACHED_RDWR : ATTACHED_RD;
  return 0;
}

int AccountManager::DetachFile() {
  if (state_ == BLANK) return -1;
  fclose(the_file_);
  state_ = BLANK;
  return 0;
}

int AccountManager::Authenticate(Username *un,
    std::string passwd) {
  if (!un->Good()) {
    *un = Username();
    return UserPass::NOT_ALLOWED;
  }
  if (state_ == State::BLANK)
    return UserPass::GUEST;
  std::string line;
  int res = get_file_line(the_file_, *un, &line);
  if (res < 0) {
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
  res = make_shadow_line(&sl, line);
  if (res < 0) return -1;
  if (check_pass(passwd, sl.salt, sl.hash)) {
    *un = sl.name;
    return sl.is_admin ? UserPass::ADMIN : UserPass::NORMAL;
  }
  *un = Username();
  return UserPass::NOT_ALLOWED;
}


int AccountManager::UserAdd(const Username &un, std::string passwd,
    bool admin) {
  if (!Writable()) return -1;
  std::string line;
  int res = get_file_line(the_file_, un, nullptr);
  if (res >= 0) return -1;
  ShadowLine sl;
  sl.name = un;
  sl.is_admin = admin;
  hash_passwd(passwd, &rand_, &sl);
  line = gen_shadow_line(&sl);
  bool dont_add_enter = chk_file_end(the_file_);
  fseek(the_file_, 0, SEEK_END);
  res = fprintf(the_file_, "%s%s\n", (dont_add_enter ? "" : "\n"), line.c_str());
  assert(res >= 0);
  fflush(the_file_);
  return 0;
}

int AccountManager::UserDel(const Username &un) {
  if (!Writable()) return -1;
  std::string line;
  int res = get_file_line(the_file_, un, &line);
  if (res < 0) return -1;
  ch_file_bytes(the_file_, res, line.size() + 1, "");
  return 0;
}

int AccountManager::UserChPasswd(const Username &un, const std::string &passwd) {
  if (!Writable()) return -1;
  std::string line;
  int res = get_file_line(the_file_, un, &line);
  if (res < 0) return -1;
  intptr_t old_len = line.size();
  ShadowLine sl;
  make_shadow_line(&sl, line);
  hash_passwd(passwd, &rand_, &sl);
  line = gen_shadow_line(&sl);
  ch_file_bytes(the_file_, res, old_len, line);
  return 0;
}

int AccountManager::UserChRole(const Username &un, bool admin) {
  if (!Writable()) return -1;
  std::string line;
  int res = get_file_line(the_file_, un, &line);
  if (res < 0) return -1;
  intptr_t old_len = line.size();
  ShadowLine sl;
  make_shadow_line(&sl, line);
  sl.is_admin = admin;
  line = gen_shadow_line(&sl);
  ch_file_bytes(the_file_, res, old_len, line);
  return 0;
}

int AccountManager::test(int argc, char **argv, char **env) {
  int res;
  AccountManager am;
  am.FeedRand(std::time(nullptr) ^ getpid());
  res = am.AttachFile("shadow.log", true);

  return 0 & res;
}

}  // namespace tin
