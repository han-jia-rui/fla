; This tm program implements the multiplication of 1-base numbers

; States
#Q = {start,check_a,check_b,reset,delete_a,add_c,illegal_input,print0,print1,print2,print3,print4,print5,print6,print7,print8,print9,print10,print11,print12,halt}

; Input alphabet
#S = {a,b}

; Tape alphabet
#G = {a,b,c,_,i,l,e,g,n,p,u,t}

; Start state
#q0 = start

; Blank symbol
#B = _

; Final states
#F = {halt}

; Tape number
#N = 3

; the transition functions
; format: <old state> <input> <output> <direction> <new state>

; <start>
start ___ ___ *** illegal_input
start b__ b__ *** illegal_input

start a__ a__ *** check_a

; <check_a>
check_a a__ _a_ rr* check_a

check_a ___ ___ *** illegal_input

check_a b__ b__ *** check_b

; <check_b>
check_b b__ __b r*r check_b

check_b a__ a__ *** illegal_input

check_b ___ ___ *ll reset

; <reset>
reset _ab _ab *ll reset
reset __b __b **l reset
reset _a_ _a_ *l* reset

reset ___ ___ *rr delete_a

; <delete_a>
delete_a _ab __b *r* add_c

delete_a __b __b l** halt

; <add_c>
add_c _ab cab r*r add_c
add_c __b c_b r*r add_c

add_c _a_ _a_ **l reset
add_c ___ ___ **l reset

; <illegal_input>
illegal_input a__ ___ r** illegal_input
illegal_input b__ ___ r** illegal_input
illegal_input ___ ___ *** print0

; <print0>
print0 ___ i__ r** print1

; <print1>
print1 ___ l__ r** print2

; <print2>
print2 ___ l__ r** print3

; <print3>
print3 ___ e__ r** print4

; <print4>
print4 ___ g__ r** print5

; <print5>
print5 ___ a__ r** print6

; <print6>
print6 ___ l__ r** print7

; <print7>
print7 ___ ___ r** print8

; <print8>
print8 ___ i__ r** print9

; <print9>
print9 ___ n__ r** print10

; <print10>
print10 ___ p__ r** print11

; <print11>
print11 ___ u__ r** print12

; <print12>
print12 ___ t__ *** halt

