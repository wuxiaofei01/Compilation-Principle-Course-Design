DSEG SEGMENT
main_ret DW 0
sum1 DW 0
sum2 DW 0
flag DW 0
fic_ret DW 0
nfic DW 0
temp2 DW 0
f_ret DW 0
nf DW 0
temp1 DW 0
maxn DW 0
DSEG ENDS

SSEG SEGMENT STACK
SKTOP DW 300 DUP(0)
SSEG ENDS

CSEG SEGMENT
ASSUME CS:CSEG,DS:DSEG,SS:SSEG

main PROC NEAR
block1:MOV DX,10
MOV nf,DX
CALL f
MOV AX,f_ret
MOV sum1,AX
MOV DX,3
MOV nfic,DX
CALL fic
MOV AX,fic_ret
MOV sum2,AX
MOV AX,sum1
MOV BX,sum2
CMP AX,BX
JLE block2
MOV AX,1
MOV flag,AX
JMP block3
block2:MOV AX,2
MOV flag,AX
block3:RET
main ENDP

block4:fic PROC NEAR
PUSH nfic
block5:MOV AX,nfic
MOV BX,1
CMP AX,BX
JNE block6
MOV CX,1
MOV fic_ret,CX
POP nfic
RET
block6:MOV AX,nfic
SUB AX,1
MOV temp2,AX
MOV DX,temp2
MOV nfic,DX
CALL fic
POP nfic
MOV AX,nfic
MOV BX,fic_ret
IMUL BL
MOV fic_ret,AX
RET
fic ENDP

block7:f PROC NEAR
PUSH nf
block8:MOV AX,nf
MOV BX,1
CMP AX,BX
JNE block9
MOV CX,1
MOV f_ret,CX
POP nf
RET
block9:MOV AX,nf
SUB AX,1
MOV temp1,AX
MOV DX,temp1
MOV nf,DX
CALL f
POP nf
MOV AX,nf
ADD AX,f_ret
MOV f_ret,AX
RET
f ENDP

block10:
START:MOV AX,DSEG
MOV DS,AX
MOV AX,SSEG
MOV SS,AX

MOV AX,20
MOV maxn,AX
block11:CALL main

MOV AH,4CH
INT 21H
CSEG ENDS
END START
