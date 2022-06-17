#ifndef UTILS_H
#define UTILS_H
    typedef struct Reader_Utils Reader_Utils;
    Reader_Utils* Reader_Utils_create(Buffer** bf,Reader** rd);
    void Reader_Utils_destroy(Reader_Utils* utils);
    Buffer* Reader_Utils_get_buffer(Reader_Utils* utils);
    Reader* Reader_Utils_get_reader(Reader_Utils* utils);
#endif