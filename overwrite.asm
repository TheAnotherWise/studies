BITS 16
ORG     0x7c00                  ;ORIGIN ADDRESS when loaded
 
jmp start
 
start:
        mov ax,cs
        mov ds,ax
        mov si,msg          ;SI now holds the message we wanna print
        call print
 
print:
        push ax
        cld
next:
        mov al,[si]
        cmp al,0            ;is the end of the string ?
        je done             ;if so,jump to done
        call printchar
        inc si              ;move to the next position in the string
        jmp next            ;repeat
done:
        jmp $               ;infinite loop

printchar:
        mov ah,0x0e         ;print char service
        int 0x10            ;print char
        ret
msg:            db        "DanielMagical O.o", 0

times 510 - ($-$$) db 0
dw        0xaa55
