MinGWPATH =  ../MinGW/mingw64/bin/

MAKE = $(MinGWPATH)mingw32-make.exe -r 

GCC  = $(MinGWPATH)gcc.exe  -mavx -mfma


MatMulAvx.exe:
	$(GCC) MatMulAvx.c Verify.c   -o MatMulAvx.exe

MatMulBase.exe:
	$(GCC)  MatMulBase.c  Verify.c -o MatMulBase.exe

MatMulAvxBlock.exe:
	$(GCC)   MatMulAvxBlock.c 	Verify.c  -o MatMulAvxBlock.exe

install1:
	$(MAKE)  MatMulBase.exe
install2:
	$(MAKE)  MatMulAvx.exe
install3 :
	$(MAKE)  MatMulAvxBlock.exe

