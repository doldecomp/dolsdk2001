# Dolphin SDK 2001 Libraries

Decompilation of the archive library files of the 5/23/2001 (estimated based on build date of the libraries) version of the Dolphin SDK.

## Compatibility

Currently this repository supports building the following libraries:

| Library Name | Completed? |
| ai           | :x: |
| amcnotstub   | :heavy_check_mark: |
| amcstubs     | :x: |
| ar           | :x: |
| ax           | :x: |
| axfx         | :x: |
| base         | :x: |
| card         | :x: |
| db           | :x: |
| demo         | :x: |
| dolformat    | :x: |
| dsp          | :x: |
| dtk          | :x: |
| dvd          | :x: |
| fileCache    | :x: |
| G2D          | :x: |
| gx           | :x: |
| hio          | :x: |
| mcc          | :x: |
| mix          | :x: |
| mtx          | :x: |
| odemustubs   | :x: |
| odenotstub   | :x: |
| os           | :x: |
| pad          | :x: |
| perf         | :x: |
| seq          | :x: |
| support      | :x: |
| syn          | :x: |
| texPalette   | :x: |
| vi           | :x: |

## Preparation

After cloning the repo, you can copy your copies of the .a files to baserom/ and run `make extract` to disassemble these files to source files with DTK. Currently, please put a DTK binary at root in order to run this repository.

The build process requires the following packages:

- build-essential
- binutils-powerpc-linux-gnu

Under Debian / Ubunutu you can install them with the following commands:

```bash
sudo apt update
sudo apt install build-essential
```

If building any libultra you can install binutils-powerpc-linux-gnu with:

```bash
sudo apt install binutils-powerpc-linux-gnu
```
