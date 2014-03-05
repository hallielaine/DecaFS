#ifndef ESPRESSO_TYPES_H_
#define ESPRESSO_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

struct data_descriptor {
    int file_id;
    int stripe_id;
    int chunk_num;
};

struct data_address {
    int offset;
    int size;
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ESPRESSO_TYPES_H_
