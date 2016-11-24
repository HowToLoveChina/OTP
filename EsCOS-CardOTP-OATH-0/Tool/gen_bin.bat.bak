cd Tool
@echo off
if not exist ..\ObjFile\BinDec md ..\ObjFile\BinDec
if not exist ..\ObjFile\BinEnc md ..\ObjFile\BinEnc
@echo on
hexbin ..\ObjFile\Z8D16R.Hex ..\ObjFile\Z8D16R.bin I
hexbin ..\ObjFile\Z8D16R.Hex ..\ObjFile\BinDec\Z8D16R.bin I
otp_enc_tool ..\ObjFile\BinDec\Z8D16R.bin ..\ObjFile\BinEnc\Z8D16REN.bin












