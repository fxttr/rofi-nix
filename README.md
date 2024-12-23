# rofi-nix

Run nixpkgs directly from rofi.

Run rofi like:

```bash
    rofi -show rofi-nix -modi rofi-nix 
```

## Compilation

### Dependencies

| Dependency | Version         |
|------------|-----------------|
| rofi 	     | 1.4 (or git)	   |

### Installation

**rofi-nix** uses nix as build system. (of course ;-) ) If installing from git, the following steps should install it:

```bash
$ nix build .#
```
