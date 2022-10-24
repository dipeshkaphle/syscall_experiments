import os
import subprocess
import sys
import select
import resource

fifo_1 = str(2*int(sys.argv[1]))
fifo_2 = str(2*int(sys.argv[1])+1)
os.mkfifo(str(fifo_1))
os.mkfifo(str(fifo_2))
efd1 = os.eventfd(0,os.EFD_SEMAPHORE )
efd2 = os.eventfd(0,os.EFD_SEMAPHORE)
print(efd1,efd2)

def setlimits():
    #  pass
    # Set maximum CPU time to 1 second in child process, after fork() but before exec()
    # print("Setting resource limit in child (pid %d)" % os.getpid())
    resource.setrlimit(resource.RLIMIT_CPU, (1,1))
    # print(resource.getrlimit())

r1 = os.open(fifo_1, os.O_RDONLY | os.O_NONBLOCK)
w1 = os.open(fifo_1, os.O_WRONLY)
os.set_blocking(r1,True)
r2 = os.open(fifo_2, os.O_RDONLY | os.O_NONBLOCK)
w2 = os.open(fifo_2, os.O_WRONLY)
os.set_blocking(r2,True)

p1 = subprocess.Popen([ "./p1", str(efd1), str(efd2)], stdin=r1, stdout =w2, preexec_fn=setlimits, close_fds=False)
p2 = subprocess.Popen([ "./p2", str(efd1), str(efd2)], stdin=r2, stdout =w1, preexec_fn=setlimits, close_fds=False)


fds = {os.pidfd_open(p1.pid) : p1 , os.pidfd_open(p2.pid) : p2}

fds_as_list=list(fds.keys())

epoll = select.epoll()
epoll.register(fds_as_list[0],select.EPOLLIN)
epoll.register(fds_as_list[1],select.EPOLLIN)

print(fds)
while len(fds)!=0:
    events = epoll.poll(-1)
    for fd, event in events:
        #  print(fd, event)
        if event & select.EPOLLIN:
            if(fds[fd].wait() !=0):
                for k,v in fds.items():
                    v.kill()
                    epoll.unregister(k)
                write(2,b"KILLED")
                fds = {}
                break
            else:
                epoll.unregister(fd)
                del fds[fd]
                break
#  subprocess.
#  ret1= p1.wait()
#  ret2= p2.wait()

#  assert(ret1==0)
#  assert(ret2==0)

os.unlink(str(fifo_1))
os.unlink(str(fifo_2))
