# Kaitlin Bleich
# Brent Lee

# to compile - type make, then run ./a.out
#TESTS:
!!
ls
ls -a
ls | grep a
cat Makefile | grep gcc
cat Makefile > hello.out
ls | grep b
!!
ls | grep M > hello.out
cat hello.out
!!
!!
# this will "hang", but you can still enter commands
# fun fact: if you run a.out, it will fix this :) Until you exit out of the child process
ls & 
ls | grep a
echo ls > somth.file
cat somth.file
./a.out < somth.file
rm somth.file
ls | grep somth
exit
