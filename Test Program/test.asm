move $t0, $v0
slti $t1, $t2, 100 
label:
j label
addiu $sp,$sp,-16
blt $t1, $t2, l2
l2:
li $t0, 0xFFFFF
lui $t0, 5
