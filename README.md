# EECS4760 - Computer Security
[![Build status](https://ci.appveyor.com/api/projects/status/yxjrmimmydrfmass?svg=true)](https://ci.appveyor.com/project/techwiz24/eecs4760)

This repository contains project code and samples for EECS4760 (Computer Security)
for the Fall 2016 Semester at the University of Toledo.

## Projects
 * DES - An implementation of the Data Encryption Standard (ECB and CBC Mode)
 * FStats - Collects statistics about files (Used for analyzing encryption effectiveness)

## Building
You can build in VS or via the cake wrapper. Simply invoke
`.\build.ps1` to build the solution in Release mode. All
configurations (`Release`, `Debug`) are built for the `x64`
architecture.

To run a quick encryption test, use `.\build.ps1 test` instead.
This encrypts the shakespeare file and then decrypts it. Then, it
compares the hash of the original plaintext to that of the decrypted
ciphertext. This test is run in both ECB and CBC mode.

### PDF-ifying the Source
The `DES` project spec requires all source be concatenated into a
PDF. You can get halfway there with the `concat` cake task. This will
output `concat.cxx` into the project root directory. You can now use
[tohtml](http://tohtml.com) to colorize the source, and then copy /
paste the colorized source into Google Docs / your word editor of
choice and finally convert to PDF.

I haven't found an online API to convert from html to pdf yet...

## License
All projects are licensed under the MIT License unless otherwise stated. See
the `LICENSE` file in the root of this repository for details.
