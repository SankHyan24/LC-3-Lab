                .orig x0200
;ADD THE PRIVILEGE
                LDI R0 KBSR
                LD R1 MASK14
                NOT R0 R0
                AND R0 R0 R1
                NOT R0 R0
                STI R0 KBSR
;ADD THE INTV
                LD R0 INTADRS
                STI R0 INTV
;OS OPTIONS
                LD R6 OS_SP
                LD R0 USER_PSR
                ADD R6 R6 #-1
                STR R0 R6 #0      ;R0 push usp
                LD R0 USER_PC
                ADD R6 R6 #-1
                STR R0 R6 #0
                RTI
OS_SP           .FILL x3000
USER_PSR        .FILL x8002
USER_PC         .FILL x3000
MASK14          .FILL xBFFF     
KBSR            .FILL xFE00         
KBDR            .FILL xFE02 
INTV            .FILL x0180
INTADRS         .FILL x0800
                .end

                .orig x0800
                LDI R0 INTKBDR
                LD R1 INTNWL
                ADD R1 R0 R1
                BRz INTMINUS
                LD R1 RANGE1
                ADD R1 R0 R1
                BRn ZHIJIE
                LD R1 RANGE2
                ADD R1 R0 R1
                BRp ZHIJIE
                ADD R1 R0 #0
                STI R1 INTIPPS
                RTI
                
INTMINUS        LDI R1 INTIPPS
                LD R2 RANGE1
                ADD R2 R1 R2
                BRz INTRTN
                ADD R1 R1 #-1
                STI R1 INTIPPS
INTRTN          RTI

ZHIJIE          LD R5 INT_LIMIT
                ADD R1 R0 #0
                LD R0 INTNWLP
                OUT
                ADD R0 R1 #0
INT_OPT         OUT
                ADD R5 R5 #1
                BRz INTEND
                BRnzp INT_OPT
INTEND          LD R0 INTNWLP
                OUT
                RTI

INT_LIMIT       .FILL -40
INTKBDR         .FILL xFE02
INTIPPS         .FILL NUMBER
INTNWL          .FILL -10
INTNWLP         .FILL 10
RANGE1          .FILL -48
RANGE2          .FILL -57      
                .end
                
                
                
                .orig x3000
NUMBER          .BLKW #1    

;OUTPUTPROGRAM
INIT            AND R5 R5 #0        ;
                LD R1 FIFYFW        ;
                ST R1 NUMBER        ;
OTPT            LD R1 NUMBER        ;
                ADD R0 R1 #0        ;
                OUT                 ;
                JSR DELAY           ;
                JSR ADNEWL
                BRnzp OTPT

ADNEWL          ADD R5 R5 #1
                LD R0 CHEKFT
                ADD R0 R5 R0
                BRz OTPTNL
                RET
OTPTNL          LD R0 NEWLINE
                OUT
                AND R5 R5 #0
                RET 

NEWLINE         .FILL X000A
CHEKFT          .FILL -40
FIFYFW          .FILL 55    

DELAY           ST R1 DELAY_R1
                LD R1 DELAY_COUNT
DELAY_LOOP      ADD R1 R1 #-1
                BRnp DELAY_LOOP
                LD R1 DELAY_R1
                RET

DELAY_COUNT     .FILL #256
DELAY_R1        .BLKW #1
            
                .end