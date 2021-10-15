                .ORIG x3000

;Init program
                LDI R4 MazeSTEP     ;存步长的正负值
                ST R4 STEPP         ;
                NOT R4 R4           ;
                ADD R4 R4 #1        ;
                ST R4 STEP          ;

                LDI R7 MazeLIMIT    ;乘法,计算迷宫规模
                LDI R1 MazeSTEP     ;
                AND R0 R0 #0        ;
MULTIN          ADD R0 R1 R0        ;
                ADD R7 R7 #-1       ;
                BRnp MULTIN         ;
                ADD R0 R0 #-1       ;
                LD R1 MazeMAP       ;
                ADD R0 R1 R0        ;
                NOT R0 R0           ;
                ADD R0 R0 #1        ;
                ST R0 LIMLINE       ;

                AND R2 R2 #0        ;
MMP             AND R3 R3 #0        ;R3 INIT
                LD R6 USER_SP       ;R6 STACK POINT INIT
                AND R5 R5 #0        ;R5 INIT
                LD R0 MazeSTART     ;
                LD R1 LIMLINE       ;
                ADD R1 R1 R0        ;
                BRnz GOON
                LD R1 ASCII
                ADD R0 R1 R2
                OUT
                HALT
GOON            ADD R0 R0 #1
                ST R0 MazeSTART

Begin           LD R4 MazeSTART     ;R4 INIT
                JSR Visit           ;LET THE HUNT BEGIN

Visit           ADD R3 R3 #1        ;R3常规加一
                ;LD R1 ASCII
                ;ADD R0 R3 R1
                ;OUT
                NOT R0 R3           ;
                ADD R0 R0 #1        ;
                ADD R0 R2 R0        ;
                BRzp Up             ;比大小看是否更新R2
                ADD R2 R3 #0        ;

Up              LD R0 STEP          ;UP模块
                LD R1 TOCOMPARE     ;
                ADD R0 R1 R0        ;是否存在
                ADD R0 R4 R0        ;
                BRn Down            ;

                LDR R1 R4 #0        ;新地址是否更小
                NOT R1 R1           ;
                ADD R1 R1 #1        ;R1 IS -R3
                LD R0 STEP          ;
                ADD R4 R4 R0        ;R4 更新//
                LDR R0 R4 #0        ;R0 IS R4'VALUE
                ADD R0 R1 R0
                BRzp REUP
                LEA R0 REUP
                JSR PUSH
                BRnzp Visit           ;成功跳转
REUP            LD R0 STEPP         ;不成功，便收回R4下一步
                ADD R4 R4 R0
                BRnzp Down

Down            LD R0 STEPP
                ADD R0 R4 R0
                LD R1 LIMLINE
                ADD R0 R1 R0
                BRp Right

                LDR R1 R4 #0
                NOT R1 R1 
                ADD R1 R1 #1
                LD R0 STEPP
                ADD R4 R4 R0
                LDR R0 R4 #0
                ADD R0 R1 R0
                BRzp REDOWN
                LEA R0 REDOWN
                JSR PUSH
                BRnzp Visit
REDOWN          LD R0 STEP
                ADD R4 R4 R0
                BRnzp Right

Right           LD R1 LIMLINE
                ADD R0 R4 R1
                BRz Left

                ADD R0 R4 #1
                NOT R0 R0
                ADD R0 R0 #1
                LD R1 MazeMAP
                ADD R0 R1 R0
                JSR IFSTEP
                ADD R0 R0 #0
                BRz Left  

                LDR R1 R4 #0
                NOT R1 R1
                ADD R1 R1 #1
                ADD R4 R4 #1
                LDR R0 R4 #0
                ADD R0 R1 R0
                BRzp RERIGHT
                LEA R0 RERIGHT
                JSR PUSH
                BRnzp Visit
RERIGHT         ADD R4 R4 #-1
                BRnzp Left

Left            LD R1 TOCOMPARE
                ADD R0 R4 R1
                BRz CHECKBEGIN

                ADD R0 R4 #0
                NOT R0 R0 
                ADD R0 R0 #1
                LD R1 MazeMAP
                ADD R0 R1 R0
                JSR IFSTEP
                ADD R0 R0 #0
                BRz CHECKBEGIN

                LDR R1 R4 #0
                NOT R1 R1
                ADD R1 R1 #1
                ADD R4 R4 #-1
                LDR R0 R4 #0
                ADD R0 R1 R0
                BRzp RELEFT
                LEA R0 RELEFT
                JSR PUSH
                BRnzp Visit
RELEFT          ADD R4 R4 #1
                BRnzp CHECKBEGIN

CHECKBEGIN      ADD R3 R3 #-1
                LD R0 MazeSTART
                NOT R0 R0
                ADD R0 R0 #1
                ADD R0 R4 R0
                BRz OPTT  
                BRnp REVERSE

REVERSE         JSR POP
                JMP R0

OPTT            BRnzp MMP

IFSTEP          LD R1 STEPP
LOOPER          ADD R0 R1 R0
                BRn LOOPER
                RET

MazeLIMIT       .FILL x4000     ;没几把用
MazeSTEP        .FILL x4001     ;步长存放的地址
MazeMAP         .FILL x4002     ;地图存放首地址
MazeSTART       .FILL x4001     ;Patt首地址
TOCOMPARE       .FILL xBFFE     ;存最小地址的负值
LIMLINE         .BLKW #1        ;存最大地址的负值
STEPP           .BLKW #1        ;存步长的正值
STEP            .BLKW #1        ;存步长的负值
LIMIT           .BLKW #1        ;没啥用的负值
BACKPOINT       .BLKW #1        ;存回去的PC指针
ASCII           .FILL X0030     ;int转ascii
RSEVEN          .BLKW #1        ;存R7

; The functions in the structrue Stack
POP             AND R5 R5 #0
                ST R1 Save1
                ST R2 Save2
                LD R1 EMPTY
                ADD R2 R6 R1
                BRz fail_exit
;
                LDR R0 R6 #0
                ADD R6 R6 #1
                BRnzp success_exit
;
PUSH            AND R5 R5 #0
                ST R1 Save1
                ST R2 Save2
                LD R1 FULL
                ADD R2 R6 R1
                BRz fail_exit
;
                ADD R6 R6 #-1
                STR R0 R6 #0
success_exit    LD R2 Save2
                LD R1 Save1
                RET
;
fail_exit       LD R2 Save2
                LD R1 Save1
                ADD R5 R5 #1
                RET
;
EMPTY           .FILL x0200; -xFE00
FULL            .FILL x3000; -xD000
Save1           .FILL x0000
Save2           .FILL x0000
USER_SP         .FILL xFE00
; Stack Range is from xFE00 to xD000
                .END