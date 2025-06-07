#ifndef INIT_H
#define INIT_H

// size_t
#include <stddef.h>

void expand_path(const char *path, char *out, size_t out_size);
void ensure_config_exists(void);

#endif
