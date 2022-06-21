add $v0, $zero, $t0
slti $t1, $t2, 100 
label:
j label
l2:
addiu $sp,$sp,-16
beq $t1, $t2, l2
mult $s2, $t5    
