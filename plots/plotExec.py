import matplotlib.pyplot as plt

file = open("exec_time.txt",'r')
lines = file.readlines()
file.close()

vertex = []
fwTime = []
bfsTime = []

for i in range(len(lines)):
	v,f,b = lines[i].rstrip().split()
	vertex.append(int(v))
	fwTime.append(float(f))
	bfsTime.append(float(b))

plt.loglog(vertex,fwTime,label = 'fwBC')
plt.loglog(vertex,bfsTime, label = 'bfsBC')
plt.grid(True,which='both',ls='-')
plt.xticks(vertex)
plt.legend(loc = 'upper left')
plt.xlabel('No. of vertices')
plt.ylabel('Execution time in seconds')
plt.suptitle('Probability of having edge = 0.5')
plt.savefig('exec_time.png')
plt.clf()

file = open("exec_time_sparse.txt",'r')
lines = file.readlines()
file.close()

vertex = []
fwTime = []
bfsTime = []

for i in range(len(lines)):
	v,f,b = lines[i].rstrip().split()
	# print(v,f,b)
	vertex.append(int(v))
	fwTime.append(float(f))
	bfsTime.append(float(b))

plt.loglog(vertex,fwTime,label = 'fwBC')
plt.loglog(vertex,bfsTime, label = 'bfsBC')
plt.xticks(vertex)
plt.grid(True,which='both',ls='-')
plt.legend(loc = 'upper left')
plt.xlabel('No. of vertices')
plt.ylabel('Execution time in seconds')
plt.suptitle('Probability of having edge = 0.02')
plt.savefig('exec_time_sparse.png')
plt.clf()

file = open("exec_time_prob.txt",'r')
lines = file.readlines()
file.close()

prob = []
fwTime = []
bfsTime = []

for i in range(len(lines)):
	p,f,b = lines[i].rstrip().split()
	prob.append(float(p))
	fwTime.append(float(f))
	bfsTime.append(float(b))

plt.loglog(prob,fwTime,label = 'fwBC')
plt.loglog(prob,bfsTime, label = 'bfsBC')
plt.xticks(vertex)
plt.grid(True,which='both',ls='-')
plt.legend(loc = 'upper left')
plt.xlabel('Probability of having an edge')
plt.ylabel('Execution time in seconds')
plt.suptitle('No. of vertices = 1000')
plt.savefig('exec_time_prob.png')