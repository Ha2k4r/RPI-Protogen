{
  pkgs ? import <nixpkgs> {
    overlays = [
      (final: prev: {
        gnutls = prev.gnutls.overrideAttrs (old: {
          outputs =
            if prev.stdenv.buildPlatform != prev.stdenv.hostPlatform then
              builtins.filter (o: o != "man" && o != "devdoc") old.outputs
            else
              old.outputs;
          configureFlags =
            old.configureFlags
            ++ prev.lib.optionals (prev.stdenv.buildPlatform != prev.stdenv.hostPlatform) [ "--disable-doc" ];
        });
      })
    ];
  },
}:

let
  opencv-desktop = pkgs.opencv.override {
    enableGtk3 = true;
  };

  crossPkgs = pkgs.pkgsCross.aarch64-multiplatform;
in

pkgs.mkShell {
  name = "rpi-protogen-dev";

  buildInputs =
    with pkgs;
    [
      cmake
      gnumake
      pkg-config
      ccache
      opencv-desktop
      gtk3
      boost
      dpkg
      wget
      gnutar
      xz # for sysroot maintenance
      debootstrap # for sysroot creation
      sshpass # for deploying to Pi
    ]
    ++ [
      crossPkgs.stdenv.cc
    ];

  shellHook = ''
    # --- Locate or create the Debian Bookworm arm64 sysroot ---
    REPO_ROOT="$(git rev-parse --show-toplevel 2>/dev/null || echo "$PWD")"
    SYSROOT_CANDIDATES=(
      "${builtins.toString ./prootmk3/build/rpi-sysroot}"
      "${builtins.toString ./build/rpi-sysroot}"
      "$REPO_ROOT/prootmk3/build/rpi-sysroot"
      "$REPO_ROOT/build/rpi-sysroot"
    )

    FOUND_SYSROOT=""
    for dir in "''${SYSROOT_CANDIDATES[@]}"; do
      if [ -f "$dir/usr/lib/aarch64-linux-gnu/libc.so.6" ]; then
        FOUND_SYSROOT="$dir"
        break
      fi
    done

    if [ -n "$FOUND_SYSROOT" ]; then
      export SYSROOT="$FOUND_SYSROOT"
      echo "  SYSROOT=$SYSROOT"
    else
      echo "  WARNING: No aarch64 sysroot found."
      echo "  Create one with: tools/create-rpi-sysroot.sh <output-dir>"
      echo "  Then: export SYSROOT=<output-dir>"
    fi

    echo ""
    echo "  RPI-Protogen development shell"
    echo "  -----------------------------"
    echo "  cmake --preset desktop-debug"
    echo "  cmake --build --preset desktop-debug"
    echo ""
    echo "  Cross (RPi):"
    echo "    cmake --preset rpi-release"
    echo "    cmake --build --preset rpi-release"
    echo "    rsync -avz build/rpi-release/proot USERNAME@IP_ADDRESS:~/RPI-Protogen/prootmk3/build/rpi-release/proot"
    echo ""
  '';
}
