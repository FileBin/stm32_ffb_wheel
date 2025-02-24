with (import <nixpkgs> {  });
mkShell {
    packages = [
        gcc-arm-embedded-13
        glibc_multi
        cmake
        stlink
        ninja
        clang-tools
        ncurses
        (vscode-with-extensions.override {
            vscodeExtensions = with vscode-extensions; [
                llvm-vs-code-extensions.vscode-clangd
                ms-vscode.cpptools
                twxs.cmake
                ms-vscode.cmake-tools
                ms-vscode.hexeditor
                tal7aouy.icons
                jnoortheen.nix-ide
                wmaurer.change-case
            ] ++ vscode-utils.extensionsFromVscodeMarketplace [
                {
                    name = "cortex-debug";
                    publisher = "marus25";
                    version = "1.12.1";
                    sha256 = "1kbw93915a2ag3rgddyz4wnlifl6f2m9cxbxqqghfs341f1vm0la";
                }
                {
                    name = "linkerscript";
                    publisher = "zixuanwang";
                    version = "1.0.4";
                    sha256 = "0mfna6xypzfwsrmfzx898pc76ddfn4n8yr8l4d8gbhcyqyj5y3zp";
                }
                {
                    name = "arm";
                    publisher = "dan-c-underwood";
                    version = "1.7.4";
                    sha256 = "1xs5sfppdl7dkh4lyqsipfwax85jpn95rivpqas3z800rpvlr441";
                }
                {
                    name = "debug-tracker-vscode";
                    publisher = "mcu-debug";
                    version = "0.0.15";
                    sha256 = "0wlz089z91my82kygv7kmip0nfkmgczcyl5h1hpxwzvb5ji0rvns";
                }
                {
                    name = "memory-view";
                    publisher = "mcu-debug";
                    version = "0.0.25";
                    sha256 = "0iqfblx5ayhpf1gj3szf3hn6145yqmkgqn18bhi5l9l7h8qkgjad";
                }
                {
                    name = "rtos-views";
                    publisher = "mcu-debug";
                    version = "0.0.7";
                    sha256 = "1nhxhiq6a5rckfy9pqdkjbfq06c4xzlaqm4a5rq5r26a9rhh1wsn";
                }
                {
                    name = "peripheral-viewer";
                    publisher = "mcu-debug";
                    version = "1.4.6";
                    sha256 = "1a499vnjmdxlh98h4c82dfrkafak3rj9h0wiwqrbibd0xcmq2mby";
                }
            ];
        })
    ];
}
