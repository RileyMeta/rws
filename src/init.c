#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define CONFIG_DIR  "~/.config/rws"
#define CONFIG_FILE "~/.config/rws/server.config"

void expand_path(const char *path, char *out, size_t out_size) {
    if (path[0] == '~') {
        const char *home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "HOME not set\n");
            exit(1);
        }
        snprintf(out, out_size, "%s%s", home, path + 1);
    } else {
        snprintf(out, out_size, "%s", path);
    }
}

void ensure_config_exists() {
    char path[512], dir[512];
    expand_path(CONFIG_FILE, path, sizeof(path));
    expand_path(CONFIG_DIR, dir, sizeof(dir));

    // Check if file exists
    if (access(path, F_OK) != 0) {
        // Create config dir if not exists
        struct stat st = {0};
        if (stat(dir, &st) == -1) {
            mkdir(dir, 0700);
        }

        // Auto-gen default config
        FILE *f = fopen(path, "w");
        if (!f) {
            perror("fopen");
            exit(1);
        }
        fprintf(f, "# Auto-generated config\n");
        fprintf(f, "[Default Settings]\n");
        fprintf(f, "port=8080\n");
        fprintf(f, "root=\"./root/\"\n");
        fprintf(f, "\n[Aesthetic Settings]\n");
        fprintf(f, "show_extensions=false\n");

        fclose(f);

        printf("Default config auto-generated at: %s\n", path);
    }
}
