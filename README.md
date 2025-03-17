# exam_06
one last sigh for old time sake (not featuring Dr dre)

# road to exam
1. small http answer on defined port at runtime
2. multiple connections saved and listed
3. small client that connects, waits for message to be sent to said client.
4. eeeeee

# ressources
code walkthrough : https://www.youtube.com/watch?v=hfQhGAinjS0&ab_channel=Glootie
select() : https://www.youtube.com/watch?v=Y6pFtgRdUts&ab_channel=JacobSorber
socket programming: https://www.youtube.com/watch?v=LtXEMwSG5-8&ab_channel=EduonixLearningSolutions
linux socket api: https://www.youtube.com/watch?v=XXfdzwEsxFk&t=749s&ab_channel=ChrisKanich
big playlist mayube skip it: https://www.youtube.com/watch?v=SrGyi0WOTBk&list=PLysdvSvCcUhbOBztd-CdB5fCXFrvwk107&index=1&ab_channel=IndronilBanerjee


# allowed functions runthrough
### the stuff we know
write
close
malloc
free
calloc
memset
bzero
atoi
strlen
exit

### the stuff we knew
strstr - return pointer to needle in haystack
sprintf - write to str
strcpy - put arg1 into arg2 (copy str) - manage memory allocation yourself
strcat - concatenates strings

### the stuff thats new
select
socket
accept
listen
send
recv
bind
realloc