## HxCrypt Tool

This tool can make patch file for some games that used new encryption.

> No source code of this tool here.

### Usage

Before use the tool you must find all key for encryption.

1. Index Key ( 32 bytes )
2. Index Nonce Key ( 16 bytes )
3. Index Verification Key ( 32 bytes )
4. Cxdec Table ( 4096 bytes )
5. Cxdec Order ( 17 bytes )
6. Cxdec Random ( 0 or 1 )
7. Filter Base Split Position ( hex number )
8. Filter Split Position Mask ( hex number )
9. Filter Key ( hex number )

### Note

The framework of there new games will not read `patch.xp3` or `patch2.xp3` etc.

Create patch with name like `patch@r456.xp3`, the number `456` mean `revision number`.

Try increase `revision number` if the game doesn't read patch.

Don't create directory structure in patch, put all the files into root of patch.
