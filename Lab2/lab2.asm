                .ORIG x3000
;R0 is the input value
;R1 is the data base value
;R2 is the point of the string database
;R3 is the "if found flag"
;R4 is not used 
;R5 is the point of input string base
;R6 is the point of linked list

;The begining of the program
                AND R3,R3,#0    ;
                ADD R3,R3,#1    ;
                LD R0 HLWD1     ;
                PUTS            ;
                LD R5 STPHEAD   ;

;The input program
INPUT           GETC            ;
                OUT             ;
                ADD R1,R0,#-10  ;
                BRz MATCH       ;
                STR R0,R5,#0    ;
                ADD R5,R5,#1    ;
                BRnzp  INPUT    ;
                                
MATCH           LD R5 STPHEAD   ;R5 RESET
                LDI R6 DATHEAD  ;R6 SET (ADDRESS OF NODE0)
                BRnzp STRCMPY   ;
                                
;Check the first name:          
STRCMPY         LDR R0,R5,#0    ;
                LDR R2,R6,#2    ;
CHECK           LDR R1,R2,#0    ;
                NOT R0,R0       ;
                ADD R0,R0,#1    ;
                ADD R0,R1,R0    ;
                BRnp NEXT       ;
                ADD R1,R1,#0    ;
                BRnp GOON       ;       
                JSR FETCH       ;      
                BRnzp NEXT      ;
GOON            ADD R5,R5,#1    ;
                ADD R2,R2,#1    ;
                LDR R0,R5,#0    ;
                LDR R1,R2,#0    ;
                BRnzp CHECK     ;
NEXT            LDR R6,R6,#0    ;
                ;BRz EDPRO      ;
                BRz REGOGO      ;
                LD R5 STPHEAD   ;
                BRnzp STRCMPY   ;
                                
;Check the last name:           
REGOGO          LDI R6 DATHEAD  ;R6 SET (ADDRESS OF NODE0)
STRCMPY2        LDR R0,R5,#0    ;
                LDR R2,R6,#3    ;
CHECK2          LDR R1,R2,#0    ;
                NOT R0,R0       ;
                ADD R0,R0,#1    ;
                ADD R0,R1,R0    ;
                BRnp NEXT2      ;
                ADD R1,R1,#0    ;
                BRnp GOON2      ;
                JSR FETCH       ;
                BRnzp NEXT2     ;      
GOON2           ADD R5,R5,#1    ;
                ADD R2,R2,#1    ;
                LDR R0,R5,#0    ;
                LDR R1,R2,#0    ;
                BRnzp CHECK2    ;
NEXT2           LDR R6,R6,#0    ;
                BRz EDPRO       ;
                LD R5 STPHEAD   ;
                BRnp STRCMPY2   ;
        
;The end of program
FETCH           LDR R2,R6,#2    ;
                ADD R0,R2,#0    ;
                PUTS            ;
                LD R0 SPACE     ;
                PUTC            ;
                LDR R2,R6,#3    ;
                ADD R0,R2,#0    ;
                PUTS            ;
                LD R0 SPACE     ;
                PUTC            ;
                LDR R2,R6,#1    ;
                ADD R0,R2,#0    ;
                PUTS            ;
                LD R0 ENTER     ;
                PUTC            ;
                AND R3,R3,#0    ;
                RET             ;
                                
EDPRO           ADD R3,R3,0     ;
                BRz FOND
                LD R0 ERWD1     ;
                PUTS            ;
FOND            AND R3,R3,#0    ;
                ADD R3,R3,#1    ;
                HALT            ;

;Strings Appendix
STPHEAD         .FILL STORRST   ;
SPACE1          .FILL SPACE     ;
ENTER1          .FILL ENTER     ;
HLWD1           .FILL HLWD      ;
ERWD1           .FILL ERWD      ;
DATHEAD         .FILL    X4000  ;

;String Data base
HLWD            .STRINGZ "Enter a name: "
ERWD            .STRINGZ "Not found"
ENTER           .FILL x000A
SPACE           .FILL x0020
STORRST         .BLKW    #32    ;TO STORE THE STRING

                .END        

