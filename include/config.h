#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CONFIG_MAX_ENTRIES 256
#define CONFIG_MAX_KEY     128
#define CONFIG_MAX_VALUE   256

typedef struct {
    char key[CONFIG_MAX_KEY];
    char value[CONFIG_MAX_VALUE];
} ConfigEntry;

typedef struct {
    ConfigEntry entries[CONFIG_MAX_ENTRIES];
    size_t count;
} Config;

static void trim(char *s) {
    // Trim leading
    while (isspace((unsigned char)*s)) s++;

    // Trim trailing
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) *end-- = '\0';

    memmove(s, s, strlen(s) + 1);
}

static void strip_quotes(char *s) {
    if ((s[0] == '"' || s[0] == '\'') && s[strlen(s) - 1] == s[0]) {
        s[strlen(s) - 1] = '\0';
        memmove(s, s + 1, strlen(s));
    }
}

static int config_load(Config *cfg, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    char line[512];
    char section[64] = "";

    cfg->count = 0;

    while (fgets(line, sizeof(line), fp)) {
        trim(line);
        if (line[0] == '\0' || line[0] == '#' || line[0] == ';') continue;

        if (line[0] == '[') {
            char *end = strchr(line, ']');
            if (end) {
                *end = '\0';
                strncpy(section, line + 1, sizeof(section) - 1);
            }
            continue;
        }

        char *eq = strchr(line, '=');
        if (!eq) continue;

        *eq = '\0';
        char *key = line;
        char *value = eq + 1;

        trim(key);
        trim(value);
        strip_quotes(value);

        if (cfg->count >= CONFIG_MAX_ENTRIES) break;

        if (strlen(section) > 0) {
            snprintf(cfg->entries[cfg->count].key, CONFIG_MAX_KEY, "%s.%s", section, key);
        } else {
            strncpy(cfg->entries[cfg->count].key, key, CONFIG_MAX_KEY);
        }
        strncpy(cfg->entries[cfg->count].value, value, CONFIG_MAX_VALUE);
        cfg->count++;
    }

    fclose(fp);
    return 0;
}

static const char *config_get(Config *cfg, const char *key) {
    for (size_t i = 0; i < cfg->count; i++) {
        if (strcmp(cfg->entries[i].key, key) ==0) {
            return cfg->entries[i].value;
        }
    }
    return NULL;
}

#endif
