#ifndef ANALYZER_H
#define ANALYZER_H

    typedef struct Analyzer Analyzer;
    Analyzer* Analyzer_create(int core_quantity);
    int Analyzer_get_core_quantity(Analyzer* const al);
    void Analyzer_destroy(Analyzer* const al);
    void* analyzer_thread(void* const args);

#endif
