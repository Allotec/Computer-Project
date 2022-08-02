addi $t1, $0, 30
main:
addi $t0, $t0, 5
beq $t1, $t0, end
j main
end:
nop
