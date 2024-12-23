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
        llvmPackages.libstdcxxClang
      ];

      nativeBuildInputs = with pkgs; [
        nixpkgs-fmt

        autoreconfHook
        pkg-config
        gobject-introspection
        wrapGAppsHook3

        llvmPackages.lldb
        llvmPackages.clang
        llvmPackages.llvm
        clang-tools

        (inputs.nix-code.vscode.${system} {
          extensions = with inputs.nix-code.extensions.${system}; [
            bbenoist.nix
            jnoortheen.nix-ide
            mkhl.direnv
            llvm-vs-code-extensions.vscode-clangd
          ];
        })
      ];
    in
    {
      devShell.x86_64-linux = pkgs.mkShell {
        LD_LIBRARY_PATH = "${nixpkgs.lib.makeLibraryPath buildInputs}";

        nativeBuildInputs = nativeBuildInputs;

        buildInputs = buildInputs;
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
