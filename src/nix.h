#ifndef NIX_H
#define NIX_H

typedef struct
{
    char **pkgs;
    unsigned int num_of_pkgs;
} nixpkgs_t;

nixpkgs_t* get_nixpkgs();

#endif