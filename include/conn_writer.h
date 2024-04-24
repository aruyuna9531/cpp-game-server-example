#ifndef SVR_CONN_WRITER_H_
#define SVR_CONN_WRITER_H_
#include <list>
#include <stdint.h>
#include <memory>

class WriteObject{
public:
    std::streamsize start;   // std::streamsize: long int
    std::streamsize len;
    std::shared_ptr<char> buff;
public:
    ~WriteObject();
};

class ConnWriter {
public:
    int fd;
private:
    bool isClosing = false;
    std::list<std::shared_ptr<WriteObject>> objs;
public:
    void EntireWrite(std::shared_ptr<char> buff, std::streamsize len);
    void LingerClose();
    void OnWriteable();
private:
    void EntireWriteWhenEmpty(std::shared_ptr<char> buff, std::streamsize len);
    void EntireWriteWhenNotEmpty(std::shared_ptr<char> buff, std::streamsize len);
    bool WriteFrontObj();

public:
};
#endif