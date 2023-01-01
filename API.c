#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

typedef struct {
    sqlite3 *conn;
} DatabaseAPI;

int store(DatabaseAPI *api, const char *key, const char *value) {
    char *error_message = 0;
    int rc = sqlite3_exec(api->conn, "CREATE TABLE IF NOT EXISTS data (key TEXT PRIMARY KEY, value TEXT)", 0, 0, &error_message);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message);
        return 1;
    }

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(api->conn, "INSERT INTO data (key, value) VALUES (?, ?)", -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(api->conn));
        return 1;
    }

    rc = sqlite3_bind_text(stmt, 1, key, -1, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(api->conn));
        return 1;
    }
    rc = sqlite3_bind_text(stmt, 2, value, -1, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(api->conn));
        return 1;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(api->conn));
        return 1;
    }

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(api->conn));
        return 1;
    }

    return 0;
}

char *retrieve(DatabaseAPI *api, const char *key) {
    char *error_message = 0;
    int rc = sqlite3_exec(api->conn, "CREATE TABLE IF NOT EXISTS data (key TEXT PRIMARY KEY, value TEXT)", 0, 0, &error_message);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message);
        return NULL;
    }

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(api->conn, "SELECT value FROM data WHERE key=?", -1, &stmt, 0);
    if (rc != SQLITE_
