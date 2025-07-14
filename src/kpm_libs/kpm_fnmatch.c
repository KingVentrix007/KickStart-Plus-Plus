#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool partial_match(char *pattern, char *string) {
    return strstr(string, pattern) != NULL;
}

bool full_match(char *pattern, char *string) {
    return strcmp(pattern, string) == 0;
}

bool is_in(char **list, char *value, size_t list_len) {
    for (size_t i = 0; i < list_len; i++) {
        if (strcmp(list[i], value) == 0) {
            return true;
        }
    }
    return false;
}

char **single_pattern_match(char *pattern, char **strings, size_t string_count, size_t *match_count) {
    char **string_matches = malloc(string_count * sizeof(char *));
    size_t current_matches = 0;
    if (string_matches == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < string_count; i++) {
        bool matched = false;

        if (pattern[0] == '*') {
            matched = partial_match(pattern + 1, strings[i]);
        } else {
            matched = full_match(pattern, strings[i]);
        }

        if (matched && !is_in(string_matches, strings[i], current_matches)) {
            string_matches[current_matches++] = strdup(strings[i]);
        }
    }

    *match_count = current_matches;
    return string_matches;
}

char **kpm_fnmatch(char **patterns, char **strings, size_t pattern_count, size_t strings_count) {
    char **all_matches = malloc(pattern_count * strings_count * sizeof(char *));
    size_t total_matches = 0;

    if (all_matches == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < pattern_count; i++) {
        size_t num_found_matches = 0;
        char **curr_matches = single_pattern_match(patterns[i], strings, strings_count, &num_found_matches);

        if (curr_matches != NULL) {
            for (size_t j = 0; j < num_found_matches; j++) {
                if (!is_in(all_matches, curr_matches[j], total_matches)) {
                    all_matches[total_matches++] = curr_matches[j];
                } else {
                    free(curr_matches[j]); // already exists, free duplicate
                }
            }
            free(curr_matches); // free container only
        }
    }

    if (total_matches == 0) {
        free(all_matches);
        return NULL;
    }

    return all_matches;
}
