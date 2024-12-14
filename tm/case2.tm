; This tm program identifies {1^p | p is a perfect square}

; States
#Q = {start,go,add1,add2,reset,accept,accept2,accept3,accept4,reject,reject2,reject3,reject4,reject5,halt}

; Input alphabet
#S = {1}

; Tape alphabet
#G = {1,_,t,r,u,e,f,a,l,s}

; Start state
#q0 = start

; Blank symbol
#B = _

; Final states
#F = {halt}

; Tape number
#N = 2

; the transition functions
; format: <old state> <input> <output> <direction> <new state>

; <start>
start __ __ ** reject
start 1_ 11 ** go

; <go>
go 11 _1 rr go

go 1_ 1_ ** add1
go _1 _1 ** reject
go __ __ ** accept

; <add1>
add1 1_ 11 *r add2

; <add2>
add2 1_ 11 ** reset

; <reset>
reset 11 11 *l reset
reset 1_ 1_ *r go

; <accept>
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt

; <reject>
reject __ f1 r* reject2
reject2 __ a1 r* reject3
reject3 __ l1 r* reject4
reject4 __ s1 r* reject5
reject5 __ e1 ** halt

reject _1 f1 r* reject2
reject2 _1 a1 r* reject3
reject3 _1 l1 r* reject4
reject4 _1 s1 r* reject5
reject5 _1 e1 ** halt

