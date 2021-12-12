# This shell defines a development environment for a c project.
{
  pkgs ? import <nixos>{}
}:

pkgs.mkShell.override {stdenv = pkgs.llvmPackages_10.stdenv;} {
  buildInputs = with pkgs; [
    shellcheck
  ];

  hardeningDisable = [ "fortify" ];

  shellHook = ''
    # start user default shell
    SHELL=$(awk -F: -v user="$USER" '$1 == user {print $NF}' /etc/passwd)
    $SHELL
  '';

  NIX_ENFORCE_PURITY=0;
}
