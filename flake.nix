{
  description = "Rofi nixpkgs plugin";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    nix-vscode-extensions.url = "github:nix-community/nix-vscode-extensions";

    code-nix = {
      url = "github:fxttr/code-nix";
      inputs = {
        nixpkgs.follows = "nixpkgs";
        extensions.follows = "nix-vscode-extensions";
      };
    };
  };

  outputs = { self, nixpkgs, ... }@inputs:
    let
      pkgs = import nixpkgs { 
        system = "x86_64-linux";
      };

      buildInputs = with pkgs; [
        rofi-unwrapped
        glib
        cairo
        nix.dev
      ];

      nativeBuildInputs = with pkgs; [
        autoreconfHook
        pkg-config
        gobject-introspection
        wrapGAppsHook3
        python3
      ];

      code = inputs.code-nix.packages.${pkgs.system}.default;
    in
    {
      devShell.x86_64-linux = pkgs.mkShell {
        PATH = "${pkgs.clang-tools}/bin:$PATH";
        LD_LIBRARY_PATH = "${nixpkgs.lib.makeLibraryPath buildInputs}";
        CPATH = nixpkgs.lib.makeSearchPathOutput "dev" "include" buildInputs;

        nativeBuildInputs = nativeBuildInputs ++ (with pkgs; [
          nixpkgs-fmt

          (hiPrio clang-tools.override {
            enableLibcxx = false;
          })

          llvmPackages.lldb
          llvmPackages.clang
          llvmPackages.llvm

          (code {
            profiles = {
              nix.enable = true;
              c.enable = true;
            };
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
