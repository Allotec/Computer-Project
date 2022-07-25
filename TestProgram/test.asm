.data
var1:	.word	7		# declare storage for var1; initial value is 23
array1:		.byte	'a','b','c','d'	# create a 2-element character array with elements initialized
				#   to  a  and  b
array2:		.space	40	# allocate 40 consecutive bytes, with storage uninitialized
				#   could be used as a 40-element character array, or a
				#   10-element integer array; a comment should indicate which!
.text
main:
li	$v0, 5			# load appropriate system call code into register $
syscall				# call operating system to perform operation
move $s0, $v0