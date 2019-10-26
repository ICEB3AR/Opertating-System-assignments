from pwn import*
import random
import string
import time
s=process(['client'],env={"LD_PRELOAD":""})

passed =0
failed =0
trial = 1000
for j in range(trial):
    s.recvuntil('USEAGE')
    if j %2 == 0: #write
        rand_string = ''.join(random.choice(string.ascii_uppercase+string.digits) for _ in range(random.randrange(1,101)))
        s.sendline(str(j)+'.test -w '+rand_string)
    else:
        size = random.randrange(2,101)
        s.sendline(str(j-1)+'.test -r '+str(size))
        s.recvuntil('response: ')
        out = s.recvline()
        #sleep(0.5)
        f = open(str(j-1)+'.test','r')
        real = f.read(size)
        print '[*] Test'+str(j)
        print real+" : "+out
        if real in out:
            print "PASS"
            passed += 1
        else:
            print "FAIL"
            failed += 1
        f.close()
        #sleep(0.5)

print "RESULT : total "+str(trial/2)+" trial "+str(passed)+" pass / "+str(failed)+" fail " +str((passed/(float(trial)/2))*100)+"%"
s.interactive()
