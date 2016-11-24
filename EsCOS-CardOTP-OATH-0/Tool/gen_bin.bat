cd Tool
@echo off
if not exist ..\ObjFile\BinDec md ..\ObjFile\BinDec
if not exist ..\ObjFile\BinEnc md ..\ObjFile\BinEnc
@echo on
hexbin ..\ObjFile\Z8D16R_2.Hex ..\ObjFile\Z8D16R_2.bin I
hexbin ..\ObjFile\Z8D16R_2.Hex ..\ObjFile\BinDec\Z8D16R_2.bin I













