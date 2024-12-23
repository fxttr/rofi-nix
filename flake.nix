{
  description = "Rofi nixpkgs plugin";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    nix-vscode-extensions.url = "github:nix-community/nix-vscode-extensions";

    nix-code = {
      url = "github:fxttr/nix-code";
      inputs.extensions.follows = "nix-vscode-extensions";
    };
  };

  outputs = { self, nixpkgs, ... }@inputs:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };

      buildInputs = with pkgs; [
        rofi-unwrapped
        glib
        cairo
      ];

      nativeBuildInputs = with pkgs; [
        autoreconfHook
        pkg-config
        gobject-introspection
        wrapGAppsHook3
      ];
    in
    {
      devShell.x86_64-linux = pkgs.mkShell {
        LD_LIBRARY_PATH = "${nixpkgs.lib.makeLibraryPath buildInputs}";
        CPATH = nixpkgs.lib.makeSearchPathOutput "dev" "include" buildInputs;

        nativeBuildInputs = nativeBuildInputs ++ (with pkgs; [
          nixpkgs-fmt

          llvmPackages.lldb
          llvmPackages.clang
          llvmPackages.llvm

          (hiPrio clang-tools.override {
            enableLibcxx = false;
          })

          (inputs.nix-code.vscode.${system} {
            extensions = with inputs.nix-code.extensions.${system}; [
              bbenoist.nix
              jnoortheen.nix-ide
              mkhl.direnv
              llvm-vs-code-extensions.vscode-clangd
            ];
          })
        ]);

        buildInputs = buildInputs ++ (with pkgs; [
          llvmPackages.libstdcxxClang
        ]);
      };

      defaultPackage.x86_64-linux = pkgs.stdenv.mkDerivation
        {
          pname = "rofi-nix";
          version = "0.0.1";

          src = ./.;

          nativeBuildInputs = nativeBuildInputs;

          buildInputs = buildInputs;

          meta = with nixpkgs.lib; {
            description = "Run nixpkgs with rofi";
            homepage = "https://github.com/fxttr/rofi-nix";
            license = licenses.mit;
            maintainers = with maintainers; [ fxttr ];
            platforms = with platforms; linux;
          };
        };
    };
}
