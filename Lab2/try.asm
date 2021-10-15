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
FOND            HALT            ;

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Please delet the code below when you check
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        .ORIG x4000
    
        .FILL NODE0

NODE0   .FILL NODE1
        .FILL NODE0FIELD1
        .FILL NODE0FIELD2
        .FILL NODE0FIELD3
NODE1   .FILL NODE2
        .FILL NODE1FIELD1
        .FILL NODE1FIELD2
        .FILL NODE1FIELD3
NODE2   .FILL NODE3
        .FILL NODE2FIELD1
        .FILL NODE2FIELD2
        .FILL NODE2FIELD3
NODE3   .FILL NODE4
        .FILL NODE3FIELD1
        .FILL NODE3FIELD2
        .FILL NODE3FIELD3
NODE4   .FILL NODE5
        .FILL NODE4FIELD1
        .FILL NODE4FIELD2
        .FILL NODE4FIELD3
NODE5   .FILL NODE6
        .FILL NODE5FIELD1
        .FILL NODE5FIELD2
        .FILL NODE5FIELD3
NODE6   .FILL NODE7
        .FILL NODE6FIELD1
        .FILL NODE6FIELD2
        .FILL NODE6FIELD3
NODE7   .FILL NODE8
        .FILL NODE7FIELD1
        .FILL NODE7FIELD2
        .FILL NODE7FIELD3
NODE8   .FILL NODE9
        .FILL NODE8FIELD1
        .FILL NODE8FIELD2
        .FILL NODE8FIELD3
NODE9   .FILL NODE10
        .FILL NODE9FIELD1
        .FILL NODE9FIELD2
        .FILL NODE9FIELD3
NODE10  .FILL NODE11
        .FILL NODE10FIELD1
        .FILL NODE10FIELD2
        .FILL NODE10FIELD3
NODE11  .FILL NODE12
        .FILL NODE11FIELD1
        .FILL NODE11FIELD2
        .FILL NODE11FIELD3
NODE12  .FILL NODE13
        .FILL NODE12FIELD1
        .FILL NODE12FIELD2
        .FILL NODE12FIELD3
NODE13  .FILL x0000
        .FILL NODE13FIELD1
        .FILL NODE13FIELD2
        .FILL NODE13FIELD3

NODE0FIELD1 .STRINGZ "101"
NODE0FIELD2 .STRINGZ "Yale"
NODE0FIELD3 .STRINGZ "Patt"
NODE1FIELD1 .STRINGZ "413"
NODE1FIELD2 .STRINGZ "Circle"
NODE1FIELD3 .STRINGZ "Lin"
NODE2FIELD1 .STRINGZ "403"
NODE2FIELD2 .STRINGZ "Wei"
NODE2FIELD3 .STRINGZ "Rentao"
NODE3FIELD1 .STRINGZ "416"
NODE3FIELD2 .STRINGZ "Zhang"
NODE3FIELD3 .STRINGZ "Ru"
NODE4FIELD1 .STRINGZ "411"
NODE4FIELD2 .STRINGZ "Zhou"
NODE4FIELD3 .STRINGZ "Haoyang"
NODE5FIELD1 .STRINGZ "410"
NODE5FIELD2 .STRINGZ "Yang"
NODE5FIELD3 .STRINGZ "Zhiquan"
NODE6FIELD1 .STRINGZ "402"
NODE6FIELD2 .STRINGZ "Liang"
NODE6FIELD3 .STRINGZ "Chen"
NODE7FIELD1 .STRINGZ "309"
NODE7FIELD2 .STRINGZ "Guo"
NODE7FIELD3 .STRINGZ "Ruorong"
NODE8FIELD1 .STRINGZ "412"
NODE8FIELD2 .STRINGZ "Wang"
NODE8FIELD3 .STRINGZ "Lichao"
NODE9FIELD1 .STRINGZ "414"
NODE9FIELD2 .STRINGZ "Xu"
NODE9FIELD3 .STRINGZ "Zikai"
NODE10FIELD1 .STRINGZ "417"
NODE10FIELD2 .STRINGZ "Bao"
NODE10FIELD3 .STRINGZ "Dezheng"
NODE11FIELD1 .STRINGZ "411"
NODE11FIELD2 .STRINGZ "Yang"
NODE11FIELD3 .STRINGZ "Bochun"
NODE12FIELD1 .STRINGZ "402"
NODE12FIELD2 .STRINGZ "Ji"
NODE12FIELD3 .STRINGZ "Gaoqiang"
NODE13FIELD1 .STRINGZ "520"
NODE13FIELD2 .STRINGZ "Jiang"
NODE13FIELD3 .STRINGZ "Xiaohong"

        .END