#include <nix_api_util.h>
#include <nix_api_expr.h>
#include <nix_api_value.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nix.h"
 
// NOTE: This example lacks all error handling. Production code must check for
// errors, as some return values will be undefined.


void my_get_string_cb(const char * start, unsigned int n, void * user_data)
{
    *((char **) user_data) = strdup(start);
}

const unsigned int get_nix_path(char** nix_path, int init_nix_path_length) 
{
    char* input = getenv("NIX_PATH");
    char *token;
    const char *delimiter = ":";
    
    int nix_path_length = 0;
    int upper_limit = 100;

    token = strtok(input, delimiter);

    while (token != NULL) {
        if (nix_path_length >= upper_limit) {
            // We don't support more than 100 tokens. This is insane enough.
            break;
        }

        if (nix_path_length >= init_nix_path_length ) {
            init_nix_path_length *= 2;
            nix_path = realloc(nix_path, init_nix_path_length * sizeof(char *));
        }
        
        nix_path[nix_path_length] = malloc(strlen(token) + 1);
        strcpy(nix_path[nix_path_length], token);
        
        nix_path_length++;
        
        token = strtok(NULL, delimiter);
    }

    return nix_path_length;
}
 
nixpkgs_t* get_nixpkgs()
{    
    char **nix_path = malloc(10 * sizeof(char *));
    unsigned int nix_path_length = get_nix_path(nix_path, 10);
    nixpkgs_t* nixpkgs = malloc(sizeof(*nixpkgs));

    nix_libexpr_init(NULL);
 
    Store * store = nix_store_open(NULL, NULL, NULL);
    
    EvalState* state = nix_state_create(NULL, (const char**)nix_path, store);
    nix_value* value = nix_alloc_value(NULL, state);
 
    nix_expr_eval_from_string(NULL, state, "builtins.attrNames (import <nixpkgs> { system = \"x86_64-linux\";})", ".", value);
    nix_value_force(NULL, state, value);
 
    unsigned int num_of_nixpkgs = nix_get_list_size(NULL, value);

    nixpkgs->num_of_pkgs = num_of_nixpkgs;
    nixpkgs->pkgs = malloc(num_of_nixpkgs * sizeof(char *));
    
    for(int i; i < num_of_nixpkgs; ++i) {      
        char* nixpkg;
        nix_value* nixpkg_value = nix_get_list_byidx(NULL, value, state, i);
        nix_get_string(NULL, nixpkg_value, my_get_string_cb, &nixpkg);

        nixpkgs->pkgs[i] = malloc(strlen(nixpkg) + 1);
        strcpy(nixpkgs->pkgs[i], nixpkg);
    }
 
    nix_gc_decref(NULL, value);
    nix_state_free(state);
    nix_store_free(store);
    
    for (int i = 0; i < nix_path_length; i++) {
        free(nix_path[i]);
    }

    free(nix_path);

    return nixpkgs;
}