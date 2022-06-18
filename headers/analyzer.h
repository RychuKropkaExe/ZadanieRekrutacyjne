#ifndef ANALYZER_H
#define ANALYZER_H
    typedef struct Analyzer Analyzer;
    Analyzer* Analyzer_create(int core_quantity);
    int Analyzer_get_core_quantity(Analyzer* al);
    void Analyzer_destroy(Analyzer* al);
    void* analyzer_thread(void* args);
#endif