#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <netinet/in.h>

// Custom headers
#include "config.h"
#include "init.h"

const char VERSION[] = "0.0.1";

void info_menu(char *name);
void help_menu(char *name);

int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-V") == 0) {
            info_menu(argv[0]);
            return 0;
        } else if (strcmp(argv[1], "--help") == 0) {
            help_menu(argv[0]);
            return 0;
        }
    }



    ensure_config_exists();
    const char config[] = "/home/riley/Documents/server.config";

    Config cfg;
    
    if (config_load(&cfg, config)) {
        fprintf(stderr, "Failed to load config\n");
        return 1;
    }

    for (size_t i = 0; i < cfg.count; i++) {
        printf("%s = %s\n", cfg.entries[i].key, cfg.entries[i].value);
    }

    // lookup
    const char *port = config_get(&cfg, "server.port");
    if (port) {
        printf("Server port is %s\n", port);
    }

    return 0;
}

void info_menu(char *name) {
    printf("%s %s\n", name, VERSION);
    printf("Copyright (C) 2025 Riley Ava\n");
    printf("License MPL-2.0: Mozilla Public License 2.0 <https://www.mozilla.org/en-US/MPL/2.0/>\n");
    printf("This is free software: you are free to change and redistribute it.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n");
    printf("\nWritten by Riley Ava.\n");
}

void help_menu(char *name) {
    printf("Usage: %s [OPTION]...\n", name);
    printf("Start a simple HTTP server written in C.\n");
    printf("\n  -p, --port\t\tuse a specific port\n");
    printf("\n  -V, --version\tdisplay version information and exit\n");
    printf("      --help\t\tdisplay this help and exit\n");
    printf("\nReport bugs to: https://github.com/RileyMeta/cweb/issues\n");
    printf("\nHome page: https://github.com/RileyMeta/cweb\n");
}
